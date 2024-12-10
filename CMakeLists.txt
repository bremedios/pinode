cmake_minimum_required(VERSION 3.25)

project(pinode_proj)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(libpinode)
add_subdirectory(pinode_tempsensor)
add_subdirectory(pinode_client)
add_subdirectory(piheater)
add_subdirectory(pinode_logger)