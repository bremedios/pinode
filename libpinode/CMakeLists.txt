cmake_minimum_required(VERSION 3.25)

project(pinode)

set(CMAKE_CXX_STANDARD 20)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

include_directories(include)

set(PUBLIC_HEADER include/pinode/PacketOpGetTemperature.h
        include/pinode/PacketOps.h
        include/pinode/Server.h
        include/pinode/TemperatureMonitor.h
        include/pinode/Client.h)
set(HEADER_FILES src/Debug.h)
set(SOURCE_FILES src/Server.cpp src/PacketOpGetTemperature.cpp src/TemperatureMonitor.cpp src/Client.cpp)

add_library(pinode SHARED ${SOURCE_FILES} ${PUBLIC_HEADER} )
target_link_libraries(pinode PRIVATE bpl_net devices_dht22 bpl_sys)

install(TARGETS pinode DESTINATION lib)
install(FILES include/pinode/PacketOpGetTemperature.h DESTINATION include/pinode)
install(FILES include/pinode/PacketOps.h DESTINATION include/pinode)
install(FILES include/pinode/Server.h DESTINATION include/pinode)