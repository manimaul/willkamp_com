//
// Created by William Kamp on 7/3/16.
//

#include <iostream>
#include "Httpd.h"
#include <microhttpd.h>
#include <vector>

static int keyValueIter(void *cls,
                        enum MHD_ValueKind kind,
                        const char *key,
                        const char *value) {
    std::unordered_map<std::string, std::string> *keyValues = (std::unordered_map<std::string, std::string> *) cls;
    auto got = keyValues->find(key);
    if (got != keyValues->end()) {
        keyValues->insert({key, value});
        return MHD_YES; // continue iter
    }
    return MHD_NO; // end iter
}

static int answerConnection(void *pCls,
                            struct MHD_Connection *pConn,
                            char const *pUrl,
                            char const *pMethod,
                            char const *version,
                            char const *upload_data,
                            size_t *upload_data_size,
                            void **con_cls) {

    /*
     * Fetch the request body
     */
    Request *rawRequest = (Request *) *con_cls;
    if (nullptr == rawRequest) {
        std::unordered_map<std::string, std::string> headers;
        std::unordered_map<std::string, std::string> params;
        MHD_get_connection_values(pConn, MHD_HEADER_KIND, &keyValueIter, &headers);
        MHD_get_connection_values(pConn, MHD_GET_ARGUMENT_KIND, &keyValueIter, &params);

        *con_cls = (void *) new Request(headers, params);
        return MHD_YES;
    }

    if (*upload_data_size) {
        rawRequest->appendBodyData(upload_data, upload_data_size);
        *upload_data_size = 0;
        return MHD_YES;
    }

    Httpd *httpd = (Httpd *) pCls;
    std::unique_ptr<Request> request(rawRequest);

    std::string path = pUrl;
    Httpd::RequestHandler handler = httpd->findRequestHandler(path, pMethod);
    if (nullptr != handler) {
        Response resp = handler(std::move(request));
        return httpd->enqueueResponse(pConn, std::move(resp));
    }

    /*
     * Answer with 404 not found
     */
    return httpd->enqueueResponse(pConn, Response("{\"message\": \"not found\"}", ResponseCode::NOT_FOUND));
}

Httpd::RequestHandler Httpd::findRequestHandler(std::string &path, const char *type) {
    RequestType t = UNKNOWN;
    if (0 == strcmp(type, MHD_HTTP_METHOD_GET)) {
        t = RequestType::GET;
    } else if (0 == strcmp(type, MHD_HTTP_METHOD_POST)) {
        t = RequestType::POST;
    } else if (0 == strcmp(type, MHD_HTTP_METHOD_DELETE)) {
        t = RequestType::DELETE;
    }
    HandlerMap::const_iterator itor = handlers->find(hash(path, t));
    if (itor == handlers->end()) {
        return nullptr;
    } else {
        return itor->second;
    }
}

int Httpd::enqueueResponse(struct MHD_Connection *connection, Response response) {
    struct MHD_Response *mhdResponse = MHD_create_response_from_buffer(response.bodySize(),
                                                                       (void *) response.getBody().c_str(),
                                                                       MHD_RESPMEM_MUST_COPY);
    for (auto item : response.get_headers()) {
        MHD_add_response_header(mhdResponse, item.first.c_str(), item.second.c_str());
    }
    int ret = MHD_queue_response(connection, response.getCode(), mhdResponse);
    MHD_destroy_response(mhdResponse);
    return ret;
}

void Httpd::listenOnPort(uint16_t port) {
    if (nullptr == daemon) {
        daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY,
                                  port,
                                  nullptr,
                                  nullptr,
                                  &answerConnection,
                                  this,
                                  MHD_OPTION_NOTIFY_COMPLETED,
                                  nullptr,
                                  nullptr, MHD_OPTION_END);
    } else {
        std::cout << "no-op: already listening" << std::endl;
    }
}

void Httpd::addHandler(RequestType requestType,
                       std::string path,
                       RequestHandler fp) {
    std::size_t h = hash(path, requestType);
    handlers->insert({h, fp});
}

void Httpd::stopListening() {
    if (nullptr != Httpd::daemon) {
        MHD_stop_daemon(Httpd::daemon);
    }
}

std::size_t Httpd::hash(std::string &path, RequestType &type) {
    std::size_t h1 = std::hash<std::string>()(path);
    std::size_t h2 = std::hash<RequestType>()(type);
    return h1 ^ (h2 << 1);
}