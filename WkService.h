//
// Created by William Kamp on 7/19/16.
//

#ifndef WILLKAMP_WKSERVICE_H
#define WILLKAMP_WKSERVICE_H

#include <iostream>
#include <thread>
#include <unistd.h>
#include "httpd/Httpd.h"
#include "mongo/Mongo.h"

static const std::string kDbUrl = "mongodb://localhost:27017/";
static const std::string kDb = "wk";
static const std::string kDbCollectionPages = "pages";
static const std::string kApiPublic = "/api/v1/public";
static const std::string kApiPrivate = "/api/v1/private";
static const int kPort = 8080;

class WkService {

private:
    Mongo mongo;

public:

    WkService() : mongo(Mongo(kDbUrl)) { }

public:

    /**
     * Request handler for GET "/"
     */
    Response handleRoot(std::unique_ptr<Request> request);

    /**
     * Request handler for POST "/page"
     */
    Response handleAddPage(std::unique_ptr<Request> request);

    /**
     * Request handler for GET "/page"
     */
    Response handleGetPage(std::unique_ptr<Request> request);

private:

    Response notFound();

    void printRequestHeadersAndParams(Request &request);

};


#endif //WILLKAMP_WKSERVICE_H
