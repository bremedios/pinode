//
// Created by Bradley Remedios on 11/28/24.
//
#include <iostream>

#include <pinode/HeaterControlHwStub.h>

namespace pinode {
    void HeaterControlHwStub::On() {
        std::cout << "Heater On" << std::endl;
    } // On

    void HeaterControlHwStub::Off() {
        std::cout << "Heater Off" << std::endl;
    } // Off
}; // namespace pinode