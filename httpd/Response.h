//
// Created by William Kamp on 7/4/16.
//

#ifndef WILLKAMP_RESPONSE_H
#define WILLKAMP_RESPONSE_H


#include <string>
#include "ResponseCodes.h"

static const std::string kContentType = "Content-Type";
static const std::string kContentTypeJson = "application/json";
static const std::string kContentTypeHtml = "text/html";

class Response {

private:

    std::unordered_map<std::string, std::string> _headers;
    std::string body;
    ResponseCode code;
    static std::string defaultContentType;

public:

    Response(const std::string &body, ResponseCode code) : body(body), code(code) {
        if (!defaultContentType.empty()) {
            setDefaultContentType(defaultContentType);
        }
    }

    Response(Response &other) = delete; //prevent copy

    Response(Response &&other) : _headers(), body(), code(ResponseCode::NOT_FOUND) {
        std::swap(other._headers, _headers);
        std::swap(other.body, body);
        std::swap(other.code, code);
    }

    void addHeader(const std::string &key, const std::string &value) {
        _headers.insert({key, value});
    }

    void setContentTypeJson() {
        _headers.emplace(kContentType, kContentTypeJson);
    }

    void setContentTypeHtml() {
        _headers.emplace(kContentType, kContentTypeHtml);
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

    static void setDefaultContentType(std::string type) {
        defaultContentType = type;
    }

};


#endif //WILLKAMP_RESPONSE_H
