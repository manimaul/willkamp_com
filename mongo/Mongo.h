//
// Created by William Kamp on 7/6/16.
//

#ifndef WILLKAMP_MONGO_H
#define WILLKAMP_MONGO_H


#include <mongoc.h>
#include <string>
#include <iostream>
#include <vector>

class Bson {
    bson_t *bson;
    bson_error_t error;

public:

    Bson() {
        bson = bson_new();
    }

    Bson(const std::string &json) {
        bson = bson_new_from_json((const uint8_t *) json.c_str(), json.size(), &error);
    }

    Bson(Bson &other) = delete;

    Bson(Bson &&other) : bson(nullptr), error(other.error) {
        error = other.error;
        std::swap(other.bson, bson);
    }

    const bson_t &getBson() const {
        return *bson;
    }

    const bson_error_t &getError() const {
        return error;
    }

    std::string getJson() {
        char * valueStr = bson_as_json(bson, NULL);
        std::string val = std::string(valueStr);
        bson_free(valueStr);
        return val;
    }

    virtual ~Bson() {
        bson_free(bson);
    }

};

class MongoCollection {
    mongoc_collection_t *coll;


public:
    MongoCollection(mongoc_client_t *client, std::string &database, std::string &collection) {
        coll = mongoc_client_get_collection(client, database.c_str(), collection.c_str());
    }

    MongoCollection(MongoCollection &other) = delete; // prevent copy

    MongoCollection(MongoCollection &&other) : coll(nullptr) {
        coll = other.coll;
        other.coll = nullptr;
    }

    virtual ~MongoCollection() {
        mongoc_collection_destroy(coll);
    }

    void deleteRecord(std::string &query) {
        Bson bsonQuery(query);
        bson_error_t error;
        mongoc_collection_remove(coll, MONGOC_REMOVE_SINGLE_REMOVE, &bsonQuery.getBson(), NULL, &error);
        std::cout << error.message << std::endl;
    }

    std::string findOneRecord(std::string &query) {
        mongoc_cursor_t *cursor;
        bson_t const *doc;
        Bson bsonQuery(query);
        std::string value;

        cursor = mongoc_collection_find(coll, MONGOC_QUERY_NONE, 0, 1, 0, &bsonQuery.getBson(), NULL, NULL);
        if (mongoc_cursor_next(cursor, &doc)) {
            char * valueStr = bson_as_json(doc, NULL);
            value = valueStr;
            bson_free(valueStr);
        }

        mongoc_cursor_destroy(cursor);
        return value;
    }

    std::vector<std::string> findRecords(std::string &query, uint32_t skip, uint32_t limit, uint32_t batch_size) {
        mongoc_cursor_t *cursor;
        bson_t const *doc;
        Bson bsonQuery(query);

        cursor = mongoc_collection_find(coll, MONGOC_QUERY_NONE, skip, limit, batch_size, &bsonQuery.getBson(), NULL, NULL);
        std::vector<std::string> value;
        while (mongoc_cursor_next(cursor, &doc)) {
            char * valueStr = bson_as_json(doc, NULL);
            value.push_back(valueStr);
            bson_free(valueStr);
        }
        mongoc_cursor_destroy(cursor);

        return value;
    }

    bool upsertRecord(const std::string &query) {
        bson_error_t error;
        Bson record(query);
        return mongoc_collection_insert(coll, MONGOC_INSERT_NONE, &record.getBson(), NULL, &error);
    }
};

class MongoClient {
    mongoc_client_t *client;
    mongoc_client_pool_t *pool;

public:
    MongoClient(mongoc_client_pool_t *pool) : pool(pool) {
        client = mongoc_client_pool_pop(pool);
    }

    MongoClient(MongoClient &other) = delete;

    MongoClient(MongoClient &&other) : client(nullptr), pool(nullptr) {
        std::swap(other.client, client);
        std::swap(other.pool, pool);
    }

    virtual ~MongoClient() {
        mongoc_client_pool_push(pool, client);
    }

public:
    mongoc_client_t *getClient() const {
        return client;
    }
};

class Mongo {

    mongoc_client_pool_t *pool;
    mongoc_uri_t *uri;

public:

    Mongo(const std::string &dbUrl) {
        mongoc_init();
        uri = mongoc_uri_new(dbUrl.c_str());
        pool = mongoc_client_pool_new(uri);
    }

    Mongo(Mongo &other) = delete;

    Mongo(Mongo &&other) : pool(nullptr), uri(nullptr) {
        std::swap(other.pool, pool);
        std::swap(other.uri, uri);
    }

    virtual ~Mongo() {
        mongoc_client_pool_destroy(pool);
        mongoc_uri_destroy(uri);
        mongoc_cleanup();
    }

    MongoCollection getCollection(std::string database, std::string collection) {
        MongoClient c = MongoClient(pool);
        return MongoCollection(c.getClient(), database, collection);
    }

};


#endif //WILLKAMP_MONGO_H
