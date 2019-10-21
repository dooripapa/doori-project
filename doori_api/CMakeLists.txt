cmake_minimum_required(VERSION 3.15)

project(doori-api)

set(CMAKE_CXX_STANDARD 17)

set(LIB_NAME doori)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} -std=c++17 -fPIC")

link_libraries (${LIB_NAME} -lpthread)

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
        Tree.hpp)

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
        Error.h
        Getopt.cpp
        ICommunication.cpp
        Log.cpp
        Protocol.cpp
        Publisher.cpp
        Regex.cpp
        Runner.cpp
        Stream.cpp
        Subscriber.cpp
        Topic.cpp)

link_libraries (-lpthread)

add_library(${LIB_NAME} SHARED ${HEADER_FILES} ${SRC_FILES} )

set_target_properties(${LIB_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

set(MY_INSTALL_LIB_PATH /home/doori/doori/lib)
set(MY_INSTALL_HEADER_PATH /home/doori/doori/inc)

install(TARGETS ${LIB_NAME}
        LIBRARY DESTINATION ${MY_INSTALL_LIB_PATH}
        PUBLIC_HEADER DESTINATION ${MY_INSTALL_HEADER_PATH}
        )
