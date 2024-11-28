//
// Created by Bradley Remedios on 11/28/24.
//

#ifndef PINODE_HEATERCONTROL_HW_H
#define PINODE_HEATERCONTROL_HW_H

#include <memory>

namespace pinode {
    class HeaterControlHw{
    public:
        HeaterControlHw() = default;
        virtual ~HeaterControlHw() = default;

        virtual void On() = 0;
        virtual void Off() = 0;
    }; // class HeaterControlHw

    typedef std::shared_ptr<HeaterControlHw> HeaterControlHwPtr;
}; // pinode

#endif //PINODE_HEATERCONTROL_HW_H