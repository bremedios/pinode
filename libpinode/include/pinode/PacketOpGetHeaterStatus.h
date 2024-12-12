//
// Created by Bradley Remedios on 12/11/24.
//

#ifndef PINODE_PACKET_OP_GET_HEATER_STATUS_H
#define PINODE_PACKET_OP_GET_HEATER_STATUS_H

#include <bpl/net/Packet.h>
#include <bpl/net/PacketOp.h>
#include <bpl/net/AddrInfo.h>
#include <bpl/net/Udp.h>

#include <pinode/HeaterStatus.h>

namespace pinode {
    class PacketOpGetHeaterStatus : public bpl::net::PacketOp {
    public:
        static bpl::net::PacketOpPtr packetOpCreate(bpl::net::UdpPtr udp, pinode::HeaterStatusPtr& heaterStatus);

        bool isMatch(const bpl::net::PacketPtr& packet) const override;

        bool Process(const bpl::net::PacketPtr& packet, const bpl::net::AddrInfo& addr) override;

    private:
        PacketOpGetHeaterStatus();
        PacketOpGetHeaterStatus(bpl::net::UdpPtr udp, pinode::HeaterStatusPtr& heaterStatus);

        pinode::HeaterStatusPtr m_heaterStatus;
        bpl::net::UdpPtr m_udp;
}; // class PacketOpGetHeaterStatus

typedef std::shared_ptr<PacketOpGetHeaterStatus> PacketOpGetHeaterStatusPtr;
}; // namespace pinode
#endif //PINODE_PACKET_OP_GET_HEATER_STATUS_H