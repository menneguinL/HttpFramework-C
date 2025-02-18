//
// Created by loicm on 17/11/2022.
//

#include "../Global.h"

struct Request *helloPatate(struct Request *req) {
    req = setHttpCode(req, 200);
    req = setResponseBody(req, "Hello, world!");
    return req;
}

struct Request *createPatate(struct Request *req) {
    req = setHttpCode(req, 201);
    req = setContentTypeFunction(req, "json");
    char *response = concat("{\"data\" : \"Patates created !!!!\", \"post\" : \"", req->body);
    response = concat(response, "\"}");
    req = setResponseBody(req, response);
    return req;
}