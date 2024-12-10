//
// Created by Bradley Remedios on 12/09/24.
//

#ifndef PINODE_PACKET_OP_GET_SENSOR_INFO_H
#define PINODE_PACKET_OP_GET_SENSOR_INFO_H

#include <string>
#include <memory>

#include <bpl/net/Packet.h>
#include <bpl/net/PacketOp.h>
#include <bpl/net/AddrInfo.h>
#include <bpl/net/Udp.h>

#include <pinode/SensorInfo.h>

namespace pinode {
    class PacketOpGetSensorInfo : public bpl::net::PacketOp {
    public:
        static bpl::net::PacketOpPtr packetOpCreate(bpl::net::UdpPtr udp, SensorInfoPtr& sensorInfo);

        bool isMatch(const bpl::net::PacketPtr& packet) const override;

        bool Process(const bpl::net::PacketPtr& packet, const bpl::net::AddrInfo& addr) override;

    private:
        PacketOpGetSensorInfo();
        PacketOpGetSensorInfo(bpl::net::UdpPtr udp, SensorInfoPtr& sensorInfo);

        SensorInfoPtr m_sensorInfo;
        bpl::net::UdpPtr m_udp;
    }; // class PacketOpGetSensorInfo

    typedef std::shared_ptr<PacketOpGetSensorInfo> PacketOpGetSensorInfoPtr;
}; // namespace pinode
#endif //PINODE_PACKET_OP_GET_SENSOR_INFO_H