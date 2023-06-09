cmake_minimum_required(VERSION 3.15)

project(doori_api)

set(CMAKE_CXX_STANDARD 20)

if(NOT "${DOORI_PROJECT}")
    set(DOORI_PROJECT "${CMAKE_CURRENT_SOURCE_DIR}/../")
endif()

set(DOORI_API_LIBRARY_NAME doori)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} -std=c++17")

#set(CMAKE_C_FLAGS -lphtread)

#link_libraries (-lpthread)

include_directories(SYSTEM .)

set(HEADER_FILES
		Process/Application.h DataStructure/Branch.h
		DataStructure/Branch.hpp
		Data/Data.h
		Data/DataSegment.h
		Data/DataSegment.hpp
		Data/Dictionary.h
		Communication/TCP.h
		Common/Error.h
		Process/Getopt.h
		Common/Log.h
		Common/Log.hpp
		Etc/Regex.h
		Process/Runner.h
		Data/Stream.h
        Tnsd/Topic.h
        DataStructure/Tree.h
		DataStructure/Tree.hpp
		Data/Json.h
		Communication/IIPCBuilder.h
		Communication/TCPBuilder.h
		Communication/IIPCTopology.h
		Etc/Bigdecimal.h
        )

set(SRC_FILES
		Process/Application.cpp
#		Communication/Connection.cpp
		Data/Data.cpp
		Data/DataSegment.cpp
		Data/Dictionary.cpp
		Common/Error.cpp
		Process/Getopt.cpp
		Common/Log.cpp
		Etc/Regex.cpp
		Process/Runner.cpp
		Data/Stream.cpp
        Tnsd/Topic.cpp
		Data/Json.cpp
		Etc/Bigdecimal.cpp
		Communication/TCPBuilder.cpp
		Communication/TCP.cpp
		Communication/TcpApi.cpp
		Communication/TcpApi.h
		Tnsd/Protocol.h
		Tnsd/Protocol.cpp
		Communication/Socket.cpp
		Communication/Socket.h
		Communication/EpollApi.cpp
		Communication/EpollApi.h
		Stream/StreamTemplate.h
		Stream/IHeader.h
		Stream/IBody.h
		Tnsd/Body.cpp
		Tnsd/Body.h
		Tnsd/Header.cpp Tnsd/Header.h Tnsd/NodeInfo.cpp Tnsd/NodeInfo.h Common/Util.h )

#add_library(${DOORI_API_LIBRARY_NAME} SHARED ${HEADER_FILES} ${SRC_FILES}) # shared library
add_library(${DOORI_API_LIBRARY_NAME} STATIC ${HEADER_FILES} ${SRC_FILES}) # static library

set_target_properties(${DOORI_API_LIBRARY_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

install(TARGETS ${DOORI_API_LIBRARY_NAME}
        LIBRARY DESTINATION "${DOORI_PROJECT}/lib"
        ARCHIVE DESTINATION "${DOORI_PROJECT}/lib"
        PUBLIC_HEADER DESTINATION "${DOORI_PROJECT}/inc"
        )
