//
// Created by Bradley Remedios on 12/09/24.
//
#include <iostream>

#include <fmt/format.h>

#include <bpl/sys/Tick.h>
#include <bpl/storage/Json.h>

#include "pinode/Client.h"

std::string version = "v0.01";
std::string prog_name = "PiNode Logger";

std::list<std::string> deviceIps;
std::map<std::string, pinode::ClientPtr> clientMap;
std::list<std::string> devices;

void Banner_() {
    std::cout << std::endl;
    std::cout << prog_name << " " << version << std::endl;
} // Banner_

void Log_() {
    for (auto it: std::views::values(clientMap)) {
        std::cout << "    " << it->getName() << std::endl;
        std::cout << "        Temperature: " << it->getTemperature() << std::endl;
    }
} // PrintSensors_

int main(void) {
    std::cout << "PiNode Logger" << std::endl;

    pinode::Client    client;

    deviceIps.emplace_back("192.168.1.215");
    deviceIps.emplace_back("192.168.1.216");

    std::cout << "Initializing Clients" << std::endl;

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

    auto start = std::chrono::zoned_time{std::chrono::current_zone(),std::chrono::system_clock::now()};

    //std::string logFile = fmt::format("{}-sensor.csv", std::to_string(start));
    std::stringstream ss;

    ss << start << "-sensor.csv";

    std::string logFile = ss.str();

    std::ofstream csvFile = std::ofstream(logFile);

    csvFile << "Timestamp";

    std::cout << "Waiting for Clients to Get Sensor Info" << std::endl;

    //
    //  We wait for the client to get all of the details
    //
    for (auto it : clientMap) {
        bpl::sys::Tick connectTick(std::chrono::milliseconds(500));

        while (!it.second->hasTemperature()) {
            connectTick.Wait();
        }
    }

    for (auto it : clientMap) {
        devices.emplace_back(it.first);
        csvFile << "," << it.second->getName();
    }

    csvFile << std::endl << std::flush;

    bpl::sys::Tick tick(std::chrono::milliseconds(60000));

    std::cout << "Starting CSV Logging" << std::endl;

    for (;;) {
        csvFile << std::chrono::zoned_time{std::chrono::current_zone(),std::chrono::system_clock::now()};

        for (auto device : devices) {
            csvFile << fmt::format(",{:.2f}", clientMap[device]->getTemperature());

            std::cout << clientMap[device]->getName() << ": " << fmt::format("{:.2f}", clientMap[device]->getTemperature()) << std::endl;
        }

        csvFile << std::endl << std::flush;

        tick.Wait();
    }

    csvFile.close();

    std::cout << "    Goodbye!" << std::endl;

    return 0;
} // main