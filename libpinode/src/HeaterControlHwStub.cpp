//
// Created by Bradley Remedios on 11/28/24.
//
#include <iostream>

#include <pinode/HeaterControlHwStub.h>

#include "Debug.h"

namespace pinode {
    void HeaterControlHwStub::On() {
        DEBUG_HW_MSG("Heater On");
    } // On

    void HeaterControlHwStub::Off() {
        DEBUG_HW_MSG("Heater Off");
    } // Off
}; // namespace pinode