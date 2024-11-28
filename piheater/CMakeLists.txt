cmake_minimum_required(VERSION 3.25)

project(piheater)

set(CMAKE_CXX_STANDARD 20)

include_directories(../libpinode/include)

add_executable(piheater src/main.cpp)

if(EXISTS "/usr/local/lib/libwiringPi.so")
    target_link_libraries(piheater PRIVATE pinode bpl_sys wiringPi)
else()
    target_link_libraries(piheater PRIVATE pinode bpl_net bpl_sys)
endif()