cmake_minimum_required(VERSION 3.25)

project(pinode_tempsensor)

set(CMAKE_CXX_STANDARD 20)

include_directories(../libpinode/include ../../libbpl_net/libbpl_net/include)

add_executable(pinode_tempsensor main.cpp)

target_link_libraries(pinode_tempsensor PRIVATE pinode devices_dht22 bpl_net)

install(TARGETS pinode_tempsensor DESTINATION bin)
install(FILES config/sensor.json DESTINATION etc/pinode/sensor.json)
