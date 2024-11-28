//
// Created by Bradley Remedios on 11/25/24.
//

#include <bpl/net/Packet.h>
#include <bpl/net/PacketCache.h>

#include "pinode/Server.h"
#include "pinode/PacketOpGetTemperature.h"

#include "Debug.h"

namespace pinode {
    Server::Server() {
    }

    Server::~Server() {
      	Destroy();
    } // ~Server

    void Server::Destroy() {
        Terminate();

        if (nullptr != m_temperatureMonitor.get()) {
            m_temperatureMonitor->Terminate();
        }

        m_temperatureMonitor->WaitForTermination();

        m_temperatureMonitor.reset();
    } // Destroy

    void Server::Svc_() {
        while (!m_terminate) {
            bpl::net::AddrInfo addrInfo;

            auto packet = bpl::net::PacketCache::getInstance().Pop();

            if (packet.get() == nullptr) {
                ERROR_MSG("PacketCache::getInstance() failed");

                return;
            }

            DEBUG_MSG("Calling Udp::Recv()");

            if (!m_udp->Recv(packet, addrInfo)) {
                ERROR_MSG("Recv Failed");
                continue;
            }

            DEBUG_MSG("Received " << packet->getPacketSize() << " bytes");

            if (!m_packetProcessor.ProcessPacket(packet, addrInfo)) {
                ERROR_MSG("ProcessPacket Failed");

                continue;
            }
        }
    }

    bool Server::Start() {
        m_udp = std::make_shared<bpl::net::Udp>();

        DEBUG_MSG("Binding to UDP Port " << m_port);

        if (!m_udp->Bind(m_port)) {
            ERROR_MSG("Failed to bind to port " << m_port);

            return false;
        }

        if (m_temperatureMonitor.get() == nullptr) {
            m_temperatureMonitor = std::make_shared<TemperatureMonitor>();

            if (!m_temperatureMonitor->Start()) {
                ERROR_MSG("TemperatureMonitor::Start failed");

                return false;
            }
        }

        // add packet handlers to the packet processor
        m_packetProcessor.AddPacketOp(PacketOpGetTemperature::packetOpCreate(m_udp, m_temperatureMonitor));

        return true;
    } // Start

    void Server::Terminate() {ERROR_MSG("Not implemented");}

    void Server::WaitForTermination() {ERROR_MSG("Not implemented");}
}; // namespace pinode