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
        m_sensorInfo = std::make_shared<pinode::SensorInfo>();

        if (!LoadConfig_()) {
	    ERROR_MSG("LoadConfig_() failed");

	    return false;
        }

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

        m_temperatureMonitor->setRefreshInterval(m_refreshInterval);

        // add packet handlers to the packet processor
        m_packetProcessor.AddPacketOp(PacketOpGetTemperature::packetOpCreate(m_udp, m_temperatureMonitor));
        m_packetProcessor.AddPacketOp(PacketOpGetHumidity::packetOpCreate(m_udp, m_temperatureMonitor));
        m_packetProcessor.AddPacketOp(PacketOpGetSensorInfo::packetOpCreate(m_udp, m_sensorInfo));

        return true;
    } // Start


    bool Server::LoadConfig_() {
        std::list<std::filesystem::path> paths;

        paths.emplace_back("pinode-sensor.json");
        paths.emplace_back("/etc/pinode/sensor.json");

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
    } // LoadConfig

    void Server::Terminate() {ERROR_MSG("Not implemented");}

    void Server::WaitForTermination() {ERROR_MSG("Not implemented");}
}; // namespace pinode