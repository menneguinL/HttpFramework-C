#include "./manageRequest.h"
#include <sys/epoll.h>

int main() {

    Configuration *conf = malloc(sizeof(Configuration));
/*
    int sock = initSocket();
    struct sockaddr_in server = setupServer();
    int option = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    setUpRoute();

    if (sock == 0 || bindWrapper(sock, server, sizeof(server)) == 0)
        return 0;

    listen(sock, 5000);

    int file_descriptor = initEpoll(sock);

    runningLoop(sock, file_descriptor);

    close(sock);
*/

    parseIni(conf);
    printf("api : %i \n", conf->api);
    printf("kpi : %i \n", conf->kpi);
    return 0;
}
