//
// Created by Bradley Remedios on 12/13/24.
//

#ifndef PINODE_OVERRIDETEMPERATURE_H
#define PINODE_OVERRIDETEMPERATURE_H

#include <string>

#include <bpl/net/Packet.h>

namespace pinode {

    struct Packet_OverrideTemperature {
        uint16_t type;
        uint16_t len;
        uint32_t temperature;
        bool clearOverride;
        bool reserved1;
        bool reserved2;
        bool reserved3;
    };

    class OverrideTemperature {
    public:
        OverrideTemperature() = default;
        virtual ~OverrideTemperature() = default;

        void setClearOverride(bool clear) { m_clearOverride = clear; }
        bool isClearOverrideSet() { return m_clearOverride; }

        float getTemperature() const { return m_temperature; }
        void setTemperature(float temperature) { m_temperature = temperature; }

        bool isPacket(const bpl::net::PacketPtr& packet);
        bool SaveToPacket(bpl::net::PacketPtr& packet) const;
        bool LoadFromPacket(const bpl::net::PacketPtr& packet);
    private:
        float m_temperature=0.0f;
        bool m_clearOverride=false;
    }; // OverrideTemperature

    typedef std::shared_ptr<OverrideTemperature> OverrideTemperaturePtr;
}; // pinode
#endif //PINODE_OVERRIDETEMPERATURE_H