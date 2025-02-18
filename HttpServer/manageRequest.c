#include "./Route/Global.h"

#define CACHE_SIZE 500


Method *methods = NULL;
Cache *cache[CACHE_SIZE];

Method *setUpRoute() {
    struct SetRoute *export = malloc(sizeof(SetRoute));
    export = getRoute(export);
    int notFound = 1;
    Method *firstMethods = methods;

    for (int i = 0; i < export->numberFunction; i++) {
        while (methods != NULL) {
            if  (strcmp(methods->method, export->routes[i].method) == 0) {
                struct Path *newPath = malloc(sizeof(Path));
                newPath->path = export->routes[i].path;
                newPath->fun_ptr = export->routes[i].fun_ptr;
                newPath->nextPath = methods->path;
                methods->path = newPath;
                notFound = 0;
            }
            methods = methods->nextMethod;
        }
        methods = firstMethods;
        if (notFound) {
            struct Method *newMethod = malloc(sizeof(Method));
            struct Path *newPath = malloc(sizeof(Path));
            newPath->path = export->routes[i].path;
            newPath->fun_ptr = export->routes[i].fun_ptr;
            newPath->nextPath = NULL;
            newMethod->method = export->routes[i].method;
            newMethod->path = newPath;
            newMethod->nextMethod = firstMethods;
            methods = newMethod;
            firstMethods = methods;
        }
        notFound = 1;
    }

    return firstMethods;
}


char *createHTTPResponse(Request *request){
    if (strcmp(request->method, "HEAD") == 0) {
        char *response = concat("HTTP/1.1 ", request->HTTPcode);
        if (request->contentType) {
            response = concat(response, "\r\nContent-Type: ");
            response = concat(response, request->contentType);
        }
        response = concat(response, "\r\nConnection: close\r\n\r\n");
        return response;
    } else {
        char arr[10] = "";
        char *response = concat("HTTP/1.1 ", request->HTTPcode);
        response = concat(response, "\r\nContent-Length: ");
        sprintf(arr, "%lu", strlen(request->bodyResponse));
        response = concat(response, arr);
        if (request->contentType) {
            response = concat(response, "\r\nContent-Type: ");
            response = concat(response, request->contentType);
        }
        response = concat(response, "\r\nConnection: close\r\n\r\n");
        response = concat(response, request->bodyResponse);
        return response;
    }
}

void responseFile(Request *request, char *fileContent, int size) {
    char *message = "HTTP/1.1 200 OK\r\nContent-Type: ";
    message = concat(message, request->contentType);
    printf("%s\n", request->contentType);
    message = concat(message, "\r\nContent-Length: ");
    char buffSize[100];
    sprintf(buffSize, "%d\r\n", size);
    message = concat(message, buffSize);
    message = concat(message, "\r\n");
    send(request->socket, message, strlen(message), 0);
    send(request->socket, fileContent, size, 0);
}

void sendFile(Request *request, char *path) {
    FILE * picture;
    picture = fopen(path, "r");
    fseek( picture, 0, SEEK_END);
    int size = ftell( picture);
    rewind( picture);
    char *fileContent = malloc(sizeof(char) * size);
    fread(fileContent, 1, size, picture);
    responseFile(request, fileContent, size);
}

Request *getFile(Request *request) {
    char *path = concat("./public", request->path);
    if (access(path, F_OK) == 0) {
        request = setContentType(request, path);
        sendFile(request, path);
        return request;
    }
    return request;
}

void saveInCache(Request *request) {
    if (strcmp("HEAD", request->method) == 0 || strcmp("GET", request->method) == 0) {
        for (int i = 0; i < CACHE_SIZE; i++) {
            if (cache[i] == NULL) {
                cache[i] = malloc(sizeof(Cache));
                cache[i]->path = request->path;
                cache[i]->method = request->method;
                cache[i]->bodyResponse = request->bodyResponse;
                cache[i]->HTTPCode = request->HTTPcode;
                cache[i]->contentType = request->contentType;
                cache[i]->timestamp = time(NULL);
                printf("save in Cache %s %s at index : %i, timestamp : %lu\n", request->method, request->path,i, cache[i]->timestamp);
                break;
            }
        }
    }
}

Request *executeRoute(Request *request) {
    Method *methodsLoop = methods;
    while (methodsLoop != NULL) {
        if (strcmp(methodsLoop->method, request->method) == 0) {
            Path *path =  methodsLoop->path;
            while (path != NULL) {
                if (strcmp(path->path, request->path) == 0) {

                    request = path->fun_ptr(request);
                    saveInCache(request);
                    return request;
                }
                path = path->nextPath;
            }
        }
        methodsLoop = methodsLoop->nextMethod;
    }
    return request;
}

void response(Request *request) {
    char *response = createHTTPResponse(request);
    char array[strlen(response)];
    strcpy(array, response);
    send(request->socket, &array, strlen(array), 0);
}

