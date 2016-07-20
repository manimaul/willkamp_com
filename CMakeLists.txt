cmake_minimum_required(VERSION 3.5)
project(willkamp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include(FindBson.cmake)
include(FindMongoC.cmake)
include_directories(${BSON_INCLUDE_DIR})
include_directories(${MONGOC_INCLUDE_DIR})

set(LIBS ${LIBS} ${BSON_LIBRARIES} ${MONGOC_LIBRARIES})

set(SOURCE_FILES main.cpp
        httpd/Httpd.cpp
        httpd/Httpd.h
        httpd/Request.h
        httpd/Response.h
        httpd/ResponseCodes.h
        httpd/RequestTypes.h
        mongo/Mongo.h
        WkService.cpp
        WkService.h)

add_executable(willkamp ${SOURCE_FILES})

target_link_libraries(willkamp microhttpd mongoc-1.0 ${LIBS})