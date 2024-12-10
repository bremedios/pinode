//
// Created by Bradley Remedios on 12/10/24.
//
#include <pinode/HeaterControlFactory.h>

#if defined(ENABLE_HW_RPI)
    #include "pinode/HeaterControlHwRpi.h"
#else
    #include "pinode/HeaterControlHwStub.h"
#endif

namespace pinode{
    pinode::HeaterControlHwPtr HeaterControlFactory::getHeaterControl() {

#if defined(ENABLE_HW_RPI)
        pinode::HeaterControlHwRpi* controlHwRpi  = new pinode::HeaterControlHwRpi();
        pinode::HeaterControlHwPtr  controlPtr    = pinode::HeaterControlHwPtr(controlHwRpi);
#else
        pinode::HeaterControlHwStub*   controlHwStub = new pinode::HeaterControlHwStub();
        pinode::HeaterControlHwPtr     controlPtr    = pinode::HeaterControlHwPtr(controlHwStub);
#endif

        return controlPtr;
    } // getHeaterControl
}; // pinode