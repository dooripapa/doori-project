cmake_minimum_required(VERSION 3.15)

project(doori_service)

set(CMAKE_CXX_STANDARD 17)

set(DOORI_API_LIBRARY_NAME doori)

if(NOT "${DOORI_PROJECT}")
    set(DOORI_PROJECT ${CMAKE_CURRENT_SOURCE_DIR}/../)
endif()

set(CMAKE_CXX_FLAGS -pthread)

include_directories(SYSTEM ${DOORI_PROJECT}/doori_api .)

link_directories(SYSTEM ${DOORI_PROJECT}/doori_api)

link_libraries (${DOORI_API_LIBRARY_NAME})


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
