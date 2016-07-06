#include <iostream>
#include <thread>
#include <unistd.h>
#include "httpd/Httpd.h"

static const int kPort = 8080;
bool running = true;

using namespace std;

unique_ptr<Response> handleRoot(unique_ptr<Request> ignored) {
    auto response = make_unique<Response>("{}", OK);
    response->addHeader("Content-Type", "application/json");
    return response;
}

int main() {
    unique_ptr<Httpd> server(new Httpd);
    server->addHandler(GET, "/", &handleRoot);

    thread t([&] () {
        server->listenOnPort(kPort);
        while(running) {
            usleep(1000 * 1000);
        }
    });

    t.join();
    return 0;
}