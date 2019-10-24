cmake_minimum_required(VERSION 3.15)

project(doori_service)

set(CMAKE_CXX_STANDARD 17)

set(DOORI_PROJECT /home/doori/doori_project)
set(DOORI_API_LIBRARY_NAME doori)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} -std=c++17 -fPIC")

link_libraries (-lpthread ${DOORI_API_LIBRARY_NAME})

link_directories(SYSTEM ${DOORI_PROJECT}/lib .)

include_directories(SYSTEM ${DOORI_PROJECT}/inc ${DOORI_PROJECT}/lib .)

add_executable(tnsd
        tnsd/main.cpp
        tnsd/tnsd.cpp
        tnsd/tnsd.h
        )

add_executable(pub
        pub/main.cpp
        pub/Pub.cpp
        pub/Pub.h
        )

add_executable(sub
        sub/main.cpp
        sub/Sub.cpp
        sub/Sub.h
        )

install(TARGETS tnsd pub sub DESTINATION "${DOORI_PROJECT}/bin")
