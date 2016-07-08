//
// Created by William Kamp on 7/4/16.
//

#ifndef WILLKAMP_REQUEST_H
#define WILLKAMP_REQUEST_H


#include <string>
#include <unordered_map>

class Request {

private:

    std::string body;
    //std::unordered_map<std::string, std::string> headers;

public:

    Request() { }

    Request(Request &other) = delete;

    Request(Request &&other) : body() {
        std::swap(other.body, body);
    }

    void appendBodyData(char const *upload_data, size_t *upload_data_size) {
        body.append(upload_data, *upload_data_size);
    }

    const std::string &getBody() const {
        return body;
    }
};


#endif //WILLKAMP_REQUEST_H
