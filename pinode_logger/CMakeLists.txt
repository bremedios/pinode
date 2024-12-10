cmake_minimum_required(VERSION 3.25)

project(pinode_logger)

set(CMAKE_CXX_STANDARD 20)

include_directories(../libpinode/include ../../libbpl_sys/libbpl_sys/include ../../libbpl_storage/libbpl_storage/include ../../libbpl_net/libbpl_net/include)

add_executable(pinode_logger main.cpp)

target_link_libraries(pinode_logger PRIVATE pinode bpl_net bpl_sys fmt)
install(TARGETS pinode_logger DESTINATION bin)