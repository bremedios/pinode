//
// Created by Bradley Remedios on 11/24/24.
//
#include <iostream>
#include <chrono>

#include "TempMonitor.h"

int main() {
    std::cout << "Pi Temperature Node" << std::endl;

    std::chrono::steady_clock::time_point exitTime = std::chrono::steady_clock::now() + std::chrono::seconds(15);

    TempMonitor monitor;

    if (!monitor.Start()) {
        std::cerr << "Failed to start monitor" << std::endl;
    }

    while (std::chrono::steady_clock::now() < exitTime) {
        std::cout  << "\rTemperature: " << monitor.getTemperature();
    }

    monitor.WaitForTermination();

    return 0;
} // main
