cmake_minimum_required(VERSION 3.15)

project(doori_api)

set(CMAKE_CXX_STANDARD 17)

if(NOT "${DOORI_PROJECT}")
    set(DOORI_PROJECT "${CMAKE_CURRENT_SOURCE_DIR}/../")
endif()

set(DOORI_API_LIBRARY_NAME doori)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} -std=c++17")

#set(CMAKE_C_FLAGS -lphtread)

#link_libraries (-lpthread)

include_directories(SYSTEM .)

set(HEADER_FILES
        Addr.h
        Application.h Branch.h
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
        Tree.hpp
        Json.h)

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
        Topic.cpp
        Json.cpp)

#add_library(${DOORI_API_LIBRARY_NAME} SHARED ${HEADER_FILES} ${SRC_FILES}) # shared library
add_library(${DOORI_API_LIBRARY_NAME} STATIC ${HEADER_FILES} ${SRC_FILES}) # static library

set_target_properties(${DOORI_API_LIBRARY_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

install(TARGETS ${DOORI_API_LIBRARY_NAME}
        LIBRARY DESTINATION "${DOORI_PROJECT}/lib"
        ARCHIVE DESTINATION "${DOORI_PROJECT}/lib"
        PUBLIC_HEADER DESTINATION "${DOORI_PROJECT}/inc"
        )
