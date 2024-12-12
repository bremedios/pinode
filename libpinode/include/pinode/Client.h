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

#include <pinode/HeaterStatus.h>

namespace pinode {
    class Client {
    public:
        Client();
        ~Client();

        bool Start();

        void Terminate();

        bool Connect(const std::string& host, unsigned short port);

        void EnableSensor(bool enable) { m_sensorEnable = enable;}
        void EnableHeater(bool enable) { m_heaterEnable = enable;}

        bool hasTemperature() const { return m_hasTemperature; }
        float getTemperature() const {return m_temperature; }
        std::chrono::time_point<std::chrono::steady_clock> getTemperatureTimestamp() const { return m_temperatureTimeStamp; };

        bool hasHumidity() const { return m_hasHumidity; }
        float getHumidity() const {return m_humidity; }
        std::chrono::time_point<std::chrono::steady_clock> getHumidityTimestamp() const { return m_humidityTimeStamp; };

        bool heaterStatusPacketValid() const { return m_heaterStatusPacketValid; }
        pinode::HeaterStatusPtr getHeaterStatus() const { return m_heaterStatus; }

        const std::string& getName() const {return m_name; }

        void WaitForTermination();
    private:

        void ReceiverSvc_();

        void MonitorSvc_();

        void SendGetTemperaturePacket_();
        void SendGetHumidityPacket_();
        void SendGetSensorInfo_();
        void SendGetHeaterStatus_();
        bool HandleTemperaturePacket_(bpl::net::PacketPtr packet);
        bool HandleHumidityPacket_(bpl::net::PacketPtr packet);
        bool HandleSensorInfoPacket_(bpl::net::PacketPtr packet);
        bool HandleHeaterStatusPacket_(bpl::net::PacketPtr packet);

        bool m_sensorEnable = false;
        bool m_heaterEnable = false;
        bool m_terminate = false;
        std::string m_name;
        bool m_hasTemperature = false;
        bool m_hasHumidity = false;

        float                                                 m_temperature=0;
        float                                                 m_humidity=0;
        std::chrono::time_point<std::chrono::steady_clock>   m_temperatureTimeStamp;
        std::chrono::time_point<std::chrono::steady_clock>   m_humidityTimeStamp;


        // heater status items
        pinode::HeaterStatusPtr m_heaterStatus;
        bool m_heaterStatusPacketValid = false;

        bpl::net::Udp                     m_udp;
        bpl::net::AddrInfo                m_addr;
        std::shared_ptr<std::thread>      m_receiverThread;
        std::shared_ptr<std::thread>      m_monitorThread;
        std::chrono::milliseconds         m_monitorPeriod;
    }; // class Client

    typedef std::shared_ptr<Client> ClientPtr;
}; // namespace pinode
#endif //PINODE_CLIENT_H_