int getCache(Request *request) {
    int notFound = 1;
    if (strcmp("HEAD", request->method) == 0 || strcmp("GET", request->method) == 0) {
        for (int i = 0; i < CACHE_SIZE; i++) {
            if (cache[i] == NULL) {
                break;
            } else if(strcmp(cache[i]->path, request->path) == 0 && strcmp(cache[i]->method, request->method) == 0) {
                if(cache[i]->timestamp >= (long unsigned int)time(NULL) - 60) {
                    printf("use Cache for %s %s\n", request->method, request->path);
                    printf("from %lu \n", cache[i]->timestamp);
                    request->bodyResponse = cache[i]->bodyResponse;
                    request->HTTPcode = cache[i]->HTTPCode;
                    request->contentType = cache[i]->contentType;
                    notFound = 0;
                } else {
                    free(cache[i]);
                    cache[i] = NULL;
                }
                break;
            }
        }
    }
    return notFound;
}

void callRoute(Request *request) {

    printf("%s\n", request->path);
    int notFound = 1;
    if (getCache(request))
        request = executeRoute(request);
    if (request->HTTPcode) {
        notFound = 0;
        response(request);
    }
    if (notFound) {
        request = getFile(request);
        if (request->contentType)
            notFound = 0;
    }
    if (notFound) {
        request = setHttpCode(request, 404);
        request = setResponseBody(request, "Not Found");
        response(request);
    }
    close(request->socket);
    request->contentType = NULL;
    request->HTTPcode = NULL;
    request->path = NULL;
    request->method = NULL;
    free(request);
}


void setMethodPath(Request *request, char *line) {
    char *parsedLine = strtok(line, " ");
    request->method = parsedLine;
    parsedLine = strtok(NULL, " ");
    request->path = parsedLine;
}

Header *insertHeader(Header *headers, char *headerLine) {
    Header *newHeader = malloc(sizeof(Header));
    char *parsedHeader =  strtok(headerLine, " ");
    newHeader->name = parsedHeader;
    parsedHeader =  strtok(NULL, " ");
    newHeader->value = parsedHeader;
    newHeader->nextHeader = headers;
    headers = newHeader;
    return headers;
}

void getHeaders(Request *request, char *requestStr) {
    char **test = malloc(sizeof(char *));
    test = &requestStr;
    char *ptr =  __strtok_r(requestStr, "\n", test);
    Header *headers = malloc(sizeof(Header));
    headers = NULL;
    while(strlen(ptr) > 1)
    {
        if (strstr(ptr, ":")) {
            char *headerLine = malloc(sizeof(char) * (strlen(ptr) + 1));
            strcpy(headerLine, ptr);
            headers = insertHeader(headers, headerLine);
        }
        if (strstr(ptr, "\n")) {
            ptr = __strtok_r(NULL, "\n", test);
        }
        if (strstr(ptr, "\0")) {
            ptr = __strtok_r(NULL, "\0", test);
            ptr="\0";
        }
    }
    request->Header = headers;
    ptr = __strtok_r(NULL, "\n", test);
    request->body = ptr;
}

void createRequest(char *requestStr, int socket) {
    Request *request = malloc(sizeof(Request));
    char *cpyRequest = malloc(sizeof(char) * (strlen(requestStr) + 1));
    strcpy(cpyRequest, requestStr);
    setMethodPath(request,  strtok(cpyRequest, "\n"));
    getHeaders(request, requestStr);
    Header *headers = request->Header;
    while (headers != NULL) {
        headers = headers->nextHeader;
    }
    request->socket = socket;
    callRoute(request);
}

void *parseRequest(void *recvData) {
    RecvData *request = (RecvData *)recvData;
    createRequest(request->requestStr, request->socket);
    epoll_ctl(request->fileDescriptor, EPOLL_CTL_DEL, request->event.data.fd, NULL);
    return NULL;
}

void acceptNewRequest(int sock, int fileDescriptor) {
    socklen_t client_addr_len;
    struct sockaddr_in client_addr;
    struct epoll_event event;

    int newRequest = accept(sock, (struct sockaddr *) &client_addr, &client_addr_len);
    if (newRequest < 0) {
        perror("accept");
    }
    event.data.fd = newRequest;
    event.events = EPOLLIN;
    if (epoll_ctl(fileDescriptor, EPOLL_CTL_ADD, newRequest, &event) == -1) {
        perror("epoll_ctl");
    }
}

void executeRequest(struct epoll_event event, int fileDescriptor) {
    pthread_t requestThread;
    char *buf = malloc(65535);
    int newRequest = event.data.fd;
    int bytes_read = read(newRequest, buf, 65535);
    if (bytes_read <= 0) {
        epoll_ctl(fileDescriptor, EPOLL_CTL_DEL, newRequest, NULL);
        close(newRequest);
    } else {
        RecvData *recvData = malloc(sizeof(RecvData));
        recvData->requestStr = buf;
        recvData->socket = newRequest;
        recvData->fileDescriptor = fileDescriptor;
        recvData->event = event;
        if(pthread_create(&requestThread, NULL, parseRequest, (void *) recvData)) {
            close(newRequest);
        }
    }
}


void runningLoop(int sock, int fileDescriptor) {
    struct epoll_event eventList[5000];
    int event_count;
    while (1) {
        event_count = epoll_wait(fileDescriptor, eventList, 5000, -1);
        if (event_count == -1) {
            perror("epoll_wait");
            printf("Could not epoll_wait.\n");
        }
        for (int i = 0; i < event_count; i++) {
            if (eventList[i].data.fd == sock) {
                acceptNewRequest(sock, fileDescriptor);
            } else {
                executeRequest(eventList[i], fileDescriptor);
            }
        }
    }
}
