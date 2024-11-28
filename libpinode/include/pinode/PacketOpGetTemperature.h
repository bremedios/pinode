//
// Created by bradr on 11/25/24.
//

#ifndef PINODE_PACKET_OP_GET_TEMPERATURE_H
#define PINODE_PACKET_OP_GET_TEMPERATURE_H

#include <bpl/net/Packet.h>
#include <bpl/net/PacketOp.h>
#include <bpl/net/AddrInfo.h>
#include <bpl/net/Udp.h>

#include <pinode/TemperatureMonitor.h>

namespace pinode {
    class PacketOpGetTemperature : public bpl::net::PacketOp {
    public:
        static bpl::net::PacketOpPtr packetOpCreate(bpl::net::UdpPtr udp, TemperatureMonitorPtr temperatureMonitor);

        bool isMatch(const bpl::net::PacketPtr& packet) const override;

        bool Process(const bpl::net::PacketPtr& packet, const bpl::net::AddrInfo& addr) override;

    private:
        PacketOpGetTemperature();
        PacketOpGetTemperature(bpl::net::UdpPtr udp, TemperatureMonitorPtr temperatureMonitor);

        TemperatureMonitorPtr m_temperatureMonitor;
        bpl::net::UdpPtr m_udp;
    }; // class PacketOpGetTemperature

    typedef std::shared_ptr<PacketOpGetTemperature> PacketOpGetTemperaturePtr;
}; // namespace pinode
#endif //PINODE_PACKET_OP_GET_TEMPERATURE_H