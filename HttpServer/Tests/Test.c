//
// Created by Randy Deret on 08/12/2022.
//
#include <string.h>
#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "../manageRequest.h"

///Faudra test.

char *concat(const char *a, const char *b);
Method *setUpRoute();
char *createHTTPResponse(Request *request);
Request *executeRoute(Request *request);
void setMethodPath(Request *request, char *line);
Header *insertHeader(Header *headers, char *headerLine);
int initSocket();
struct sockaddr_in setupServer();
int initEpoll(int sock);

Test(sample, test){
    //arrange
    int sock;
    socklen_t client_addr_len;
    struct sockaddr_in server;
    pthread_t requestThread;
    sock = socket(AF_INET , SOCK_STREAM , 0);
    char *buf = malloc(65535);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons( 8080 );

    if (sock == -1)
    {
        printf("Could not create socket");
    }

    if(connect(sock,(struct sockaddr *) &server, sizeof(server)) < 0)
    {
        perror("connect()");
    }

    // act
    char message[] = "POST /patates HTTP/1.1\r\nConnection: Close\r\nContent-type: application/x-www-form-urlencoded\r\nContent-Length: 4\r\n\r\ntest";
    send(sock, &message, strlen(message), 0);
    int rcvd=recv(sock, buf, 65535, 0);
    if (rcvd<0)
        printf("recv() error\n");
    else if (rcvd==0)
        printf("Client disconnected upexpectedly.\n");
    close(sock);
    char **test = malloc(sizeof(char *));
    test = &buf;
    char *ptr =  strtok_r(buf, "\n", test);
    // assert
    cr_assert_not_null(strstr(ptr, "201"));
    while(strlen(ptr) > 1)
    {
        ptr =  strtok_r(NULL, "\n", test);
    }
    ptr =  strtok_r(NULL, "\n", test);
    cr_assert_not_null(strstr(ptr, "{\"data\" : \"Patates created !!!!\", \"post\" : \"test\"}"));
}

Test(sample, testNotFound){
    sleep(2);
    //arrange
    int sock;
    socklen_t client_addr_len;
    struct sockaddr_in server;
    pthread_t requestThread;
    sock = socket(AF_INET , SOCK_STREAM , 0);
    char *buf = malloc(65535);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons( 8080 );
    connect(sock,(struct sockaddr *) &server, sizeof(server));

    // act
    char message[] = "POST /patatessdsqsqdqsdsq HTTP/1.1\r\nConnection: Close\r\nContent-type: application/x-www-form-urlencoded\r\nContent-Length: 4\r\n\r\ntest";
    send(sock, &message, strlen(message), 0);
    int rcvd=recv(sock, buf, 65535, 0);
    if (rcvd<0)
    printf("recv() error\n");
    else if (rcvd==0)
    printf("Client disconnected upexpectedly.\n");
    close(sock);
    char **test = malloc(sizeof(char *));
    test = &buf;
    char *ptr =  strtok_r(buf, "\n", test);
    // assert
    cr_assert_not_null(strstr(ptr, "404"));
    while(strlen(ptr) > 1)
    {
    ptr =  strtok_r(NULL, "\n", test);
    }
    ptr =  strtok_r(NULL, "\n", test);
    cr_assert_not_null(strstr(ptr, "Not Found"));
}

Test(sample, testFile){
    sleep(2);
    //arrange
    int sock;
    socklen_t client_addr_len;
    struct sockaddr_in server;
    pthread_t requestThread;
    sock = socket(AF_INET , SOCK_STREAM , 0);
    char *buf = malloc(65535);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons( 8080 );
    connect(sock,(struct sockaddr *) &server, sizeof(server));

    // act
    char message[] = "GET /test.txt HTTP/1.1\r\nConnection: Close\r\nContent-type: application/x-www-form-urlencoded\r\n\r\n";
    send(sock, &message, strlen(message), 0);
    int rcvd=recv(sock, buf, 65535, 0);
    if (rcvd<0)
    printf("recv() error\n");
    else if (rcvd==0)
    printf("Client disconnected upexpectedly.\n");
    close(sock);
    char **test = malloc(sizeof(char *));
    test = &buf;
    char *ptr =  strtok_r(buf, "\n", test);
    // assert
    cr_assert_not_null(strstr(ptr, "200"));
    while(strlen(ptr) > 1)
    {
    ptr =  strtok_r(NULL, "\n", test);
    }
    ptr =  strtok_r(NULL, "\n", test);
    cr_assert_not_null(strstr(ptr, "aaaaaaaaaaaaaaaaaa"));
}

Test(sample, testSetUpRoute){
    //Arrange
    Method *methods = NULL;

    //Act
    methods = setUpRoute();

    //Assert
    cr_assert_not_null(methods);
}

Test(sample, testCreateHTTPResponse){
    //Arrange
    char *ANNE = NULL;
    Request *request = malloc(sizeof(request));
    request->HTTPcode = "200 OK";
    request->contentType = "text/plain";
    request->bodyResponse = "MARC !!!";
    request->method = "POST";

    //Act
    ANNE = createHTTPResponse(request);

    //Assert
    cr_assert_not_null(ANNE);
}

Test(sample, testExcuteRoute){
    //Arrange
    Request *request = malloc(sizeof(request));
    request->path = "/patates";
    request->method = "POST";

    //Act
    request = executeRoute(request);

    //Assert
    cr_assert_not_null(request);
}

Test(sample, testSetMethodPath){
    //Arrange
    char http[] = "POST /patates HTTP/1.1";
    Request *request = malloc(sizeof(request));

    setMethodPath(request, http);

    //Assert
    cr_assert(strcmp(request->method, "POST") == 0);
    cr_assert(strcmp(request->path, "/patates") == 0);
}

Test(sample, testInsertHeader){
    //Arrange
    char content[] = "Connection: keep-alive";
    Header *header = malloc(sizeof(Header));

    //Act
    header = insertHeader(header, content);
    //Assert
    cr_assert_not_null(header);
}

Test(sample, testConcat){
    char *bobby = concat("a","b");
    cr_assert(strcmp(bobby, "ab") == 0);
}

Test(sample, testSetupServer){
    struct sockaddr_in server = setupServer();
    cr_assert_not_null(&server);
}

Test(sample, testInitSocket){
    int sock = initSocket();
    cr_assert(sock > 0);
}

Test(sample, testInitEpoll){
    //arrange
    int sock;
    socklen_t client_addr_len;
    struct sockaddr_in server;
    pthread_t requestThread;
    sock = socket(AF_INET , SOCK_STREAM , 0);
    //Act
    int epoll = initEpoll(sock);

    //Assert
    cr_assert(epoll > 0);

}