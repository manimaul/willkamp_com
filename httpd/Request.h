//
// Created by William Kamp on 7/4/16.
//

#ifndef WILLKAMP_REQUEST_H
#define WILLKAMP_REQUEST_H


#include <string>
#include <unordered_map>

class Request {

public:
    typedef std::unordered_map<std::string, std::string> Headers;
    typedef std::unordered_map<std::string, std::string> Parameters;

private:

    std::string body;
    Headers headers;
    Parameters params;

public:

    Request() { }

    Request(const Headers &headers,
            const Parameters &params) : headers(headers), params(params) { }

    Request(Request &other) = delete;

    Request(Request &&other) : body() {
        std::swap(other.body, body);
        std::swap(other.headers, headers);
        std::swap(other.params, params);
    }

    void appendBodyData(char const *upload_data, size_t *upload_data_size) {
        body.append(upload_data, *upload_data_size);
    }

    std::string findRequestParameter(std::string key) {
        auto iter = params.find(key);
        if (iter != params.end()) {
            return iter->second;
        }
        return "";
    };

    const std::string &getBody() const {
        return body;
    }

    const Headers &getHeaders() const {
        return headers;
    }

    const Parameters &getParams() const {
        return params;
    }
};


#endif //WILLKAMP_REQUEST_H
