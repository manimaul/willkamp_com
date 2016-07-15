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
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> params;

public:

    Request() { }

    Request(const std::unordered_map<std::string, std::string> &headers,
            const std::unordered_map<std::string, std::string> &params) : headers(headers), params(params) { }

    Request(Request &other) = delete;

    Request(Request &&other) : body() {
        std::swap(other.body, body);
        std::swap(other.headers, headers);
        std::swap(other.params, params);
    }

    void appendBodyData(char const *upload_data, size_t *upload_data_size) {
        body.append(upload_data, *upload_data_size);
    }

    const std::string &getBody() const {
        return body;
    }

    const std::unordered_map<std::string, std::string> &getHeaders() const {
        return headers;
    }

    const std::unordered_map<std::string, std::string> &getParams() const {
        return params;
    }
};


#endif //WILLKAMP_REQUEST_H
