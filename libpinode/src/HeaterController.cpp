//
// Created by Bradley Remedios on 11/28/24.
//
#include <pinode/HeaterController.h>

#include "Debug.h"

namespace pinode {
    bool HeaterController::Update() {
        float temp = m_client->getTemperature();

        // We do nothing if we are within our temperature range to avoid turning
        // on and off too frequently.
        //
        // This should cause us to cycle between our high temp and low temp
        // point.
        if (temp > (m_temperature + (m_temperatureThreshold / 2))) {
            DEBUG_MSG("Turning off heater: Setpoint[" << std::to_string(m_temperature) << "] Current: [" << temp << "]");
            m_heaterControl->Off();
            m_heaterStatus->setOn(false);
        }
        else if (temp < (m_temperature - (m_temperatureThreshold / 2))) {
            DEBUG_MSG("Turning on heater: Setpoint[" << std::to_string(m_temperature) << "] Current: [" << temp << "]");
            m_heaterControl->On();
            m_heaterStatus->setOn(true);
        }
        else {
            DEBUG_MSG("Temperature Threshold: " << m_temperatureThreshold);
            DEBUG_MSG("High Threshold: " << m_temperature + (m_temperatureThreshold/2));
            DEBUG_MSG("Low Threshold: " << m_temperature - (m_temperatureThreshold/2));
            DEBUG_MSG("No change to heater: Setpoint[" << std::to_string(m_temperature) << "] Current: [" << temp << "]");

        }

        m_heaterStatus->setOverridden(false);   // not yet supported
        m_heaterStatus->setName("Heater");
        m_heaterStatus->setProgramTemperature(m_temperature); // not yet supported
        m_heaterStatus->setTemperature(m_temperature);

        return true;
    } // Update
}; // pinode