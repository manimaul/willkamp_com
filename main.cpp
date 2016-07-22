#include <iostream>
#include <thread>
#include <unistd.h>
#include "httpd/Httpd.h"
#include "mongo/Mongo.h"
#include "WkService.h"

using namespace std;


static bool running = true;

int main() {
    Response::setDefaultContentType(kContentTypeJson);

    auto server = make_unique<Httpd>();
    auto handler = make_unique<WkService>();

    auto rh = bind(&WkService::handleRoot, handler.get(), placeholders::_1);
    server->addHandler(RequestType::GET, "/", rh);

    rh = bind( &WkService::handleAddPage, handler.get(), placeholders::_1);
    server->addHandler(RequestType::POST, kApiPublic + "/page", rh);

    rh = bind( &WkService::handleGetPage, handler.get(), placeholders::_1);
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