//
// Created by Bradley Remedios on 11/28/24.
//
#ifndef PINODE_HEATERCONTROL_HW_RPI_H
#define PINODE_HEATERCONTROL_HW_RPI_H

#include <wiringPi.h>

#include <pinode/HeaterControlHwRpi.h>

namespace pinode {

    void HeaterControlHwRpi::HeaterControlHwRpi() {
        if (0 > wiringPiSetup()) {
          std::cerr << "dht22::Create: wiringPiSetup failed" << std::endl;

          return false;
        }

        if (setuid(getuid()) < 0)
        {
          std::cerr << "dht22::Create: setuid() failed" << std::endl;
          return false;
        }

        //
        //  Initialize to off
        //
        Off();
    }// HeaterControlHwRpi

    void HeaterControlHwRpi::On() {
        pinMode(m_pin, OUTPUT);
        digitalWrite(m_pin, HIGH);
    }// On

    void HeaterControlHwRpi::Off() {
        pinMode(m_pin, OUTPUT);
        digitalWrite(m_pin, LOW);
    }// Off
}; // namespace pinode

#endif // PINODE_HEATERCONTROL_HW_RPI_H