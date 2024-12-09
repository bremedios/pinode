//
// Created by Bradley Remedios on 12/09/24.
//

#ifndef PINODE_PACKET_OP_GET_HUMIDITY_H
#define PINODE_PACKET_OP_GET_HUMIDITY_H

#include <bpl/net/Packet.h>
#include <bpl/net/PacketOp.h>
#include <bpl/net/AddrInfo.h>
#include <bpl/net/Udp.h>

#include <pinode/TemperatureMonitor.h>

namespace pinode {
class PacketOpGetHumidity : public bpl::net::PacketOp {
public:
  static bpl::net::PacketOpPtr packetOpCreate(bpl::net::UdpPtr udp, TemperatureMonitorPtr temperatureMonitor);

  bool isMatch(const bpl::net::PacketPtr& packet) const override;

  bool Process(const bpl::net::PacketPtr& packet, const bpl::net::AddrInfo& addr) override;

private:
  PacketOpGetHumidity();
  PacketOpGetHumidity(bpl::net::UdpPtr udp, TemperatureMonitorPtr temperatureMonitor);

  TemperatureMonitorPtr m_temperatureMonitor;
  bpl::net::UdpPtr m_udp;
}; // class PacketOpGetHumidity

typedef std::shared_ptr<PacketOpGetHumidity> PacketOpGetHumidityPtr;
}; // namespace pinode
#endif //PINODE_PACKET_OP_GET_HUMIDITY_H