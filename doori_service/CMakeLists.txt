cmake_minimum_required(VERSION 3.15)

project(doori_service)

set(CMAKE_CXX_STANDARD 17)

set(DOORI_API_LIBRARY_NAME doori)

if(NOT "${DOORI_PROJECT}")
    set(DOORI_PROJECT ${CMAKE_CURRENT_SOURCE_DIR}/../)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} -std=c++17 -fPIC")

link_libraries (-lpthread ${DOORI_API_LIBRARY_NAME})

link_directories(SYSTEM ${DOORI_PROJECT}/doori_api)

include_directories(SYSTEM ${DOORI_PROJECT}/doori_api .)

add_executable(tnsd
        tnsDaemon/main.cpp
        tnsDaemon/tnsd.cpp
        tnsDaemon/tnsd.h
        )

add_executable(pub
        publisher/main.cpp
        publisher/Pub.cpp
        publisher/Pub.h
        )

add_executable(sub
        subscription/main.cpp
        subscription/Sub.cpp
        subscription/Sub.h
        )

install(TARGETS tnsd pub sub DESTINATION "${DOORI_PROJECT}/bin")
