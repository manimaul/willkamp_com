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

    unique_ptr<Response> handleRoot(unique_ptr<Request> ignored) {
        auto response = make_unique<Response>("{}", ResponseCode::OK);
        response->addHeader("Content-Type", "application/json");
        return response;
    }

    unique_ptr<Response> handleAddPage(unique_ptr<Request> request) {
        MongoCollection collection = mongo.getCollection(DB_WK, DB_COL_PAGES);
        collection.upsertRecord(request->body);
        auto response = make_unique<Response>("{}", ResponseCode::OK);
        response->addHeader("Content-Type", "application/json");
        return response;
    }
};


int main() {
    unique_ptr<Httpd> server(new Httpd);
    unique_ptr<Handler> handler(new Handler);

    Httpd::RequestHandler rh = std::bind( &Handler::handleRoot, handler.get(), std::placeholders::_1);
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