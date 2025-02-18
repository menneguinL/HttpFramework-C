//
// Created by loicm on 16/02/2023.
//

#include "../Global.h"

struct Request *putCheese(struct Request *req) {
    req = setHttpCode(req, 200);
    req = setResponseBody(req, "I like raclette");
    return req;
}
