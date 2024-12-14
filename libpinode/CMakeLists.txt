cmake_minimum_required(VERSION 3.25)

project(pinode)

set(CMAKE_CXX_STANDARD 20)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

include_directories(include ../../libbpl_net/libbpl_net/include ../../libbpl_sys/libbpl_sys/include ../../libdevices_dht22/libdht22/include ../../libbpl_storage/libbpl_storage/include)

set(PUBLIC_HEADER include/pinode/PacketOpGetTemperature.h
        include/pinode/PacketOps.h
        include/pinode/Server.h
        include/pinode/TemperatureMonitor.h
        include/pinode/Client.h
        include/pinode/HeaterControlHw.h
        include/pinode/HeaterControlHwRpi.h
        include/pinode/HeaterControlHwStub.h
        include/pinode/HeaterController.h)
set(HEADER_FILES src/Debug.h)
set(CORE_SOURCE_FILES src/Server.cpp
        src/PacketOpGetTemperature.cpp
        src/PacketOpGetHumidity.cpp
        src/PacketOpGetSensorInfo.cpp
        src/TemperatureMonitor.cpp
        src/Client.cpp
        src/HeaterControlHwStub.cpp
        src/HeaterController.cpp
        src/HeaterControlFactory.cpp
        src/HeaterStatus.cpp
        src/PacketOpGetHeaterStatus.cpp
        src/OverrideTemperature.cpp
        src/PacketOpOverrideTemperature.cpp)

if(EXISTS "/usr/local/include/wiringPi.h")
    set(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS} -DENABLE_HW_RPI)
    set(SOURCE_FILES ${CORE_SOURCE_FILES} src/HeaterControlHwRpi.cpp)
else()
    set(SOURCE_FILES ${CORE_SOURCE_FILES})
endif()

add_library(pinode SHARED ${SOURCE_FILES} ${PUBLIC_HEADER})

if(EXISTS "/usr/local/lib/libwiringPi.so")
    target_link_libraries(pinode PRIVATE bpl_net devices_dht22 bpl_sys bpl_storage wiringPi)
else()
    target_link_libraries(pinode PRIVATE bpl_net devices_dht22 bpl_sys bpl_storage)
endif()

install(TARGETS pinode DESTINATION lib)
install(FILES include/pinode/PacketOpGetTemperature.h DESTINATION include/pinode)
install(FILES include/pinode/PacketOps.h DESTINATION include/pinode)
install(FILES include/pinode/Server.h DESTINATION include/pinode)
install(FILES include/pinode/HeaterControlHw.h DESTINATION include/pinode)
install(FILES include/pinode/HeaterControlHwStub.h DESTINATION include/pinode)
install(FILES include/pinode/HeaterController.h DESTINATION include/pinode)

if(EXISTS "/usr/local/include/wiringPi.h")
    install(FILES include/pinode/HeaterControlHwRpi.h DESTINATION include/pinode)
endif()