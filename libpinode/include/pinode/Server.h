//
// Created by Bradley Remedios on 11/25/24.
//

#ifndef PINODE_SERVER_H_
#define PINODE_SERVER_H_

#include <bpl/net/Udp.h>
#include <bpl/net/UdpPacketProcessor.h>
#include <bpl/net/AddrInfo.h>

#include <pinode/TemperatureMonitor.h>
#include <pinode/HeaterStatus.h>
#include <pinode/SensorInfo.h>

namespace pinode {
    class Server{
    public:
        Server();
        virtual ~Server();

        void Destroy();

        //  WARNING: these must be enabled prior to calling Start()
        void enableTemperature(bool enable) { m_enableTemperature = enable; };
        void enableHumidity(bool enable) { m_enableHumidity = true; };

        bool Start(const std::list<std::filesystem::path>& paths);

        bool EnableSensor();
        bool EnableHeater();

        void Terminate();

        void WaitForTermination();

        pinode::HeaterStatusPtr getHeaterStatus() { return m_heaterStatus; }

    private:
        void Svc_();
        bool LoadConfig_(const std::list<std::filesystem::path>& paths);

        pinode::SensorInfoPtr        m_sensorInfo;
        std::chrono::milliseconds    m_refreshInterval;
        bool                         m_enableTemperature = true;
        bool                         m_enableHumidity    = true;
        uint16_t 					 m_port              = 9999;
        std::string                  m_location = "<No Location Set>";
        bpl::net::UdpPtr             m_udp;
        bpl::net::UdpPacketProcessor m_packetProcessor;

        std::shared_ptr<std::thread>     m_serverThread;
        bool                            m_terminate = false;

        pinode::TemperatureMonitorPtr     m_temperatureMonitor;
        pinode::HeaterStatusPtr          m_heaterStatus;
    };

    typedef std::shared_ptr<Server> ServerPtr;
}; // namespace

#endif //PINODE_SERVER_H_