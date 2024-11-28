//
// Created by Bradley Remedios on 11/24/24.
//

#include <iostream>

#include <devices/dht22.h>

#include <pinode/TemperatureMonitor.h>

#include "Debug.h"

namespace pinode {

TemperatureMonitor::~TemperatureMonitor() {

    if (m_monitorThread.get() != nullptr) {
        WaitForTermination();
    }
} // TemperatureMonitor

bool TemperatureMonitor::Start() {
    if (m_monitorThread.get() != nullptr) {
        ERROR_MSG("Thread already running, cannot start");

        return false;
    }

    m_terminate = false;
    m_monitorThread = std::make_shared<std::thread>(&TemperatureMonitor::svc_, this);

    return true;
} // Start

void TemperatureMonitor::WaitForTermination() {
    // If we are not terminating, we terminate.
    if (!m_terminate) {
        Terminate();
    }

    if (m_monitorThread.get() == nullptr) {
        ERROR_MSG("No thread active");

        return;
    }

    m_monitorThread->join();

    m_monitorThread.reset();
} // WaitForTermination

void TemperatureMonitor::Terminate() {
    m_terminate = true;
} // Terminate

void TemperatureMonitor::svc_() {
    devices::dht22 dht;

    if (!dht.Create()) {
        ERROR_MSG("Failed to initialize DHT22");
        return;
    }

    std::chrono::steady_clock::time_point nextCheck = std::chrono::steady_clock::now();

    while (!m_terminate) {
        std::this_thread::sleep_until(nextCheck);

        if (!dht.Read()) {
            ERROR_MSG("Failed to read DHT22");
            continue;
        }

        nextCheck = std::chrono::steady_clock::now() + m_refreshInterval;

        m_temperature = dht.getTemperature();
        m_humidity = dht.getHumidity();
    }
} // svc_

}; // namespace pinode
