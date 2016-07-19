#include <iostream>
#include <thread>
#include <unistd.h>
#include "httpd/Httpd.h"
#include "mongo/Mongo.h"

using namespace std;

static const string kDbUrl = "mongodb://localhost:27017/";
static const string kDb = "wk";
static const string kDbCollectionPages = "pages";
static const string kApiPublic = "/api/v1/public";
static const string kApiPrivate = "/api/v1/private";
static const int kPort = 8080;
static bool running = true;

class Handler {

    Mongo mongo;

public:

    Handler() : mongo(Mongo(kDbUrl)) { }

public:

    /**
     * Request handler for GET "/"
     */
    Response handleRoot(unique_ptr<Request> request) {
        cout << "handleRoot" << endl;
        for (auto p : request->getHeaders()) {
            cout << "Header: " << p.first << " : " << p.second << endl;
        }
        for (auto p : request->getParams()) {
            cout << "Param: " << p.first << " : " << p.second << endl;
        }
        request->getBody();
        Bson bson;
        bson.add("message", "ok");
        auto response = Response(bson.getJson(), ResponseCode::OK);
        response.addHeader("Content-Type", "application/json");
        return response;
    }

    /**
     * Request handler for POST "/page"
     */
    Response handleAddPage(unique_ptr<Request> request) {
        cout << "handleAddPage" << endl;
        for (auto p : request->getHeaders()) {
            cout << "Header: " << p.first << " : " << p.second << endl;
        }
        for (auto p : request->getParams()) {
            cout << "Param: " << p.first << " : " << p.second << endl;
        }
        MongoCollection collection = mongo.getCollection(kDb, kDbCollectionPages);
        collection.upsertRecord(request->getBody());
        Bson bson;
        bson.add("message", "ok");
        auto response = Response(bson.getJson(), ResponseCode::OK);
        response.addHeader("Content-Type", "application/json");
        return response;
    }

    /**
     * Request handler for GET "/page"
     */
    Response handleGetPage(unique_ptr<Request> request) {
        cout << "handleGetPage" << endl;
        for (auto p : request->getHeaders()) {
            cout << "Header: " << p.first << " : " << p.second << endl;
        }
        for (auto p : request->getParams()) {
            cout << "Param: " << p.first << " : " << p.second << endl;
        }
        MongoCollection collection = mongo.getCollection(kDb, kDbCollectionPages);

        string body = request->getBody();
        string respBody = collection.findOneRecord(body);
        auto response = Response(respBody, ResponseCode::NOT_FOUND);
        response.addHeader("Content-Type", "application/json");
        return response;
    }

    Response notFound(unique_ptr<Request> request) {
        Bson respBody;
        auto response = Response(respBody.getJson(), ResponseCode::NOT_FOUND);
        return response;
    }

//    void printRequestHeadersAndParams(std::unordered_map<std::string, std::string> )

};


int main() {
    auto server = make_unique<Httpd>();
    auto handler = make_unique<Handler>();

    auto rh = bind(&Handler::handleRoot, handler.get(), placeholders::_1);
    server->addHandler(RequestType::GET, "/", rh);

    rh = bind( &Handler::handleAddPage, handler.get(), placeholders::_1);
    server->addHandler(RequestType::POST, kApiPublic + "/page", rh);

    rh = bind( &Handler::handleGetPage, handler.get(), placeholders::_1);
    server->addHandler(RequestType::GET, kApiPublic + "/page", rh);

    thread t([&] () {
        server->listenOnPort(kPort);
        while(running) {
            usleep(1000 * 1000);
        }
    });

    t.join();
    return 0;
}