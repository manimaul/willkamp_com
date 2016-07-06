//
// Created by William Kamp on 7/4/16.
//

#ifndef WILLKAMP_RESPONSE_H
#define WILLKAMP_RESPONSE_H


#include <string>
#include "ResponseCodes.h"

class Response {

private:
    std::unordered_map<std::string, std::string> _headers;

public:
    std::string body;
    ResponseCode code;

    Response(const std::string &body, ResponseCode code) : body(body), code(code) {
        _headers = std::unordered_map<std::string, std::string>();
    }

    void addHeader(const std::string &key, const std::string &value) {
        _headers.insert({key, value});
    }

    const std::unordered_map<std::string, std::string> &get_headers() const {
        return _headers;
    }
};


#endif //WILLKAMP_RESPONSE_H
