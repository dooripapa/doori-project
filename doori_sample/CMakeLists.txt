cmake_minimum_required(VERSION 3.15)

project(doori_sample)

set(CMAKE_CXX_STANDARD 17)

set(DOORI_API_LIBRARY_NAME doori)

if(NOT "${DOORI_PROJECT}")
    set(DOORI_PROJECT ${CMAKE_CURRENT_SOURCE_DIR}/../)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} -std=c++17")

link_libraries (-lpthread ${DOORI_API_LIBRARY_NAME})

include_directories(${DOORI_PROJECT}/doori_api/ .)

link_directories(SYSTEM ${DOORI_PROJECT}/doori_api/)

add_executable(ping
        ping.cpp
        )

add_executable(pong
        pong.cpp
        )

install(TARGETS ping pong DESTINATION ${DOORI_PROJECT}/bin)
