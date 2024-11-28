//
// Created by Bradley Remedios on 11/28/24.
//
#ifndef PINODE_HEATERCONTROL_HW_STUB_H
#define PINODE_HEATERCONTROL_HW_STUB_H

#include <pinode/HeaterControlHw.h>

namespace pinode {
    class HeaterControlHwStub : public HeaterControlHw {
    public:
        HeaterControlHwStub() = default;
        ~HeaterControlHwStub() override = default;

        void On() override;
        void Off() override;
    }; // class HeaterControlHwStub
}; // namespace pinode

#endif // PINODE_HEATERCONTROL_HW_STUB_H