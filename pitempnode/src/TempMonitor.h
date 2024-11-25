//
// Created by Bradley Remedios on 11/24/24.
//
#ifndef TEMPMONITOR_H
#define TEMPMONITOR_H

#include <chrono>
#include <memory>
#include <thread>

class TempMonitor {
public:
    TempMonitor()=default;
    ~TempMonitor();

    bool Start();

    void WaitForTermination();
    void Terminate();

    float getTemperature() { return m_temperature; }
    float getHumidity() { return m_humidity; }
private:
    void svc_();

    bool                             m_terminate = true;
    float                            m_temperature=0.0f;
    float                            m_humidity=0.0f;
    std::shared_ptr<std::thread>     m_monitorThread;
    std::chrono::milliseconds        m_refreshInterval = std::chrono::milliseconds(5000);
}; // TempMonitor

#endif //TEMPMONITOR_H
