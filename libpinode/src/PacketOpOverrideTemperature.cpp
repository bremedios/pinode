//
// Created by Bradley Remedios on 12/13/24.
//
#include <cmath>

#include <arpa/inet.h>

#include <bpl/net/PacketCache.h>

#include <pinode/PacketOpOverrideTemperature.h>
#include <pinode/PacketOps.h>

#include "Debug.h"

namespace pinode {
    PacketOpOverrideTemperature::PacketOpOverrideTemperature() {
        m_name = "Override Temperature";
        m_overrideTemperature = std::make_shared<pinode::OverrideTemperature>();
    }

    PacketOpOverrideTemperature::PacketOpOverrideTemperature(bpl::net::UdpPtr udp, HeaterStatusPtr& heaterStatus)
    	: m_heaterStatus(heaterStatus)
        , m_udp(udp) {
        m_name = "Override Temperature";
        m_overrideTemperature = std::make_shared<pinode::OverrideTemperature>();
    }

    bpl::net::PacketOpPtr PacketOpOverrideTemperature::packetOpCreate(bpl::net::UdpPtr udp, HeaterStatusPtr& heaterStatus) {
        PacketOpOverrideTemperature* op = new PacketOpOverrideTemperature(udp, heaterStatus);

        return bpl::net::PacketOpPtr(op);
    } // packetOpCreate

    bool PacketOpOverrideTemperature::isMatch(const bpl::net::PacketPtr& packet) const {
        DEBUG_MSG("Checking if Packet matches");

        if (!m_overrideTemperature->isPacket(packet)) {
            ERROR_MSG("Received invalid packet, cannot match");

            return false;
        }

        DEBUG_MSG("Packet is Override Temperature");

        return true;
    } // isMatch

    bool PacketOpOverrideTemperature::Process(const bpl::net::PacketPtr& packet, const bpl::net::AddrInfo& addr) {
        if (!m_overrideTemperature->LoadFromPacket(packet)) {
            ERROR_MSG("Failed to load data from packet");

            return false;
        }

        m_heaterStatus->setTemperature(m_overrideTemperature->getTemperature());
        m_heaterStatus->setOverridden(true);

        return true;
    } // Process
}; // namesapce pinode