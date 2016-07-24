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

    Request();

    Request(const Headers &headers,
            const Parameters &params);

    Request(Request &other) = delete;

    Request(Request &&other);

    void appendBodyData(char const *upload_data, size_t *upload_data_size);

    std::string findRequestParameter(std::string key);

    const std::string &getBody() const;

    const Headers &getHeaders() const;

    const Parameters &getParams() const;
};


#endif //WILLKAMP_REQUEST_H
