//
// Created by Bradley Remedios on 12/10/24.
//

#ifndef PINODE_HEATERCONTROLFACTORY_H
#define PINODE_HEATERCONTROLFACTORY_H

#include <pinode/HeaterControlHw.h>

namespace pinode {
    class HeaterControlFactory {
    public:
        static pinode::HeaterControlHwPtr getHeaterControl();
    }; // HeaterControlFactory
}; // pinode
#endif //PINODE_HEATERCONTROLFACTORY_H