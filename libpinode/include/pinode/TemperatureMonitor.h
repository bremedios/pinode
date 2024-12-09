//
// Created by Bradley Remedios on 11/24/24.
//
#ifndef PINODE_TEMPERATURE_MONITOR_H_
#define PINODE_TEMPERATURE_MONITOR_H_

#include <chrono>
#include <memory>
#include <thread>

namespace pinode {
    class TemperatureMonitor {
    public:
        TemperatureMonitor()=default;
        ~TemperatureMonitor();

        [[nodiscard]] bool Start();

        void WaitForTermination();
        void Terminate();

        float getTemperature() { return m_temperature; }
        float getHumidity() { return m_humidity; }
        void setRefreshInterval(const std::chrono::milliseconds& ms) { m_refreshInterval = std::min(std::chrono::milliseconds(2000), ms);};
    private:
        void svc_();

        bool                             m_terminate = true;
        float                            m_temperature=0.0f;
        float                            m_humidity=0.0f;
        std::shared_ptr<std::thread>     m_monitorThread;
        std::chrono::milliseconds        m_refreshInterval = std::chrono::milliseconds(5000);
    }; // TemperatureMonitor

    typedef std::shared_ptr<TemperatureMonitor> TemperatureMonitorPtr;
}; // pinode

#endif //PINODE_TEMPERATURE_MONITOR_H_