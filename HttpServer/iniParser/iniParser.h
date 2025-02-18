#ifndef HTTPSERVER_INIPARSER_H
#define HTTPSERVER_INIPARSER_H

#endif //HTTPSERVER_INIPARSER_H

#include <stdlib.h>
#include <stdio.h>

typedef struct Configuration {
    int api;
    int kpi;
} Configuration;

int parseIni();