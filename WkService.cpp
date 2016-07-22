//
// Created by William Kamp on 7/19/16.
//

#include "WkService.h"

using namespace std;

/**
 * Request handler for GET "/"
 */
Response WkService::handleRoot(unique_ptr<Request> request) {
    printRequestHeadersAndParams(*request);
    request->getBody();
    Bson bson;
    bson.add("message", "ok");
    auto response = Response(bson.getJson(), ResponseCode::OK);
    return response;
}

/**
 * Request handler for POST "/page"
 */
Response WkService::handleAddPage(unique_ptr<Request> request) {
    printRequestHeadersAndParams(*request);
    MongoCollection collection = mongo.getCollection(kDb, kDbCollectionPages);
    collection.upsertRecord(request->getBody());
    Bson bson;
    bson.add("message", "ok");
    auto response = Response(bson.getJson(), ResponseCode::OK);
    return response;
}

/**
 * Request handler for GET "/page"
 */
Response WkService::handleGetPage(unique_ptr<Request> request) {
    printRequestHeadersAndParams(*request);
    auto oid = request->findRequestParameter("oid");
    if (!oid.empty()) {
        MongoCollection collection = mongo.getCollection(kDb, kDbCollectionPages);
        Bson bson;
        bson.addOid(oid);
        string record = collection.findOneRecord(bson);
        if (!record.empty()) {
            auto response = Response(record, ResponseCode::OK);
            return response;
        }
    }
    return notFound();
}

Response WkService::notFound() {
    Bson respBody;
    respBody.add("message", "not found");
    auto response = Response(respBody.getJson(), ResponseCode::NOT_FOUND);
    return response;
}

void WkService::printRequestHeadersAndParams(Request &request) {
    cout << "handleGetPage" << endl;
    for (auto p : request.getHeaders()) {
        cout << "Header: " << p.first << " : " << p.second << endl;
    }
    for (auto p : request.getParams()) {
        cout << "Param: " << p.first << " : " << p.second << endl;
    }
}


