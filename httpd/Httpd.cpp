//
// Created by William Kamp on 7/3/16.
//

#include <iostream>
#include "Httpd.h"
#include "Response.h"
#include <microhttpd.h>

/**
 * The handler function for all http requests that conform to the MHD_AccessHandlerCallback protocol in microhttpd.h
 */
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
    if (nullptr == *con_cls) {
        *con_cls = (void *) 1;
        return MHD_YES;
    }
    if (*upload_data_size) {
        //todo append data
        return MHD_YES;
    }

    Httpd *httpd = (Httpd *) pCls;

    std::string path = pUrl;
    if (0 == strcmp(pMethod, MHD_HTTP_METHOD_GET)) {
        Httpd::RequestHandler handler = httpd->findRequestHandler(path, RequestType::GET);
        if (nullptr != handler) {
            std::unique_ptr<Request> request = std::make_unique<Request>();
            request->setBody(upload_data);
            std::unique_ptr<Response> resp = handler(std::move(request));

            struct MHD_Response *response;
            response = MHD_create_response_from_buffer(resp->body.size(), (void *) resp->body.c_str(), MHD_RESPMEM_MUST_COPY);
            //todo: add headers from resp
            MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_TYPE, "application/json");
            int ret = MHD_queue_response(pConn, MHD_HTTP_NOT_FOUND, response);
            MHD_destroy_response(response);
            return ret;
        }

    } else if (0 == strcmp(pMethod, MHD_HTTP_METHOD_POST)) {

    } else if (0 == strcmp(pMethod, MHD_HTTP_METHOD_DELETE)) {

    }

    //todo hook up handlers

    /*
     * Answer with 404 not found
     */
    *upload_data_size = 0;
    return MHD_NO; //todo 404

}

void Httpd::listenOnPort(uint16_t port) {
    if (nullptr == daemon) {
        std::cout << "listening on port: " << port << std::endl;
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
    std::cout << "addHandler" << std::endl;
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

Httpd::RequestHandler Httpd::findRequestHandler(std::string &path, RequestType type) {
    HandlerMap *map = handlers.get();
    HandlerMap::const_iterator itor = map->find(hash(path, type));
    if (itor == map->end()) {
        return nullptr;
    } else {
        return itor->second;
    }
}












