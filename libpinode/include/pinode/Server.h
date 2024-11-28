//
// Created by Bradley Remedios on 11/25/24.
//

#ifndef PINODE_SERVER_H_
#define PINODE_SERVER_H_

#include <bpl/net/Udp.h>
#include <bpl/net/UdpPacketProcessor.h>
#include <bpl/net/AddrInfo.h>

#include <pinode/TemperatureMonitor.h>

namespace pinode {
    class Server{
    public:
        Server();
        virtual ~Server();

        void Destroy();

        //  WARNING: these must be enabled prior to calling Start()
        void enableTemperature(bool enable) { m_enableTemperature = enable; };
        void enableHumidity(bool enable) { m_enableHumidity = true; };

        bool Start();

        void Terminate();

        void WaitForTermination();

        // NOTE: call this directly for now as we only need 1 thread.
        void Svc_();
    private:
        uint16_t 					m_port=9999;
        bool                         m_terminate = false;
        bpl::net::UdpPtr                m_udp;
        bpl::net::UdpPacketProcessor m_packetProcessor;

        pinode::TemperatureMonitorPtr     m_temperatureMonitor;
        bool m_enableTemperature = false;
        bool m_enableHumidity = false;
    };
}; // namespace

#endif //PINODE_SERVER_H_
