//
// Created by William Kamp on 7/19/16.
//

#include "WkService.h"

using namespace std;

/**
 * Request handler for GET "/"
 */
Response WkService::handleRoot(unique_ptr<Request> request) {
    cout << "handleRoot" << endl;
    for (auto p : request->getHeaders()) {
        cout << "Header: " << p.first << " : " << p.second << endl;
    }
    for (auto p : request->getParams()) {
        cout << "Param: " << p.first << " : " << p.second << endl;
    }
    request->getBody();
    Bson bson;
    bson.add("message", "ok");
    auto response = Response(bson.getJson(), ResponseCode::OK);
    response.addHeader("Content-Type", "application/json");
    return response;
}

/**
 * Request handler for POST "/page"
 */
Response WkService::handleAddPage(unique_ptr<Request> request) {
    cout << "handleAddPage" << endl;
    for (auto p : request->getHeaders()) {
        cout << "Header: " << p.first << " : " << p.second << endl;
    }
    for (auto p : request->getParams()) {
        cout << "Param: " << p.first << " : " << p.second << endl;
    }
    MongoCollection collection = mongo.getCollection(kDb, kDbCollectionPages);
    collection.upsertRecord(request->getBody());
    Bson bson;
    bson.add("message", "ok");
    auto response = Response(bson.getJson(), ResponseCode::OK);
    response.addHeader("Content-Type", "application/json");
    return response;
}

/**
 * Request handler for GET "/page"
 */
Response WkService::handleGetPage(unique_ptr<Request> request) {
    cout << "handleGetPage" << endl;
    for (auto p : request->getHeaders()) {
        cout << "Header: " << p.first << " : " << p.second << endl;
    }
    for (auto p : request->getParams()) {
        cout << "Param: " << p.first << " : " << p.second << endl;
    }
    MongoCollection collection = mongo.getCollection(kDb, kDbCollectionPages);

    string body = request->getBody();
    string respBody = collection.findOneRecord(body);
    auto response = Response(respBody, ResponseCode::NOT_FOUND);
    response.addHeader("Content-Type", "application/json");
    return response;
}

Response WkService::notFound(unique_ptr<Request> request) {
    Bson respBody;
    auto response = Response(respBody.getJson(), ResponseCode::NOT_FOUND);
    return response;
}


