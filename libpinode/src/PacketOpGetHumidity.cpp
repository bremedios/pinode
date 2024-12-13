//
// Created by Bradley Remedios on 11/25/24.
//
#include <cmath>

#include <arpa/inet.h>

#include <bpl/net/PacketCache.h>

#include "pinode/PacketOpGetHumidity.h"
#include "pinode/PacketOps.h"

#include "Debug.h"

namespace pinode {
    PacketOpGetHumidity::PacketOpGetHumidity() {
        m_name = "Get Humidity";
    }

    PacketOpGetHumidity::PacketOpGetHumidity(bpl::net::UdpPtr udp, TemperatureMonitorPtr temperatureMonitor)
    	: m_temperatureMonitor(temperatureMonitor)
    	, m_udp(udp) {
        m_name = "Get Humidity";
    }

    bpl::net::PacketOpPtr PacketOpGetHumidity::packetOpCreate(bpl::net::UdpPtr udp, TemperatureMonitorPtr temperatureMonitor) {
        PacketOpGetHumidity* op = new PacketOpGetHumidity(udp, temperatureMonitor);

        return bpl::net::PacketOpPtr(op);
    } // packetOpCreate

    bool PacketOpGetHumidity::isMatch(const bpl::net::PacketPtr& packet) const {
        DEBUG_MSG("Checking if Packet matches");

        if (!packet->isValid()) {
            ERROR_MSG("Received invalid packet, cannot match");

            return false;
        }

        //  If we aren't the right amount of data, no need to continue to parse.
        if (sizeof(PacketGetHumidity) != packet->getPacketSize()) {
            DEBUG_MSG("Packet size mismatch.  PacketSize: " << packet->getPacketSize() << " Data  Size: " << sizeof(PacketGetHumidity));

            return false;
        }

        struct PacketGetHumidity *pgt = static_cast<struct PacketGetHumidity *>(packet->getPacketData());

        if (PacketOpType::PacketOp_GET_HUMIDITY != ntohs(pgt->type)) {
            DEBUG_MSG("Packet type not Get Humidity");

            return false;
        }

        // final check for size, this should always be correct as long as we are not corrupt.
        if (sizeof(PacketGetHumidity) != ntohs(pgt->len)) {
            DEBUG_MSG("Packet has incorrect size encoded");

            return false;
        }

        DEBUG_MSG("Packet is Get Humidity");

        return true;
    } // isMatch

    bool PacketOpGetHumidity::Process(const bpl::net::PacketPtr& packet, const bpl::net::AddrInfo& addr) {
        if (!isMatch(packet)) {
            return false;
        }

        struct PacketGetHumidity *pgt = static_cast<struct PacketGetHumidity *>(packet->getPacketData());

        // get a packet from the cache.
        bpl::net::PacketPtr outgoingPacket = bpl::net::PacketCache::getInstance().Pop();

        if (nullptr == outgoingPacket.get()) {
            ERROR_MSG("Failed to get packet from packet cache");

            return false;
        }

        // generate packet with sensor data
        PacketHumidity* pkt = static_cast<PacketHumidity*>(outgoingPacket->getPacketData());

        float humidity = m_temperatureMonitor->getHumidity();

        pkt->type = htons(PacketOp_HUMIDITY);
        pkt->len = htons(sizeof(PacketHumidity));
        pkt->humidity = htonl(static_cast<int>(std::lround(humidity * 100)));

        outgoingPacket->setPacketDataSize(sizeof(PacketHumidity));

        if (!m_udp->Send(outgoingPacket, addr)) {
            ERROR_MSG("Failed to send packet");

            return false;
        }

        //  since we have processed the packet, we can push it back onto the packet cache.
        bpl::net::PacketCache::getInstance().Push(packet);

        return true;
    } // Process
}; // namesapce pinode