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
#include <string.h>
#include <sys/stat.h>
#include "./Route/Global.h"
#include "./iniParser/iniParser.h"

int initSocket();
struct sockaddr_in setupServer();
Method *setUpRoute();
int bindWrapper(int sock, struct sockaddr_in server, int sizeofServer);
void runningLoop(int sock, int fileDescriptor);
int initEpoll(int sock);