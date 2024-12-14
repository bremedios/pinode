//
// Created by Bradley Remedios on 12/13/24.
//

#ifndef PINODE_PACKET_OP_OVERRIDE_TEMPERATURE_H
#define PINODE_PACKET_OP_OVERRIDE_TEMPERATURE_H

#include <bpl/net/Packet.h>
#include <bpl/net/PacketOp.h>
#include <bpl/net/AddrInfo.h>
#include <bpl/net/Udp.h>

#include <pinode/HeaterStatus.h>
#include <pinode/OverrideTemperature.h>

namespace pinode {
    class PacketOpOverrideTemperature : public bpl::net::PacketOp {
    public:
        static bpl::net::PacketOpPtr packetOpCreate(bpl::net::UdpPtr udp, pinode::HeaterStatusPtr& heaterStatus);

        bool isMatch(const bpl::net::PacketPtr& packet) const override;

        bool Process(const bpl::net::PacketPtr& packet, const bpl::net::AddrInfo& addr) override;

    private:
        PacketOpOverrideTemperature();
        PacketOpOverrideTemperature(bpl::net::UdpPtr udp, pinode::HeaterStatusPtr& heaterStatus);

        pinode::OverrideTemperaturePtr   m_overrideTemperature;
        pinode::HeaterStatusPtr          m_heaterStatus;
        bpl::net::UdpPtr                 m_udp;
    }; // class PacketOpOverrideTemperature

    typedef std::shared_ptr<PacketOpOverrideTemperature> PacketOpOverrideTemperaturePtr;
}; // namespace pinode
#endif //PINODE_PACKET_OP_OVERRIDE_TEMPERATURE_H