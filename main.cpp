#include <iostream>
#include <thread>
#include <unistd.h>
#include "httpd/Httpd.h"
#include "mongo/Mongo.h"

#define DB_URL "mongodb://localhost:27017/"
#define DB_WK "wk"
#define DB_COL_PAGES "pages"

static const int kPort = 8080;
bool running = true;

using namespace std;

class Handler {

    Mongo mongo;

public:

    Handler() : mongo(Mongo(DB_URL)) { }

public:

    Response handleRoot(unique_ptr<Request> request) {
        std::cout << "handleRoot" << std::endl;
        for (auto p : request->getHeaders()) {
            std::cout << "Header: " << p.first << " : " << p.second << std::endl;
        }
        for (auto p : request->getParams()) {
            std::cout << "Param: " << p.first << " : " << p.second << std::endl;
        }
        request->getBody();
        auto response = Response("{}", ResponseCode::OK);
        response.addHeader("Content-Type", "application/json");
        return response;
    }

    Response handleAddPage(unique_ptr<Request> request) {
        std::cout << "handleAddPage" << std::endl;
        for (auto p : request->getHeaders()) {
            std::cout << "Header: " << p.first << " : " << p.second << std::endl;
        }
        for (auto p : request->getParams()) {
            std::cout << "Param: " << p.first << " : " << p.second << std::endl;
        }
        MongoCollection collection = mongo.getCollection(DB_WK, DB_COL_PAGES);
        collection.upsertRecord(request->getBody());
        auto response = Response("{}", ResponseCode::OK);
        response.addHeader("Content-Type", "application/json");
        return response;
    }
};


int main() {
    auto server = make_unique<Httpd>();
    auto handler = make_unique<Handler>();

    auto rh = std::bind(&Handler::handleRoot, handler.get(), std::placeholders::_1);
    server->addHandler(RequestType::GET, "/", rh);

    rh = std::bind( &Handler::handleAddPage, handler.get(), std::placeholders::_1);
    server->addHandler(RequestType::POST, "/page", rh);

    thread t([&] () {
        server->listenOnPort(kPort);
        while(running) {
            usleep(1000 * 1000);
        }
    });

    t.join();
    return 0;
}