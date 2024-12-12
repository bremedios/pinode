//
// Created by Bradley Rmedios on 11/25/24.
//
#include <iostream>

#include "pinode/Server.h"

int main(void) {
    std::cout << "PiNode Temperature Sensor" << std::endl;

    pinode::Server    server;


    std::list<std::filesystem::path> paths;

    paths.emplace_back("pinode-sensor.json");
    paths.emplace_back("/etc/pinode/sensor.json");

    if (!server.EnableSensor()) {
        std::cout << "    ERROR: Failed to enable sensor component in server" << std::endl;

        return -1;
    }

    if (!server.Start(paths)) {
        std::cout << "    ERROR: Failed to start server" << std::endl;

        return -1;
    }

    // everything is done on the internal server thread so we just wait for it to
    // terminate.
    server.WaitForTermination();

    return 0;
} // main