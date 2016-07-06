//
// Created by William Kamp on 7/4/16.
//

#ifndef WILLKAMP_REQUEST_H
#define WILLKAMP_REQUEST_H


#include <string>
#include <unordered_map>

class Request {


public:
    Request() {
        body = std::string();
    }

public:
    std::string body;
    std::unordered_map<std::string, std::string> headers;

    void setBody(const char* body) {
        if (body && strlen(body)) {
            Request::body = std::string(body);
        }
    }
};


#endif //WILLKAMP_REQUEST_H
