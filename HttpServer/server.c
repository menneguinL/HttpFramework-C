//
// Created by loicm on 16/02/2023.
//

#include "./Route/Global.h"
#define PORT 8080

struct sockaddr_in setupServer() {
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    return server;
}

int initSocket() {
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        perror("socket");
        printf("Could not create socket.\n");
        close(sock);
        return 0;
    }

    return sock;
}

int initEpoll(int sock) {
    struct epoll_event event;
    int file_descriptor = epoll_create1(0);
    event.data.fd = sock;
    event.events = EPOLLIN;

    epoll_ctl(file_descriptor, EPOLL_CTL_ADD, sock, &event);
    return file_descriptor;
}

int bindWrapper(int sock, struct sockaddr_in server, int sizeofServer) {
    if (bind(sock, (struct sockaddr *) &server, sizeofServer) < 0) {
        perror("bind");
        printf("Error while binding.\n");
        return 0;
    }

    return 1;
}