//
// Created by Bradley Remedios on 11/28/24.
//
#include <pinode/HeaterController.h>

#include "Debug.h"

namespace pinode {
    HeaterController::HeaterController() {
        m_timeStart.set(22, 0, 0);
        m_timeStop.set(9,0,0);
    }

    bool HeaterController::Update() {
        float temp = m_client->getTemperature();

        auto m_targetTemperature=m_temperature;

        // set our target temperature based on the time.
        if (m_timeStop > m_timeStart) {
            bpl::sys::LocalTimePoint now;
            now.setNow();

            if ((now > m_timeStop) && (now < m_timeStop)) {
                m_targetTemperature = m_temperature;
            }
            else {
                m_targetTemperature = m_temperatureLow;
            }
        }
        else if (m_timeStop < m_timeStart) {
            bpl::sys::LocalTimePoint now;
            now.setNow();

            if ((now > m_timeStart) || (now < m_timeStop)) {
                m_targetTemperature = m_temperature;
            }
            else {
                m_targetTemperature = m_temperatureLow;
            }
        }

        // We do nothing if we are within our temperature range to avoid turning
        // on and off too frequently.
        //
        // This should cause us to cycle between our high temp and low temp
        // point.
        if (temp > (m_targetTemperature + (m_temperatureThreshold / 2))) {
            DEBUG_MSG("Turning off heater: Setpoint[" << std::to_string(m_targetTemperature) << "] Current: [" << temp << "]");
            m_heaterControl->Off();
            m_heaterStatus->setOn(false);
        }
        else if (temp < (m_targetTemperature - (m_temperatureThreshold / 2))) {
            DEBUG_MSG("Turning on heater: Setpoint[" << std::to_string(m_targetTemperature) << "] Current: [" << temp << "]");
            m_heaterControl->On();
            m_heaterStatus->setOn(true);
        }
        else {
            DEBUG_MSG("Temperature Threshold: " << m_temperatureThreshold);
            DEBUG_MSG("High Threshold: " << m_targetTemperature + (m_temperatureThreshold/2));
            DEBUG_MSG("Low Threshold: " << m_targetTemperature - (m_temperatureThreshold/2));
            DEBUG_MSG("No change to heater: Setpoint[" << std::to_string(m_targetTemperature) << "] Current: [" << temp << "]");
        }

        // update our heater status values
        m_heaterStatus->setOverridden(false);   // not yet supported
        m_heaterStatus->setName("Heater");
        m_heaterStatus->setProgramTemperature(m_targetTemperature); // not yet supported
        m_heaterStatus->setTemperature(m_targetTemperature);

        return true;
    } // Update
}; // pinode