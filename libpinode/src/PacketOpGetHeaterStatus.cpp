//
// Created by Bradley Remedios on 12/11/24.
//
#include <cmath>

#include <arpa/inet.h>

#include <bpl/net/PacketCache.h>

#include <pinode/PacketOpGetHeaterStatus.h>
#include <pinode/PacketOps.h>

#include "Debug.h"

namespace pinode {
    PacketOpGetHeaterStatus::PacketOpGetHeaterStatus() {
        m_name = "Get Heater Status";
    }

    PacketOpGetHeaterStatus::PacketOpGetHeaterStatus(bpl::net::UdpPtr udp, HeaterStatusPtr& heaterStatus)
    	: m_heaterStatus(heaterStatus)
    	, m_udp(udp) {
    }

    bpl::net::PacketOpPtr PacketOpGetHeaterStatus::packetOpCreate(bpl::net::UdpPtr udp, HeaterStatusPtr& heaterStatus) {
        PacketOpGetHeaterStatus* op = new PacketOpGetHeaterStatus(udp, heaterStatus);

        return bpl::net::PacketOpPtr(op);
    } // packetOpCreate

    bool PacketOpGetHeaterStatus::isMatch(const bpl::net::PacketPtr& packet) const {
        DEBUG_MSG("Checking if Packet matches");

        if (!packet->isValid()) {
            ERROR_MSG("Received invalid packet, cannot match");

            return false;
        }

        //  If we aren't the right amount of data, no need to continue to parse.
        if (sizeof(PacketGetHeaterStatus) != packet->getPacketSize()) {
            DEBUG_MSG("Packet size mismatch.  PacketSize: " << packet->getPacketSize() << " Data  Size: " << sizeof(PacketGetHeaterStatus));

            return false;
        }

        struct PacketGetHeaterStatus *pgt = static_cast<struct PacketGetHeaterStatus *>(packet->getPacketData());

        if (PacketOpType::PacketOp_GET_HEATER_STATUS != ntohs(pgt->type)) {
            DEBUG_MSG("Packet type not Get Heater Status");

            return false;
        }

        // final check for size, this should always be correct as long as we are not corrupt.
        if (sizeof(PacketGetHeaterStatus) != ntohs(pgt->len)) {
            DEBUG_MSG("Packet has incorrect size encoded");

            return false;
        }

        DEBUG_MSG("Packet is Get Heater Status");
        return true;
    } // isMatch

    bool PacketOpGetHeaterStatus::Process(const bpl::net::PacketPtr& packet, const bpl::net::AddrInfo& addr) {
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

        if (!m_heaterStatus->SaveToPacket(outgoingPacket)) {
            ERROR_MSG("Failed to save heater status to packet");

            return false;
        }
        outgoingPacket->setPacketDataSize(sizeof(Packet_HeaterStatus));

        if (!m_udp->Send(outgoingPacket, addr)) {
            ERROR_MSG("Failed to send packet");

            return false;
        }

        //  since we have processed the packet, we can push it back onto the packet cache.
        bpl::net::PacketCache::getInstance().Push(packet);

        return true;
    } // Process
}; // namesapce pinode