//
// Created by William Kamp on 7/3/16.
//

#ifndef WILLKAMP_HTTPSERVER_H
#define WILLKAMP_HTTPSERVER_H

#include <string>
#include <functional>
#include <unordered_map>
#include "Response.h"
#include "Request.h"
#include "RequestTypes.h"


class Httpd {

    //region DEFINITIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

public:
    typedef std::function<std::unique_ptr<Response>(std::unique_ptr<Request>)> RequestHandler;

private:
    typedef std::unordered_map<std::size_t, RequestHandler> HandlerMap;

    //endregion

    //region MEMBERS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

private:
    struct MHD_Daemon *daemon;
    std::unique_ptr<HandlerMap> handlers;

    //endregion

    //region CONSTRUCTOR ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

public:

    Httpd() {
        handlers = std::make_unique<HandlerMap>(HandlerMap());
        daemon = nullptr;
    }

    virtual ~Httpd() {
        stopListening();
    }

    RequestHandler findRequestHandler(std::string &path, RequestType type);

    //endregion

    //region METHODS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

private:
    std::size_t hash(std::string &path, RequestType &type);

public:
    void addHandler(RequestType requestType,
                    std::string path,
                    RequestHandler fp);

    void listenOnPort(uint16_t port);

    void stopListening();

    //endregion

};


#endif //WILLKAMP_HTTPSERVER_H
