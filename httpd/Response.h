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
    std::string body;
    ResponseCode code;

public:

    Response(const std::string &body, ResponseCode code) : body(body), code(code) { }

    Response(Response &other) = delete; //prevent copy

    Response(Response &&other) : _headers(), body(), code(ResponseCode::NOT_FOUND) {
        std::swap(other._headers, _headers);
        std::swap(other.body, body);
        std::swap(other.code, code);
    }

    void addHeader(const std::string &key, const std::string &value) {
        _headers.insert({key, value});
    }

    const std::unordered_map<std::string, std::string> &get_headers() const {
        return _headers;
    }


    const std::string &getBody() const {
        return body;
    }

    size_t bodySize() const {
        return body.size();
    }

    ResponseCode getCode() const {
        return code;
    }
};


#endif //WILLKAMP_RESPONSE_H
