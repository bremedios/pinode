//
// Created by Bradley Remedios on 11/28/24.
//
#include "pinode/Server.h"

#include <iostream>

#include <bpl/sys/Tick.h>

#include <pinode/HeaterControlFactory.h>
#include <pinode/Client.h>
#include <pinode/Server.h>
#include <pinode/HeaterController.h>

void Usage_() {
    std::cerr << "Usage:" << std::endl;
} // Usage_

int main (int argc, char** argv) {
    std::cout << "Pi Heater v0.01" << std::endl;

    auto controlPtr = pinode::HeaterControlFactory::getHeaterControl();

    if (argc != 1) {
        std::string cmd (argv[1]);

        if (cmd == "on") {
            controlPtr->On();
            std::cout <<  "Turning on heater" << std::endl;
            return 0;
        } else if (cmd == "off") {
            controlPtr->Off();
            std::cout <<  "Turning off heater" << std::endl;
            return 0;
        }
        else {
            Usage_();

            return -1;
        }
    }

    bpl::sys::Tick              tick(std::chrono::milliseconds(1000));
    pinode::HeaterController    controller;
    pinode::ClientPtr           client = std::make_shared<pinode::Client>();
    pinode::ServerPtr server = std::make_shared<pinode::Server>();

    if (!client->Connect("192.168.1.215", 9999))
    {
        std::cerr << "    ERROR: Failed to connect to pinode temperature sensor node" << std::endl;
        return -1;
    }

    if (!client->Start()) {
        std::cerr << "    ERROR: Failed to start to pinode sensor client" << std::endl;
        return -1;
    }

    client->EnableSensor(true);

    std::list<std::filesystem::path> paths;

    paths.emplace_back("pinode-heater.json");
    paths.emplace_back("/etc/pinode/heater.json");

    if (!server->EnableHeater()) {
        std::cout << "    ERROR: Failed to enable heater component in server" << std::endl;

        return -1;
    }

    if (!server->Start(paths)) {
        std::cout << "    ERROR: Failed to start server" << std::endl;

        return -1;
    }

    controller.setHeaterControl(controlPtr);
    controller.setPinodeClient(client);
    controller.setHeaterStatus(server->getHeaterStatus());
    controller.setThreshold(0.5f);
    controller.setTemperature(20.5f);

    while (true) {
        if(!controller.Update()) {
            std::cerr << "    ERROR: Failed to update heater control" << std::endl;
        }

        tick.Wait();
    }

    return 0;
} // main