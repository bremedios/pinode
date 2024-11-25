//
// Created by Bradley Remedios on 11/24/24.
//
#include <iostream>

#include <devices/dht22.h>

#include "TempMonitor.h"

TempMonitor::~TempMonitor() {

    if (m_monitorThread.get() != nullptr) {
        WaitForTermination();
    }
} // TempMonitor

bool TempMonitor::Start() {
    if (m_monitorThread.get() != nullptr) {
        std::cerr << "TempMonitor::Start(): Thread already running, cannot start" << std::endl;

        return false;
    }

    m_terminate = false;
    m_monitorThread = std::make_shared<std::thread>(&TempMonitor::svc_, this);

    return true;
} // Start

void TempMonitor::WaitForTermination() {
    // If we are not terminating, we terminate.
    if (!m_terminate) {
        Terminate();
    }

    if (m_monitorThread.get() == nullptr) {
        std::cerr << "TempMonitor::WaitForTermination(): No thread active" << std::endl;

        return;
    }

    m_monitorThread->join();

    m_monitorThread.reset();
} // WaitForTermination

void TempMonitor::Terminate() {
    m_terminate = true;
} // Terminate

void TempMonitor::svc_() {
    devices::dht22 dht;

    if (!dht.Create()) {
        std::cerr << "TempMonitor::svc_(): Failed to initialize DHT22" << std::endl;
        return;
    }

    std::chrono::steady_clock::time_point nextCheck = std::chrono::steady_clock::now();

    while (!m_terminate) {
        std::this_thread::sleep_until(nextCheck);

        if (!dht.Read()) {
            std::cerr << "TempMonitor::svc_(): Failed to read DHT22" << std::endl;
            continue;
        }

        nextCheck = std::chrono::steady_clock::now() + m_refreshInterval;

        m_temperature = dht.getTemperature();
        m_humidity = dht.getHumidity();
    }
} // svc_
