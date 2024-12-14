//
// Created by Bradley Remedios on 11/27/24.
//
#include <pinode/Client.h>

#include <bpl/sys/Tick.h>
#include <bpl/net/PacketCache.h>
#include <bpl/net/Packet.h>

#include <pinode/PacketOps.h>
#include <pinode/OverrideTemperature.h>

#include "Debug.h"

namespace pinode {
    Client::Client()
        : m_monitorPeriod(5000)
        , m_monitorTick(std::chrono::milliseconds(5000)){

        m_heaterStatus = std::make_shared<pinode::HeaterStatus>();
    }

    Client::~Client() {
        Terminate();
        WaitForTermination();
    }

    bool Client::Connect(const std::string& host, unsigned short port) {
        DEBUG_CLIENT_MSG("Connect (" << host << ", " << port << ")");

        m_name = host + ":" + std::to_string(port);

        if (!m_udp.OpenSocket()) {
            ERROR_MSG("Failed to open UDP socket");

            return false;
        }

        if (!m_addr.FromIp(host)) {
            ERROR_MSG("Failed to parse IP address");

            return false;
        }

        m_addr.setPort(port);

        return true;
    } // Connect

    bool Client::Start() {
        DEBUG_CLIENT_MSG("Starting ...");
        m_terminate = false;

        m_monitorThread = std::make_shared<std::thread>(&Client::MonitorSvc_, this);
        m_receiverThread = std::make_shared<std::thread>(&Client::ReceiverSvc_, this);

        return true;
    } // Start

    void Client::Terminate() {
        DEBUG_CLIENT_MSG("Termination requested.");
        m_terminate = true;
        m_monitorTick.Release();

        m_udp.Close();
    } // Terminate

    void Client::WaitForTermination() {
        DEBUG_CLIENT_MSG("Waiting for termination requested");

        if (nullptr != m_monitorThread.get()) {
            DEBUG_CLIENT_MSG("Waiting for termination of monitor thread");
            m_monitorThread->join();

            m_monitorThread.reset();
        }

        if (nullptr != m_receiverThread.get()) {
            DEBUG_CLIENT_MSG("Waiting for termination of receiver thread");
            m_receiverThread->join();

            m_receiverThread.reset();
        }
        DEBUG_CLIENT_MSG("Waiting for termination completed");
    } // WaitForTermination

    bool Client::HandleTemperaturePacket_(bpl::net::PacketPtr packet) {
        PacketTemperature* pkt = static_cast<PacketTemperature*>(packet->getPacketData());

        if (sizeof(PacketTemperature) != packet->getPacketSize()) {
            DEBUG_CLIENT_MSG("Packet type incorrect size");

            return false;
        }

        if (PacketOpType::PacketOp_TEMPERATURE != ntohs(pkt->type)) {
            DEBUG_CLIENT_MSG("Packet type not Get Temperature");

            return false;
        }

        // final check for size, this should always be correct as long as we are not corrupt.
        if (sizeof(PacketTemperature) != ntohs(pkt->len)) {
            DEBUG_CLIENT_MSG("Packet has incorrect size encoded");

            return false;
        }

        m_temperature = static_cast<float>(ntohl(pkt->temperature)) / 100.0f;
        m_temperatureTimeStamp = std::chrono::steady_clock::now();

        DEBUG_CLIENT_MSG("Temperature Packet handled: Temp: " << m_temperature);

        return true;
    } // HandleTemperaturePacket_

    bool Client::HandleHumidityPacket_(bpl::net::PacketPtr packet) {
        PacketHumidity* pkt = static_cast<PacketHumidity*>(packet->getPacketData());

        if (sizeof(PacketHumidity) != packet->getPacketSize()) {
            DEBUG_CLIENT_MSG("Packet type incorrect size");

            return false;
        }


        if (PacketOpType::PacketOp_HUMIDITY != ntohs(pkt->type)) {
            DEBUG_CLIENT_MSG("Packet type not Get Humidity");

            return false;
        }

        // final check for size, this should always be correct as long as we are not corrupt.
        if (sizeof(PacketHumidity) != ntohs(pkt->len)) {
            DEBUG_CLIENT_MSG("Packet has incorrect size encoded");

            return false;
        }

        m_humidity = static_cast<float>(ntohl(pkt->humidity)) / 100.0f;
        m_humidityTimeStamp = std::chrono::steady_clock::now();

        return true;
    } // HandleHumidityPacket_

