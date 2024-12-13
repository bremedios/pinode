//
// Created by Bradley Remedios on 12/09/24.
//
#include <cmath>

#include <arpa/inet.h>

#include <bpl/net/PacketCache.h>

#include "pinode/PacketOpGetSensorInfo.h"
#include "pinode/PacketOps.h"

#include "Debug.h"

namespace pinode {
    PacketOpGetSensorInfo::PacketOpGetSensorInfo() {
        m_name = "Get Humidity";
    }

    PacketOpGetSensorInfo::PacketOpGetSensorInfo(bpl::net::UdpPtr udp, SensorInfoPtr& sensorInfo)
    	: m_sensorInfo(sensorInfo)
    	, m_udp(udp) {
    }

    bpl::net::PacketOpPtr PacketOpGetSensorInfo::packetOpCreate(bpl::net::UdpPtr udp, SensorInfoPtr& sensorInfo) {
        PacketOpGetSensorInfo* op = new PacketOpGetSensorInfo(udp, sensorInfo);

        return bpl::net::PacketOpPtr(op);
    } // packetOpCreate

    bool PacketOpGetSensorInfo::isMatch(const bpl::net::PacketPtr& packet) const {
        DEBUG_MSG("Checking if Packet matches");

        if (!packet->isValid()) {
            ERROR_MSG("Received invalid packet, cannot match");

            return false;
        }

        //  If we aren't the right amount of data, no need to continue to parse.
        if (sizeof(PacketGetSensorInfo) != packet->getPacketSize()) {
            DEBUG_MSG("Packet size mismatch.  PacketSize: " << packet->getPacketSize() << " Data  Size: " << sizeof(PacketGetSensorInfo));

            return false;
        }

        struct PacketGetSensorInfo *pgt = static_cast<struct PacketGetSensorInfo *>(packet->getPacketData());

        if (PacketOpType::PacketOp_GET_SENSOR_INFO != ntohs(pgt->type)) {
            DEBUG_MSG("Packet type not Get Sensor Info");

            return false;
        }

        // final check for size, this should always be correct as long as we are not corrupt.
        if (sizeof(PacketGetSensorInfo) != ntohs(pgt->len)) {
            DEBUG_MSG("Packet has incorrect size encoded");

            return false;
        }


        DEBUG_MSG("Packet is Get Sensor Info");

        return true;
    } // isMatch

    bool PacketOpGetSensorInfo::Process(const bpl::net::PacketPtr& packet, const bpl::net::AddrInfo& addr) {
        if (!isMatch(packet)) {
            return false;
        }

        struct PacketGetSensorInfo *pgt = static_cast<struct PacketGetSensorInfo *>(packet->getPacketData());

        // get a packet from the cache.
        bpl::net::PacketPtr outgoingPacket = bpl::net::PacketCache::getInstance().Pop();

        if (nullptr == outgoingPacket.get()) {
            ERROR_MSG("Failed to get packet from packet cache");

            return false;
        }

        // generate packet with sensor data
        PacketSensorInfo* pkt = static_cast<PacketSensorInfo*>(outgoingPacket->getPacketData());

        pkt->type = htons(PacketOp_SENSOR_INFO);
        pkt->len = htons(sizeof(PacketSensorInfo));

        pkt->temperatureSensor = m_sensorInfo->hasTemperatureSensor();
        pkt->humiditySensor = m_sensorInfo->hasHumiditySensor();
        pkt->reserved1 = false;
        pkt->reserved2 = false;

        std::string location = m_sensorInfo->getLocation();

        if (location.length() + 1 > sizeof(pkt->location)) {
            ERROR_MSG("Location too long, truncating...");

            location.resize(sizeof(pkt->location) - 1);
        }

        strcpy(pkt->location, location.c_str());

        outgoingPacket->setPacketDataSize(sizeof(PacketSensorInfo));

        if (!m_udp->Send(outgoingPacket, addr)) {
            ERROR_MSG("Failed to send packet");

            return false;
        }

        //  since we have processed the packet, we can push it back onto the packet cache.
        bpl::net::PacketCache::getInstance().Push(packet);

        return true;
    } // Process
}; // namesapce pinode