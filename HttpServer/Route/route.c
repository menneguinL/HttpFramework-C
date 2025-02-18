//
// Created by Randy Deret on 25/11/2022.
//

#include "./Header/patate.h"
#include "./Header/raclette.h"
#include "./Global.h"
#include <stdlib.h>

struct RouteConfig Route[] = {
        {"PUT",  "/cheese", &putCheese},
        {"GET", "/patates", &helloPatate},
        {"POST", "/patates", &createPatate},
        {"POST", "/newPatates", &createPatate},
        {"HEAD", "/patates", &helloPatate}
};

struct SetRoute *getRoute(struct SetRoute *export) {
    int numberFunction = sizeof(Route) / sizeof(*Route);
    struct RouteConfig *test = malloc(sizeof(RouteConfig)*numberFunction);

    for (int i = 0; i < numberFunction; ++i) {
        test[i] = Route[i];
    }

    export->routes = test;
    export->numberFunction = numberFunction;
    return export;
}