    bool Client::HandleSensorInfoPacket_(bpl::net::PacketPtr packet) {
        PacketSensorInfo* pkt = static_cast<PacketSensorInfo*>(packet->getPacketData());

        if (sizeof(PacketSensorInfo) != packet->getPacketSize()) {
            DEBUG_CLIENT_MSG("Packet type incorrect size");

            return false;
        }

        if (PacketOpType::PacketOp_SENSOR_INFO != ntohs(pkt->type)) {
            DEBUG_CLIENT_MSG("Packet type not Sensor Info");

            return false;
        }

        // final check for size, this should always be correct as long as we are not corrupt.
        if (sizeof(PacketSensorInfo) != ntohs(pkt->len)) {
            DEBUG_CLIENT_MSG("Packet has incorrect size encoded");

            return false;
        }

        // force null terminate in case it was not already
        pkt->location[sizeof(pkt->location) - 1] = '\0';

        m_name = std::string(pkt->location);
        m_hasTemperature = pkt->temperatureSensor;
        m_hasHumidity = pkt->humiditySensor;

        return true;
    } // HandleHumidityPacket_

    bool Client::HandleHeaterStatusPacket_(bpl::net::PacketPtr packet) {
        if (!m_heaterStatus->isPacket(packet)) {
            return false;
        }

        if (!m_heaterStatus->LoadFromPacket(packet)) {
            m_heaterStatusPacketValid = false;
            return false;
        }

        m_heaterStatusPacketValid = true;

        return true;
    } // HandleHeaterStatusPacket_

    void Client::ReceiverSvc_() {
        DEBUG_CLIENT_MSG("Receiver thread started");

        bpl::net::PacketPtr packet = bpl::net::PacketCache::getInstance().Pop();

        while (!m_terminate) {
            bpl::net::AddrInfo addr;

            if (!m_udp.Recv(packet, addr)) {
                ERROR_MSG("Failed to receive packet");
                continue;
            }

            if (m_terminate) {
                break;
            }

            if (HandleTemperaturePacket_(packet)) {
                DEBUG_CLIENT_MSG("Received temperature packet");
                continue;
            }

            if (HandleHumidityPacket_(packet)) {
                DEBUG_CLIENT_MSG("Received temperature packet");
                continue;
            }

            if (HandleSensorInfoPacket_(packet)) {
                DEBUG_CLIENT_MSG("Received sensor Info packet");
                continue;
            }

            if (HandleHeaterStatusPacket_(packet)) {
                DEBUG_CLIENT_MSG("Received Heater status packet");
                continue;
            }

            ERROR_MSG("Unknown packet received");
        }

        DEBUG_CLIENT_MSG("Receiver thread terminating...");
    } // ReceiverSvc_

    void Client::SendTemperatureOverride(float temperature) {
        pinode::OverrideTemperature override;

        override.setTemperature(temperature);

        bpl::net::PacketPtr packet = bpl::net::PacketCache::getInstance().Pop();

        override.SaveToPacket(packet);

        DEBUG_CLIENT_MSG("Sending Packet of " << packet->getPacketSize() << " bytes");

        if (!m_udp.Send(packet, m_addr)) {
            ERROR_MSG("Failed to send packet");
        }

        bpl::net::PacketCache::getInstance().Push(packet);
    } // SendTemperatureOverride

    void Client::ClearTemperatureOverride() {
        pinode::OverrideTemperature override;

        override.setClearOverride(true);

        bpl::net::PacketPtr packet = bpl::net::PacketCache::getInstance().Pop();

        override.SaveToPacket(packet);

        DEBUG_CLIENT_MSG("Sending Packet of " << packet->getPacketSize() << " bytes");

        if (!m_udp.Send(packet, m_addr)) {
            ERROR_MSG("Failed to send packet");
        }

        bpl::net::PacketCache::getInstance().Push(packet);
    } // ClearTemperatureOverride

