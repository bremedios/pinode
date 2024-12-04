//
// Created by Bradley Remedios on 11/27/24.
//
#include <pinode/Client.h>

#include <bpl/sys/Tick.h>
#include <bpl/net/PacketCache.h>
#include <bpl/net/Packet.h>

#include <pinode/PacketOps.h>

#include "Debug.h"

namespace pinode {
    Client::Client()
        : m_monitorPeriod(5000) {
    }

    Client::~Client() {
        Terminate();
        WaitForTermination();
    }

    bool Client::Connect(const std::string& host, unsigned short port) {
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
        m_terminate = false;

        m_monitorThread = std::make_shared<std::thread>(&Client::MonitorSvc_, this);
        m_receiverThread = std::make_shared<std::thread>(&Client::ReceiverSvc_, this);

        return true;
    } // Start

    void Client::Terminate() {
        DEBUG_MSG("Termination requested.");
        m_terminate = true;
    } // Terminate

    void Client::WaitForTermination() {
        Terminate();

        if (nullptr != m_monitorThread.get()) {
            DEBUG_MSG("Waiting for termination of monitor thread");
            m_monitorThread->join();

            m_monitorThread.reset();
        }

        if (nullptr != m_receiverThread.get()) {
            DEBUG_MSG("Waiting for termination of receiver thread");
            m_receiverThread->join();

            m_receiverThread.reset();
        }
    } // WaitForTermination

    bool Client::HandleTemperaturePacket_(bpl::net::PacketPtr packet) {
        PacketTemperature* pkt = static_cast<PacketTemperature*>(packet->getPacketData());

        if (sizeof(PacketTemperature) != packet->getPacketSize()) {
            DEBUG_MSG("Packet type incorrect size");

            return false;
        }


        if (PacketOpType::PacketOp_TEMPERATURE != ntohs(pkt->type)) {
            DEBUG_MSG("Packet type not Get Temperature");

            return false;
        }

        // final check for size, this should always be correct as long as we are not corrupt.
        if (sizeof(PacketTemperature) != ntohs(pkt->len)) {
            DEBUG_MSG("Packet has incorrect size encoded");

            return false;
        }

        m_temperature = static_cast<float>(ntohl(pkt->temperature)) / 100.0f;
        m_temperatureTimeStamp = std::chrono::steady_clock::now();

        return true;
    } // HandleTemperaturePacket_

    void Client::ReceiverSvc_() {
        bpl::net::PacketPtr packet = bpl::net::PacketCache::getInstance().Pop();

        while (!m_terminate) {
            bpl::net::AddrInfo addr;

             if (!m_udp.Recv(packet, addr)) {
                 ERROR_MSG("Failed to receive packet");
                 continue;
             }

            if (HandleTemperaturePacket_(packet)) {
                DEBUG_MSG("Received temperature packet");
                continue;
            }

            ERROR_MSG("Unknown packet received");
        }

        DEBUG_MSG("Receiver thread terminating...");
    } // ReceiverSvc_

    void Client::MonitorSvc_() {
        bpl::sys::Tick tick(m_monitorPeriod);

        //  We'll send out all our queries every monitor period.
        while (!m_terminate) {
            // Wait until our next monitor period
            tick.Wait();

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
        }
        DEBUG_MSG("Monitor thread terminating...");
    } // MonitorSvc_

}; // namespace pinode