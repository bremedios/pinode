//
// Created by Bradley Remedios on 11/28/24.
//
#include "pinode/HeaterController.h"
#include <bpl/sys/Tick.h>

#if defined(ENABLE_HW_RPI)
    #include "pinode/HeaterControlHwRpi.h"
#else
    #include "pinode/HeaterControlHwStub.h"
#endif

int main () {
    bpl::sys::Tick tick(std::chrono::milliseconds(1000));
    pinode::HeaterController    controller;
    pinode::ClientPtr           client = std::make_shared<pinode::Client>();

#if defined(ENABLE_HW_RPI)
    pinode::HeaterControlHwRpi* controlHwRpi  = new pinode::HeaterControlHwRpi();
    pinode::HeaterControlHwPtr  controlPtr    = pinode::HeaterControlHwPtr(controlHwRpi);
#else
    pinode::HeaterControlHwStub*   controlHwStub = new pinode::HeaterControlHwStub();
    pinode::HeaterControlHwPtr      controlPtr    = pinode::HeaterControlHwPtr(controlHwStub);
#endif

    if (!client->Connect("127.0.0.1", 9999))
    {
        std::cerr << "    ERROR: Failed to connect to pinode temperature sensor node" << std::endl;
        return -1;
    }

    if (!client->Start()) {
        std::cerr << "    ERROR: Failed to start to pinode sensor client" << std::endl;
        return -1;
    }

    controller.setHeaterControl(controlPtr);
    controller.setPinodeClient(client);
    controller.setThreshold(0.5f);
    controller.setTemperature(20.0f);

    while (true) {
        if(!controller.Update()) {
            std::cerr << "    ERROR: Failed to update heater control" << std::endl;
        }

        tick.Wait();
    }

    return 0;
} // main