//
// Created by William Kamp on 7/4/16.
//

#include <string>
#include <unordered_map>
#include "Request.h"

Request::Request() { }

Request::Request(const Headers &headers,
        const Parameters &params) : headers(headers), params(params) { }

Request::Request(Request &&other) : body() {
    std::swap(other.body, body);
    std::swap(other.headers, headers);
    std::swap(other.params, params);
}

void Request::appendBodyData(char const *upload_data, size_t *upload_data_size) {
    body.append(upload_data, *upload_data_size);
}

std::string Request::findRequestParameter(std::string key) {
    auto iter = params.find(key);
    if (iter != params.end()) {
        return iter->second;
    }
    return "";
};

const std::string &Request::getBody() const {
    return body;
}

const Headers &Request::getHeaders() const {
    return headers;
}

const Parameters &Request::getParams() const {
    return params;
}