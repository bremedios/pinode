//
// Created by Bradley Remedios on 11/25/24.
//

#include <bpl/sys/Path.h>
#include <bpl/storage/Json.h>
#include <bpl/net/Packet.h>

#include <bpl/net/PacketCache.h>

#include "pinode/Server.h"
#include "pinode/PacketOpGetTemperature.h"
#include "pinode/PacketOpGetHumidity.h"
#include "pinode/PacketOpGetSensorInfo.h"
#include "pinode/PacketOpGetHeaterStatus.h"
#include "pinode/PacketOpOverrideTemperature.h"

#include "Debug.h"

namespace pinode {
    Server::Server() {
        m_udp = std::make_shared<bpl::net::Udp>();
        m_sensorInfo = std::make_shared<pinode::SensorInfo>();
    }

    Server::~Server() {
        Destroy();

        WaitForTermination();
    } // ~Server

    void Server::Destroy() {
        Terminate();

        if (nullptr != m_temperatureMonitor.get()) {
            m_temperatureMonitor->Terminate();
            m_temperatureMonitor->WaitForTermination();

            m_temperatureMonitor.reset();
        }

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

    bool Server::Start(const std::list<std::filesystem::path>& paths) {

        if (!LoadConfig_(paths)) {
	        ERROR_MSG("LoadConfig_() failed");

	        return false;
        }

        DEBUG_MSG("Binding to UDP Port " << m_port);

        if (!m_udp->Bind(m_port)) {
            ERROR_MSG("Failed to bind to port " << m_port);

            return false;
        }

        m_terminate = false;
        m_serverThread = std::make_shared<std::thread>(&Server::Svc_, this);

        return true;
    } // Start

    bool Server::EnableSensor() {
        if (m_temperatureMonitor.get() == nullptr) {
            m_temperatureMonitor = std::make_shared<TemperatureMonitor>();

            if (!m_temperatureMonitor->Start()) {
                ERROR_MSG("TemperatureMonitor::Start failed");

                return false;
            }
        }

        m_temperatureMonitor->setRefreshInterval(m_refreshInterval);

        // add packet handlers to the packet processor
        m_packetProcessor.AddPacketOp(PacketOpGetTemperature::packetOpCreate(m_udp, m_temperatureMonitor));
        m_packetProcessor.AddPacketOp(PacketOpGetHumidity::packetOpCreate(m_udp, m_temperatureMonitor));
        m_packetProcessor.AddPacketOp(PacketOpGetSensorInfo::packetOpCreate(m_udp, m_sensorInfo));

        DEBUG_MSG("Sensor Enabled");

        return true;
    } // EnableSensor

    bool Server::EnableHeater() {
        m_heaterStatus = std::make_shared<HeaterStatus>();

        m_packetProcessor.AddPacketOp(PacketOpGetHeaterStatus::packetOpCreate(m_udp, m_heaterStatus));
        m_packetProcessor.AddPacketOp(PacketOpOverrideTemperature::packetOpCreate(m_udp, m_heaterStatus));

        DEBUG_MSG("Heater Enabled");

        return true;
    } // EnableHeater

    bool Server::LoadConfig_(const std::list<std::filesystem::path>& paths) {
        std::string path = bpl::sys::Path::getFilenameFromList(paths);

        if (path.empty()) {
            ERROR_MSG("Could not find server configuration, using defaults");

            return false;
        }

        auto json = bpl::storage::Json::Open(path);

        int port;

        const rapidjson::Value  value = json->GetObject();

        if (bpl::storage::Json::Load(value, "port", port)) {
            m_port = port;
        }

        if (!bpl::storage::Json::Load(value, "enable-temperature", m_enableTemperature)) {
            ERROR_MSG("Cannot read temperature enable, using default");
        }

        if (!bpl::storage::Json::Load(value, "enable-humidity", m_enableHumidity)) {
            ERROR_MSG("Cannot read temperature enable, using default");
        }

        int refreshInterval = 0;

        if (bpl::storage::Json::Load(value, "refresh-interval", refreshInterval)) {
            m_refreshInterval = std::chrono::milliseconds(refreshInterval);
        }

        if (!bpl::storage::Json::Load(value, "location", m_location)) {
            ERROR_MSG("Cannot read location, using default");
        }

        m_sensorInfo->setLocation(m_location);
        m_sensorInfo->setHasHumiditySensor(m_enableHumidity);
        m_sensorInfo->setHasTemperatureSensor(m_enableTemperature);

        DEBUG_MSG("Location:              " << m_location);
        DEBUG_MSG("Port:                  " << m_port);
        DEBUG_MSG("Temperature Enabled:   " << m_enableTemperature);
        DEBUG_MSG("Humidity Enabled:      " << m_enableHumidity);
        DEBUG_MSG("Refresh interval (ms): " << m_refreshInterval.count());

        return true;
    } // LoadConfig_

    void Server::Terminate() {
        m_udp->Close();
        m_terminate = true;
    } // Terminate

    void Server::WaitForTermination() {
        m_serverThread->join();
    } // WaitForTermination
}; // namespace pinode