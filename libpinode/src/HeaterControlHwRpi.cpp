//
// Created by Bradley Remedios on 11/28/24.
//
#include <wiringPi.h>

#include <pinode/HeaterControlHwRpi.h>

#include "Debug.h"

namespace pinode {

    HeaterControlHwRpi::HeaterControlHwRpi() {
        DEBUG_HW_MSG("Enablilng Raspberry Pi HeaterControl");
        if (0 > wiringPiSetup()) {
          ERROR_MSG("wiringPiSetup failed");
        }

        if (setuid(getuid()) < 0)
        {
          ERROR_MSG("setuid() failed");
        }

        //
        //  Initialize to off
        //
        Off();
    }// HeaterControlHwRpi

    void HeaterControlHwRpi::On() {
        DEBUG_HW_MSG("Turning Heater On");
        pinMode(m_pin, OUTPUT);
        digitalWrite(m_pin, HIGH);
    }// On

    void HeaterControlHwRpi::Off() {
        DEBUG_HW_MSG("Turning Heater Off");
        pinMode(m_pin, OUTPUT);
        digitalWrite(m_pin, LOW);
    }// Off
}; // namespace pinode