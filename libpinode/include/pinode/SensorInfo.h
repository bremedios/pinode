//
// Created by Bradley Remedios on 12/9/24.
//

#ifndef SENSORINFO_H
#define SENSORINFO_H

#include <string>
#include <memory>

namespace pinode
{
    class SensorInfo {
    public:
        SensorInfo () =default;


        const std::string& getLocation() const {return m_location;}
        void setLocation(const std::string& location) {m_location = location;}

        bool hasTemperatureSensor() const { return m_temperatureSensor; }
        bool hasHumiditySensor() const { return m_humiditySensor; }
        void setHasTemperatureSensor(bool temperatureSensor) { m_temperatureSensor = temperatureSensor; }
        void setHasHumiditySensor(bool humiditySensor) { m_humiditySensor = humiditySensor; }
    private:
        std::string m_location;

        bool m_temperatureSensor=false;
        bool m_humiditySensor=false;
        bool m_reserved1=false;
        bool m_reserved2=false;

    }; // SensorInfo

    typedef std::shared_ptr<SensorInfo> SensorInfoPtr;
}; // pinode
#endif //SENSORINFO_H