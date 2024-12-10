//
// Created by Bradley Rmedios on 11/25/24.
//
#include <iostream>

#include "pinode/Server.h"

int main(void) {
    std::cout << "PiNode Temperature Sensor" << std::endl;

    pinode::Server    server;

    if (!server.Start()) {
        std::cout << "    ERROR: Failed to start server" << std::endl;

        return -1;
    }

    if (!server.EnableSensor()) {
        std::cout << "    ERROR: Failed to enable sensor component in server" << std::endl;

        return -1;
    }

    server.Svc_();

    return 0;
} // main