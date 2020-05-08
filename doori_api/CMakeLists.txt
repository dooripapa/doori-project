cmake_minimum_required(VERSION 3.15)

project(doori_api)

set(CMAKE_CXX_STANDARD 17)

#set(DOORI_PROJECT /home/jaeseong/CLionProjects/doori_project)
set(DOORI_API_LIBRARY_NAME doori)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} -std=c++17 -fPIC")

link_libraries (-lpthread)

include_directories(SYSTEM .)

set(HEADER_FILES
        Addr.h
        Application.h
        Branch.h
        Branch.hpp
        Connection.h
        Data.h
        DataSegment.h
        DataSegment.hpp
        Dictionary.h
        Endpoint.h
        Epoll.h
        EpollEvents.h
        Error.h
        Getopt.h
        ICommunication.h
        Log.h
        Log.hpp
        Protocol.h
        Publisher.h
        Regex.h
        Runner.h
        Stream.h
        Subscriber.h
        Topic.h
        Tree.h
        Tree.hpp json.cpp json.h)

set(SRC_FILES
        Addr.cpp
        Application.cpp
        Connection.cpp
        Data.cpp
        DataSegment.cpp
        Dictionary.cpp
        Endpoint.cpp
        Epoll.cpp
        EpollEvents.cpp
        Error.cpp
        Getopt.cpp
        ICommunication.cpp
        Log.cpp
        Protocol.cpp
        Publisher.cpp
        Regex.cpp
        Runner.cpp
        Stream.cpp
        Subscriber.cpp
        Topic.cpp json.cpp json.h)

add_library(${DOORI_API_LIBRARY_NAME} SHARED ${HEADER_FILES} ${SRC_FILES} )

set_target_properties(${DOORI_API_LIBRARY_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

install(TARGETS "${DOORI_API_LIBRARY_NAME}"
        LIBRARY DESTINATION "${DOORI_PROJECT}/lib"
        PUBLIC_HEADER DESTINATION "${DOORI_PROJECT}/inc"
        )
