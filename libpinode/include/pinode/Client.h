//
// Created by Bradley Remedios on 11/27/24.
//

#ifndef PINODE_CLIENT_H_
#define PINODE_CLIENT_H_
#include <string>
#include <memory>
#include <chrono>

#include <bpl/net/AddrInfo.h>
#include <bpl/net/Udp.h>

namespace pinode {
    class Client {
    public:
        Client();
        ~Client();

        bool Start();

        void Terminate();

        bool Connect(const std::string& host, unsigned short port);

        float getTemperature() const {return m_temperature; }
        std::chrono::time_point<std::chrono::steady_clock> getTemperatureTimestamp() const { return m_temperatureTimeStamp; };

        float getHumidity() const {return m_humidity; }
        std::chrono::time_point<std::chrono::steady_clock> getHumidityTimestamp() const { return m_humidityTimeStamp; };

        const std::string& getName() const {return m_name; }

        void WaitForTermination();
    private:

        void ReceiverSvc_();

        void MonitorSvc_();

        void SendGetTemperaturePacket_();
        void SendGetHumidityPacket_();
        bool HandleTemperaturePacket_(bpl::net::PacketPtr packet);
        bool HandleHumidityPacket_(bpl::net::PacketPtr packet);

        bool m_terminate = false;
        std::string m_name;
        float                                                 m_temperature=0;
        float                                                 m_humidity=0;
        std::chrono::time_point<std::chrono::steady_clock>   m_temperatureTimeStamp;
        std::chrono::time_point<std::chrono::steady_clock>   m_humidityTimeStamp;
        bpl::net::Udp                     m_udp;
        bpl::net::AddrInfo                m_addr;
        std::shared_ptr<std::thread>      m_receiverThread;
        std::shared_ptr<std::thread>      m_monitorThread;
        std::chrono::milliseconds         m_monitorPeriod;
    }; // class Client

    typedef std::shared_ptr<Client> ClientPtr;
}; // namespace pinode
#endif //PINODE_CLIENT_H_