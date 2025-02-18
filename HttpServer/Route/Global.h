//
// Created by loicm on 17/11/2022.
//

#ifndef HTTPSERVER_GLOBAL_H
#define HTTPSERVER_GLOBAL_H

#endif //HTTPSERVER_GLOBAL_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <string.h>

typedef struct RouteConfig {
    char *method;
    char *path;
    struct Request *(*fun_ptr)(struct Request *);
} RouteConfig;

typedef struct SetRoute {
    int numberFunction;
    struct RouteConfig *routes;
} SetRoute;

typedef struct Header {
    char *name;
    char *value;
    struct Header *nextHeader;
} Header;

typedef struct Cache {
    char *path;
    char *method;
    char *HTTPCode;
    char *contentType;
    char *bodyResponse;
    unsigned long timestamp;
} Cache;

typedef struct Request {
    char *method;
    char *path;
    char *body;
    int socket;
    char *bodyResponse;
    char *HTTPcode;
    char *contentType;
    struct Header *Header;
} Request;

typedef struct Path {
    char *path;
    struct Request *(*fun_ptr)(struct Request *);
    struct Path *nextPath;
} Path;

typedef struct Method {
    char *method;
    struct Path *path;
    struct Method *nextMethod;
} Method;

typedef struct RecvData {
    char *requestStr;
    int socket;
    int fileDescriptor;
    struct epoll_event event;
} RecvData;

struct SetRoute *getRoute(struct SetRoute *export);
struct  Request *setHttpCode(struct  Request *request,  int httpCode);
struct  Request *setResponseBody(struct  Request *request, char *body);
struct  Request *setContentType(struct  Request *request, char *type);
char *concat(const char *a, const char *b);
struct Request *setContentTypeFunction(struct  Request *request, char *dot);


