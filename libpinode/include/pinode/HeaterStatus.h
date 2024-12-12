//
// Created by Bradley Remedios on 12/11/24.
//

#ifndef PINODE_HEATERSTATUS_H
#define PINODE_HEATERSTATUS_H

#include <string>

#include <bpl/net/Packet.h>

namespace pinode {

    struct Packet_HeaterStatus {
        uint16_t type;
        uint16_t len;
        uint32_t temperature;
        uint32_t programTemperature;
        bool isOn;
        bool isOverridden;
        bool reserved1;
        bool reserved2;
        char name[64];
    };

    class HeaterStatus {
    public:
        HeaterStatus() = default;
        virtual ~HeaterStatus() = default;

        bool isOverridden() const { return m_isOverridden; }
        void setOverridden(bool isOverridden) { m_isOverridden = isOverridden; }

        bool isOn() const { return m_isOn; }
        void setOn(bool isOn) { m_isOn = isOn; }

        const std::string& getName() const { return m_name; }
        void setName(const std::string& name) { m_name = name; }

        float getProgramTemperature() const { return m_programTemperature; }
        void setProgramTemperature(float programTemperature) { m_programTemperature = programTemperature; }

        float getTemperature() const { return m_temperature; }
        void setTemperature(float temperature) { m_temperature = temperature; }

        bool isPacket(bpl::net::PacketPtr& packet);
        bool SaveToPacket(bpl::net::PacketPtr& packet) const;
        bool LoadFromPacket(bpl::net::PacketPtr& packet);
    private:

        float m_temperature=0.0f;
        float m_programTemperature = 0.0f;
        bool m_isOverridden=false;
        bool m_isOn = false;
        std::string m_name;
    }; // HeaterStatus

    typedef std::shared_ptr<HeaterStatus> HeaterStatusPtr;
}; // pinode
#endif //PINODE_HEATERSTATUS_H