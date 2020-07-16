cmake_minimum_required(VERSION 3.15)

project(doori_sample)

set(CMAKE_CXX_STANDARD 17)

set(DOORI_API_LIBRARY_NAME doori)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} -std=c++17")

link_libraries (-lpthread ${DOORI_API_LIBRARY_NAME})

add_executable(ping
        ping/main.cpp
        )

add_executable(pong
        pong/main.cpp
        )

install(TARGETS ping pong DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
