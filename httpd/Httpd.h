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

    //endregion

    //region METHODS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

private:

    std::size_t hash(std::string &path, RequestType &type);

public:

    /**
     * Add a request handler function for a given path.
     *
     * param requestType the type of the http request (GET, POST etc.)
     * param path the url path of the request (e.g. "/" or "/index.html"
     */
    void addHandler(RequestType requestType,
                    std::string path,
                    Httpd::RequestHandler fp);

    /**
     * Start the http daemon and listen on a supplied port.
     *
     * param port the port to listen on
     */
    void listenOnPort(uint16_t port);

    /**
     * Stop the http daemon.
     */
    void stopListening();

    int enqueueResponse(struct MHD_Connection *connection, std::unique_ptr<Response> response);
    Httpd::RequestHandler findRequestHandler(std::string &path, const char* type);

    //endregion

};


#endif //WILLKAMP_HTTPSERVER_H
