//
// Created by Bradley Remedios on 11/28/24.
//

#ifndef HEATERCONTROLLER_H
#define HEATERCONTROLLER_H

#include <pinode/HeaterControlHw.h>
#include <pinode/Client.h>

namespace pinode {
    class HeaterController {
    public:
        HeaterController()=default;
        ~HeaterController()=default;

        [[nodiscard]] bool Update();

        void setHeaterControl(HeaterControlHwPtr& heaterControl) { m_heaterControl=heaterControl; }
        void setPinodeClient(ClientPtr& client) { m_client=client; }
        void setThreshold(float threshold) {m_temperatureThreshold = std::abs(threshold);}
        void setTemperature(float temperature) {m_temperature = temperature;}

    private:
        ClientPtr          m_client;
        HeaterControlHwPtr m_heaterControl;
        float              m_temperature=19.5;
        float              m_temperatureThreshold=1;
    }; // class HeaterController
}; // pinode

#endif //HEATERCONTROLLER_H