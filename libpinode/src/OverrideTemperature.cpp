//
// Created by Bradley Remedios on 12/13/24.
//
#include <memory.h>
#include <cmath>

#include <arpa/inet.h>

#include <pinode/OverrideTemperature.h>
#include <pinode/PacketOps.h>

#include "Debug.h"

namespace pinode{
    bool OverrideTemperature::isPacket(const bpl::net::PacketPtr& packet) {
        if (!packet->isValid()) {
            DEBUG_MSG("Packet not valid");

            return false;
        }

        if (packet->getPacketSize() < sizeof(struct Packet_OverrideTemperature)) {
            DEBUG_MSG("Packet size incorrect");

            return false;
        }

        struct Packet_OverrideTemperature* packetOverrideTemperature = static_cast<struct Packet_OverrideTemperature*>(packet->getPacketData());

        if (pinode::PacketOpType::PacketOp_OVERRIDE_TEMPERATURE != ntohs(packetOverrideTemperature->type)) {
            DEBUG_MSG("Packet type not Override Temperature");

            return false;
        }

        if (sizeof(struct Packet_OverrideTemperature) != ntohs(packetOverrideTemperature->len)) {
            DEBUG_MSG("Packet type not Heater status");

            return false;
        }

        return true;
    } // isPacket

    bool OverrideTemperature::SaveToPacket(bpl::net::PacketPtr& packet) const {
        if (!packet->isValid()) {
            ERROR_MSG("Packet not valid, cannot save data to packet");
            return false;
        }

        struct Packet_OverrideTemperature* packetOverrideTemperature = static_cast<struct Packet_OverrideTemperature*>(packet->getPacketData());

        DEBUG_MSG("Current Data");
        DEBUG_MSG("    Temperature:         " << getTemperature());

        packetOverrideTemperature->type = htons(pinode::PacketOpType::PacketOp_OVERRIDE_TEMPERATURE);
        packetOverrideTemperature->len = htons(sizeof(struct Packet_OverrideTemperature));
        packetOverrideTemperature->temperature = htonl(static_cast<int>(std::lround(getTemperature() * 100)));

        return true;
    } // SaveToPacket

    bool OverrideTemperature::LoadFromPacket(const bpl::net::PacketPtr& packet) {
        if (!isPacket(packet)) {
            return false;
        }

        struct Packet_OverrideTemperature* packetOverrideTemperature = static_cast<struct Packet_OverrideTemperature*>(packet->getPacketData());

        setTemperature(static_cast<float>(ntohl(packetOverrideTemperature->temperature)) / 100.0f);

        return true;
    } // LoadFromPacket
}; // pinode