    void Client::SendGetHumidityPacket_() {
        bpl::net::PacketPtr packet = bpl::net::PacketCache::getInstance().Pop();

        PacketGetHumidity* pkt = static_cast<PacketGetHumidity*>(packet->getPacketData());

        pkt->type = htons(pinode::PacketOpType::PacketOp_GET_HUMIDITY);
        pkt->len = htons(sizeof(PacketGetHumidity));

        packet->setPacketDataSize(sizeof(PacketGetHumidity));

        DEBUG_CLIENT_MSG("Sending Packet of " << sizeof(PacketGetHumidity) << " bytes");

        if (!m_udp.Send(packet, m_addr)) {
            ERROR_MSG("Failed to send packet");
        }

        bpl::net::PacketCache::getInstance().Push(packet);
    } // SendGetHumidityPacket_

    void Client::SendGetTemperaturePacket_() {
        bpl::net::PacketPtr packet = bpl::net::PacketCache::getInstance().Pop();

        PacketGetTemperature* pkt = static_cast<PacketGetTemperature*>(packet->getPacketData());

        pkt->type = htons(pinode::PacketOpType::PacketOp_GET_TEMPERATURE);
        pkt->len = htons(sizeof(PacketGetTemperature));

        packet->setPacketDataSize(sizeof(PacketGetTemperature));

        DEBUG_MSG("Sending Packet of " << sizeof(PacketGetTemperature) << " bytes");

        if (!m_udp.Send(packet, m_addr)) {
            ERROR_MSG("Failed to send packet");
        }

        bpl::net::PacketCache::getInstance().Push(packet);
    } // SendGetTemperaturePacket_

    void Client::SendGetSensorInfo_() {
        DEBUG_CLIENT_MSG("Sending Get Sensor Info");

        bpl::net::PacketPtr packet = bpl::net::PacketCache::getInstance().Pop();

        PacketGetSensorInfo* pkt = static_cast<PacketGetSensorInfo*>(packet->getPacketData());

        pkt->type = htons(pinode::PacketOpType::PacketOp_GET_SENSOR_INFO);
        pkt->len = htons(sizeof(PacketGetSensorInfo));

        packet->setPacketDataSize(sizeof(PacketGetSensorInfo));

        DEBUG_CLIENT_MSG("Sending Packet of " << sizeof(PacketGetSensorInfo) << " bytes");

        if (!m_udp.Send(packet, m_addr)) {
            ERROR_MSG("Failed to send packet");
        }

        bpl::net::PacketCache::getInstance().Push(packet);
    } // SendGetSensorInfo_

    void Client::SendGetHeaterStatus_() {
        bpl::net::PacketPtr packet = bpl::net::PacketCache::getInstance().Pop();

        PacketGetHeaterStatus* pkt = static_cast<PacketGetHeaterStatus*>(packet->getPacketData());

        pkt->type = htons(pinode::PacketOpType::PacketOp_GET_HEATER_STATUS);
        pkt->len = htons(sizeof(PacketGetHeaterStatus));

        packet->setPacketDataSize(sizeof(PacketGetHeaterStatus));

        DEBUG_CLIENT_MSG("Sending Packet of " << sizeof(PacketGetHeaterStatus) << " bytes");

        if (!m_udp.Send(packet, m_addr)) {
            ERROR_MSG("Failed to send packet");
        }

        bpl::net::PacketCache::getInstance().Push(packet);
    } // SendGetHeaterStatus_

    void Client::MonitorSvc_() {
        DEBUG_CLIENT_MSG("Monitor Thread Started");

        //  We'll send out all our queries every monitor period.
        while (!m_terminate) {
            // Wait until our next monitor period
            m_monitorTick.Wait();

            if (m_terminate) {
                DEBUG_CLIENT_MSG("Terminating");
                break;
            }

            if (m_sensorEnable) {
                if (m_hasTemperature) {
                    DEBUG_CLIENT_MSG("Sending Get Temperature");
                    SendGetTemperaturePacket_();
                }

                if (m_hasHumidity) {
                    DEBUG_CLIENT_MSG("Sending Get Humidity");
                    SendGetHumidityPacket_();
                }

                DEBUG_CLIENT_MSG("Sending Get Sensor Info");
                SendGetSensorInfo_();
            }
            if (m_heaterEnable) {
                DEBUG_CLIENT_MSG("Sending Get Heater Status");
                SendGetHeaterStatus_();
            }
        }
        DEBUG_CLIENT_MSG("Monitor thread terminating...");
    } // MonitorSvc_

}; // namespace pinode