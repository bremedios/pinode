cmake_minimum_required(VERSION 3.25)

project(pitempnode)

add_executable(pitempnode src/main.cpp src/TempMonitor.cpp)

#target_link_libraries(pitempnode PRIVATE dht22 wiringPi)
target_link_libraries(pitempnode PRIVATE dht22)
