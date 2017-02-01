cmake_minimum_required(VERSION 3.6)
project(Pintos)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_custom_target(Pintos COMMAND make -C ${Pintos_SOURCE_DIR} CLION_EXE_DIR=${PROJECT_BINARY_DIR})

