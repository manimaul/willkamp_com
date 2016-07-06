//
// Created by William Kamp on 7/3/16.
//

#include <iostream>
#include "Httpd.h"
#include <microhttpd.h>

int answerConnection(void *pCls,
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
        *con_cls = (void*) new Request();
        return MHD_YES;
    }
    if (*upload_data_size) {
        rawRequest->body.append(upload_data, *upload_data_size);
        *upload_data_size = 0;
        return MHD_YES;
    }

    Httpd *httpd = (Httpd *) pCls;
    std::unique_ptr<Request> request(rawRequest);

    std::string path = pUrl;
    Httpd::RequestHandler handler = httpd->findRequestHandler(path, pMethod);
    if (nullptr != handler) {
        request->setBody(upload_data);
        std::unique_ptr<Response> resp = handler(std::move(request));
        return httpd->enqueueResponse(pConn, std::move(resp));
    }

    /*
     * Answer with 404 not found
     */
    auto response = std::make_unique<Response>("{\"message\": \"not found\"}", ResponseCode::NOT_FOUND);
    return httpd->enqueueResponse(pConn, std::move(response));
}

Httpd::RequestHandler Httpd::findRequestHandler(std::string &path, const char* type) {
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

int Httpd::enqueueResponse(struct MHD_Connection *connection, std::unique_ptr<Response> resp) {
    struct MHD_Response *response;
    response = MHD_create_response_from_buffer(resp->body.size(), (void *) resp->body.c_str(), MHD_RESPMEM_MUST_COPY);
    for (auto it = resp->get_headers().begin(); it != resp->get_headers().end(); ++it) {
        MHD_add_response_header(response, it->first.c_str(), it->second.c_str());
    }
    int ret = MHD_queue_response(connection, resp->code, response);
    MHD_destroy_response(response);
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