cmake_minimum_required(VERSION 3.25)

project(piheater)

set(CMAKE_CXX_STANDARD 20)

include_directories(../libpinode/include ../../libbpl_net/libbpl_net/include ../../libbpl_sys/libbpl_sys/include)

if(EXISTS "/usr/local/include/wiringPi.h")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -DENABLE_HW_RPI)
endif()

add_executable(piheater src/main.cpp)

if(EXISTS "/usr/local/lib/libwiringPi.so")
    target_link_libraries(piheater PRIVATE pinode bpl_net bpl_sys wiringPi)
else()
    target_link_libraries(piheater PRIVATE pinode bpl_net bpl_sys)
endif()

install(TARGETS piheater DESTINATION bin)