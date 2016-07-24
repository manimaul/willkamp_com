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

public:

    Response(const std::string &body, ResponseCode code);

    Response(Response &other) = delete; //prevent copy

    Response(Response &&other);

    void addHeader(const std::string &key, const std::string &value);

    void setContentTypeJson();

    void setContentTypeHtml();

    const std::unordered_map<std::string, std::string> &get_headers() const;

    const std::string &getBody() const;

    size_t bodySize() const;

    ResponseCode getCode() const;

    static void setDefaultContentType(std::string type);

};

#endif //WILLKAMP_RESPONSE_H
