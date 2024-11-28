//
// Created by Bradley Remedios on 11/27/24.
//
#include <iostream>

#include <bpl/sys/Tick.h>
#include "pinode/Client.h"

std::string version = "v0.01";
std::string prog_name = "PiNode Client";

std::list<std::string> deviceIps;
std::map<std::string, pinode::ClientPtr> clientMap;

void Banner_() {
    std::cout << std::endl;
    std::cout << prog_name << " " << version << std::endl;
} // Banner_

void Help_() {
    Banner_();

    std::cout << "    sensors" << std::endl;
    std::cout << "        Display current sensor data" << std::endl << std::endl;
    std::cout << "    help" << std::endl;
    std::cout << "        This message" << std::endl << std::endl;
    std::cout << "    quit |  exit" << std::endl;
    std::cout << "        Quit program" << std::endl << std::endl;
} // Help_

void PrintSensors_() {
    for (auto it: std::views::values(clientMap)) {
        std::cout << "    " << it->getName() << std::endl;
        std::cout << "        Temperature: " << it->getTemperature() << std::endl;
    }
} // PrintSensors_

int main(void) {
    pinode::Client    client;

    // for now we hardcode our device list.  We should read this from a file and eventually discover

    deviceIps.emplace_back("127.0.0.1");
    //deviceIps.emplace_back("192.168.1.215");

    // initialize all clients
    for ( auto it : deviceIps) {
        pinode::ClientPtr client = std::make_shared<pinode::Client>();

        if (!client->Connect(it, 9999)) {
            std::cerr << "    ERROR: Failed to connect to device: " << it << std::endl;

            continue;
        }

        if (!client->Start()) {
            std::cerr << "    ERROR: Failed to start client: " << it << std::endl;

            continue;
        }

        // If we've initialized properly, we add it to our map.
        clientMap[it] = client;
    }

    for (;;) {
        std::string cmd;

        std::cout << prog_name << " " << version << "> ";

        std::getline(std::cin, cmd);

        if ((cmd == "exit") || (cmd == "quit")) {
            break;
        }
        else if (cmd == "help") {
            Help_();
        }
        else if (cmd == "sensors") {
            PrintSensors_();
        }
        else if (cmd == "poll") {
            bpl::sys::Tick tick(std::chrono::milliseconds(1000));

            auto endTime = std::chrono::steady_clock::now() + std::chrono::minutes(1);

            while (endTime > std::chrono::steady_clock::now()) {
                PrintSensors_();
                tick.Wait();
            }
        }
        else if (cmd == "") {
            // We don't do anything to prevent unecessary output.
        }
        else {
            std::cout << "    Unrecognized command: \"" << cmd << "\"" << std::endl;
            //std::cout << "    Use cmd \"help\" to get help." << std::endl;
        }
    }

    std::cout << "    Goodbye!" << std::endl;

    return 0;
} // main