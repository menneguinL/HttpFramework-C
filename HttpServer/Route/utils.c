//
// Created by loicm on 20/01/2023.
//
#include "Global.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct  Request *setHttpCode(struct  Request *request, int httpCode) {
    switch (httpCode) {
        case 200:
            request->HTTPcode = "200 OK";
            break;
        case 404:
            request->HTTPcode = "404 Not Found";
            break;
        case 201:
            request->HTTPcode = "201 Created";
            break;
        default:
            break;
    }
    return request;
}


struct  Request *setResponseBody(struct  Request *request, char *body) {
    request->bodyResponse = malloc(9999999999);
    strcpy(request->bodyResponse, body);
    return request;
}

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

struct Request *setContentType(struct  Request *request, char *path) {
    char *dot = strrchr(path, '.');
    dot = dot+1;
    char *type = "text/plain";
    if(strcmp(dot, "json") == 0)
        type = "application/json";
    else if(strcmp(dot, "png") == 0)
        type = "image/png";
    else if(strcmp(dot, "jpg") == 0)
        type = "image/jpeg";
    else if(strcmp(dot, "html") == 0)
        type = "text/html";
    else if(strcmp(dot, "mp4") == 0)
        type = "video/mp4";
    request->contentType = type;
    return request;
}

struct Request *setContentTypeFunction(struct  Request *request, char *dot) {
    char *type = "text/plain";
    if(strcmp(dot, "json") == 0)
        type = "application/json";
    else if(strcmp(dot, "png") == 0)
        type = "image/png";
    else if(strcmp(dot, "jpg") == 0)
        type = "image/jpeg";
    else if(strcmp(dot, "html") == 0)
        type = "text/html";
    request->contentType = type;
    return request;
}

char *concat(const char *a, const char *b){
    int sizeA = strlen(a);
    int sizeB = strlen(b);
    char *newStr = malloc(sizeA+sizeB+1);
    memcpy(newStr,a,sizeA);
    memcpy(newStr+sizeA,b,sizeB+1);
    return newStr;
}