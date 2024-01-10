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
		Process/Getopt.h
		Process/Runner.h
		Process/Application.h
		DataStructure/Tree.h
		DataStructure/Tree.hpp
		DataStructure/Branch.h
		DataStructure/Branch.hpp
		DataStructure/ILeafKey.h
		Data/Json.h
        Data/Fid.h
		Data/FidSegment.h
		Data/FidSegment.hpp
		Data/Dictionary.h
		Common/Error.h
		Common/Log.h
		Common/Log.hpp
		Etc/Regex.h
		Etc/Bigdecimal.h
        Tnsd/Topic.h
		Communication/TCP.h
		Communication/IIPCBuilder.h
		Communication/TCPBuilder.h
        Communication/ITopologyNode.h
		Communication/TCP/TCPTopologyNode.cpp
		Communication/TCP/TCPTopologyNode.h
)

set(SRC_FILES
		Process/Application.cpp
#		Communication/Connection.cpp
		Data/Fid.cpp
		Data/FidSegment.cpp
		Data/Dictionary.cpp
		Common/Error.cpp
		Common/Util.h
		Common/Log.cpp
		Process/Getopt.cpp
		Process/Runner.cpp
        Tnsd/Topic.cpp
		Data/Json.cpp
		Etc/Regex.cpp
		Etc/Bigdecimal.cpp
		Communication/TCPBuilder.cpp
		Communication/TCP.cpp
		Communication/TcpApi.cpp
		Communication/TcpApi.h
		Communication/Socket.cpp
		Communication/Socket.h
		Communication/EpollApi.cpp
		Communication/EpollApi.h
		Stream/StreamTemplate.h
		Stream/IHeader.h
		Stream/IBody.h
		Stream/IStream.h
		Tnsd/Body.hpp
		Tnsd/Body.h
		Tnsd/Header.cpp
		Tnsd/Header.h
		Tnsd/NodeInfo.hpp
		Tnsd/NodeInfo.h
		Tnsd/Header.cpp
		Tnsd/Header.h
        Data/Yaml.cpp Data/Yaml.h
		Communication/TCP/ITCPState.h
		Communication/TCP/TCPTopologyNode.cpp
		Communication/TCP/TCPTopologyNode.h
		Communication/TCP/TCPOpen.cpp
		Communication/TCP/TCPOpen.h
		Communication/TCP/TCPEstablish.cpp
		Communication/TCP/TCPEstablish.h
		Communication/TCP/TCPClose.cpp
		Communication/TCP/TCPClose.h
		Communication/TCP/TCPWait.cpp
		Communication/TCP/TCPWait.h
		Communication/TCP/TCPConnection.cpp
		Communication/TCP/TCPConnection.h
		Communication/Handler.cpp
		Communication/Handler.h
		Communication/Handler.h
		Communication/INode.h)

#add_library(${DOORI_API_LIBRARY_NAME} SHARED ${HEADER_FILES} ${SRC_FILES}) # shared library
add_library(${DOORI_API_LIBRARY_NAME} STATIC ${HEADER_FILES} ${SRC_FILES}) # static library

set_target_properties(${DOORI_API_LIBRARY_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

install(TARGETS ${DOORI_API_LIBRARY_NAME}
        LIBRARY DESTINATION "${DOORI_PROJECT}/lib"
        ARCHIVE DESTINATION "${DOORI_PROJECT}/lib"
        PUBLIC_HEADER DESTINATION "${DOORI_PROJECT}/inc"
        )
