//
// Created by William Kamp on 7/4/16.
//

#include <string>
#include <unordered_map>
#include "ResponseCodes.h"
#include "Response.h"

std::string defaultContentType;

Response::Response(const std::string &body, ResponseCode code) : body(body), code(code) {
    if (!defaultContentType.empty()) {
        setDefaultContentType(defaultContentType);
    }
}

Response::Response(Response &&other) :

_headers(), body(), code(ResponseCode::NOT_FOUND) {
    std::swap(other._headers, _headers);
    std::swap(other.body, body);
    std::swap(other.code, code);
}

void Response::addHeader(const std::string &key, const std::string &value) {
    _headers.insert({key, value});
}

void Response::setContentTypeJson() {
    _headers.emplace(kContentType, kContentTypeJson);
}

void Response::setContentTypeHtml() {
    _headers.emplace(kContentType, kContentTypeHtml);
}

const std::unordered_map<std::string, std::string> &Response::get_headers() const {
    return _headers;
}

const std::string &Response::getBody() const {
    return body;
}

size_t Response::bodySize() const {
    return body.size();
}

ResponseCode Response::getCode() const {
    return code;
}

void Response::setDefaultContentType(std::string type) {
    defaultContentType = type;
}


