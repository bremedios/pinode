//
// Created by Bradley Remedios on 11/25/24.
//
#include <cmath>

#include <arpa/inet.h>

#include <bpl/net/PacketCache.h>

#include "pinode/PacketOpGetTemperature.h"
#include "pinode/PacketOps.h"

#include "Debug.h"

namespace pinode {
    PacketOpGetTemperature::PacketOpGetTemperature() {
        m_name = "Get Temperature";
    }

    PacketOpGetTemperature::PacketOpGetTemperature(bpl::net::UdpPtr udp, TemperatureMonitorPtr temperatureMonitor)
    	: m_temperatureMonitor(temperatureMonitor)
    	, m_udp(udp) {
        m_name = "Get Temperature";
    }

    bpl::net::PacketOpPtr PacketOpGetTemperature::packetOpCreate(bpl::net::UdpPtr udp, TemperatureMonitorPtr temperatureMonitor) {
        PacketOpGetTemperature* op = new PacketOpGetTemperature(udp, temperatureMonitor);

        return bpl::net::PacketOpPtr(op);
    } // packetOpCreate

    bool PacketOpGetTemperature::isMatch(const bpl::net::PacketPtr& packet) const {
        DEBUG_MSG("Checking if Packet matches");

        if (!packet->isValid()) {
            ERROR_MSG("Received invalid packet, cannot match");

            return false;
        }

        //  If we aren't the right amount of data, no need to continue to parse.
        if (sizeof(PacketGetTemperature) != packet->getPacketSize()) {
            DEBUG_MSG("Packet size mismatch.  PacketSize: " << packet->getPacketSize() << " Data  Size: " << sizeof(PacketGetTemperature));

            return false;
        }

        struct PacketGetTemperature *pgt = static_cast<struct PacketGetTemperature *>(packet->getPacketData());

        if (PacketOpType::PacketOp_GET_TEMPERATURE != ntohs(pgt->type)) {
            DEBUG_MSG("Packet type not Get Temperature");

            return false;
        }

        // final check for size, this should always be correct as long as we are not corrupt.
        if (sizeof(PacketGetTemperature) != ntohs(pgt->len)) {
            DEBUG_MSG("Packet has incorrect size encoded");

            return false;
        }

        return true;
    } // isMatch

    bool PacketOpGetTemperature::Process(const bpl::net::PacketPtr& packet, const bpl::net::AddrInfo& addr) {
        if (!isMatch(packet)) {
            return false;
        }

        struct PacketGetTemperature *pgt = static_cast<struct PacketGetTemperature *>(packet->getPacketData());

        // get a packet from the cache.
        bpl::net::PacketPtr outgoingPacket = bpl::net::PacketCache::getInstance().Pop();

        if (nullptr == outgoingPacket.get()) {
            ERROR_MSG("Failed to get packet from packet cache");

            return false;
        }

        // generate packet with sensor data
        PacketTemperature* pkt = static_cast<PacketTemperature*>(outgoingPacket->getPacketData());

        float temperature = m_temperatureMonitor->getTemperature();

        pkt->type = htons(PacketOp_TEMPERATURE);
        pkt->len = htons(sizeof(PacketTemperature));
        pkt->temperature = htonl(static_cast<int>(std::lround(temperature * 100)));

        outgoingPacket->setPacketDataSize(sizeof(PacketTemperature));

        if (!m_udp->Send(outgoingPacket, addr)) {
            ERROR_MSG("Failed to send packet");

            return false;
        }

        //  since we have processed the packet, we can push it back onto the packet cache.
        bpl::net::PacketCache::getInstance().Push(packet);

        return true;
    } // Process
}; // namesapce pinode