//
// Created by Bradley Remedios on 12/11/24.
//
#include <memory.h>
#include <cmath>

#include <arpa/inet.h>

#include <pinode/HeaterStatus.h>
#include <pinode/PacketOps.h>

#include "Debug.h"

namespace pinode{
    bool HeaterStatus::isPacket(bpl::net::PacketPtr& packet) {
        if (!packet->isValid()) {
            DEBUG_MSG("Packet not valid");

            return false;
        }

        if (packet->getPacketSize() < sizeof(struct Packet_HeaterStatus)) {
            DEBUG_MSG("Packet size incorrect");

            return false;
        }

        struct Packet_HeaterStatus* packetHeaterStatus = static_cast<struct Packet_HeaterStatus*>(packet->getPacketData());

        if (pinode::PacketOpType::PacketOp_HEATER_STATUS != ntohs(packetHeaterStatus->type)) {
            DEBUG_MSG("Packet type not Heater status");

            return false;
        }

        if (sizeof(struct Packet_HeaterStatus) != ntohs(packetHeaterStatus->len)) {
            DEBUG_MSG("Packet type not Heater status");

            return false;
        }

        return true;
    } // isPacket

    bool HeaterStatus::SaveToPacket(bpl::net::PacketPtr& packet) const {
        if (!packet->isValid()) {
            ERROR_MSG("Packet not valid, cannot save data to packet");
            return false;
        }

        struct Packet_HeaterStatus* packetHeaterStatus = static_cast<struct Packet_HeaterStatus*>(packet->getPacketData());

        DEBUG_MSG("Current Data");
        DEBUG_MSG("    Temperature:         " << getTemperature());
        DEBUG_MSG("    Program Temperature: " << getProgramTemperature());
        DEBUG_MSG("    On:                  " << isOn());

        packetHeaterStatus->type = htons(pinode::PacketOpType::PacketOp_HEATER_STATUS);
        packetHeaterStatus->len = htons(sizeof(struct Packet_HeaterStatus));

        packetHeaterStatus->isOn = isOn();
        packetHeaterStatus->isOverridden = isOverridden();
        packetHeaterStatus->temperature = htonl(static_cast<int>(std::lround(getTemperature() * 100)));
        packetHeaterStatus->programTemperature = htonl(static_cast<int>(std::lround(getProgramTemperature() * 100)));;

        std::string name = getName();

        // truncate the string if it exceeds our allowable space.
        if (name.length() > sizeof(packetHeaterStatus->name) - 1) {
            name.resize(sizeof(packetHeaterStatus->name) - 1);
        }

        strcpy(packetHeaterStatus->name, name.c_str());

        return true;
    } // SaveToPacket

    bool HeaterStatus::LoadFromPacket(bpl::net::PacketPtr& packet) {
        if (!isPacket(packet)) {
            return false;
        }

        struct Packet_HeaterStatus* packetHeaterStatus = static_cast<struct Packet_HeaterStatus*>(packet->getPacketData());

        setOn(packetHeaterStatus->isOn);
        setOverridden(packetHeaterStatus->isOverridden);
        setTemperature(static_cast<float>(ntohl(packetHeaterStatus->temperature)) / 100.0f);
        setProgramTemperature(static_cast<float>(ntohl(packetHeaterStatus->programTemperature)) / 100.0f);

        packetHeaterStatus->name[sizeof(packetHeaterStatus->name)-1] = '\0';

        setName(packetHeaterStatus->name);

        return true;
    } // LoadFromPacket
}; // pinode