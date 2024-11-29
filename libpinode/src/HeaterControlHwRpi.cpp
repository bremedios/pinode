//
// Created by Bradley Remedios on 11/28/24.
//
#ifndef PINODE_HEATERCONTROL_HW_RPI_H
#define PINODE_HEATERCONTROL_HW_RPI_H

#include <wiringPi.h>

#include <pinode/HeaterControlHwRpi.h>

#include "Debug.h"

namespace pinode {

    void HeaterControlHwRpi::HeaterControlHwRpi() {
        DEBUG_MSG("Enablilng Raspberry Pi HeaterControl");
        if (0 > wiringPiSetup()) {
          ERROR_MSG("wiringPiSetup failed");

          return false;
        }

        if (setuid(getuid()) < 0)
        {
          ERROR_MSG("setuid() failed");
          return false;
        }

        //
        //  Initialize to off
        //
        Off();
    }// HeaterControlHwRpi

    void HeaterControlHwRpi::On() {
        DEBUG_MSG("Turning Heater On");
        pinMode(m_pin, OUTPUT);
        digitalWrite(m_pin, HIGH);
        std::cout <<
    }// On

    void HeaterControlHwRpi::Off() {
        DEBUG_MSG("Turning Heater Off");
        pinMode(m_pin, OUTPUT);
        digitalWrite(m_pin, LOW);
    }// Off
}; // namespace pinode

#endif // PINODE_HEATERCONTROL_HW_RPI_H