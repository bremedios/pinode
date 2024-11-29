//
// Created by Bradley Remedios on 11/28/24.
//
#ifndef PINODE_HEATERCONTROL_HW_RPI_H
#define PINODE_HEATERCONTROL_HW_RPI_H

#include <pinode/HeaterControlHw.h>

namespace pinode {
    class HeaterControlHwRpi : public HeaterControlHw {
    public:
        HeaterControlHwRpi();
        ~HeaterControlHwRpi() override=default;

        void On() override;
        void Off() override;
    private:
        int m_pin=0;
    }; // class HeaterControlHwRpi
}; // namespace pinode

#endif // PINODE_HEATERCONTROL_HW_RPI_H
