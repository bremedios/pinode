cmake_minimum_required(VERSION 3.25)

project(pinode_client)

set(CMAKE_CXX_STANDARD 20)

include_directories(../libpinode/include ../../libbpl_sys/libbpl_sys/include ../../libbpl_storage/libbpl_storage/include)

add_executable(pinode_client main.cpp)

target_link_libraries(pinode_client PRIVATE pinode bpl_net bpl_sys)
install(TARGETS pinode_client DESTINATION bin)