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
		Process/Application.h DataStructure/Branch.h
		DataStructure/Branch.hpp
#		CommunicationMember/Connection.h
		Data/Data.h
		Data/DataSegment.h
		Data/DataSegment.hpp
		Data/Dictionary.h
		CommunicationMember/TCP.h
		Common/Error.h
		Process/Getopt.h
		Tnsd/MiddleSide.h
		Common/Log.h
		Common/Log.hpp
		Tnsd/PubSide.h
		Etc/Regex.h
		Process/Runner.h
		Data/Stream.h
		Tnsd/SubSide.h
        Tnsd/Topic.h
        DataStructure/Tree.h
		DataStructure/Tree.hpp
		Data/Json.h
		CommunicationMember/IIPCBuilder.h
		CommunicationMember/TCPBuilder.h
		CommunicationMember/IIPCTopology.h
		Etc/Bigdecimal.h
        )

set(SRC_FILES
		Process/Application.cpp
#		CommunicationMember/Connection.cpp
		Data/Data.cpp
		Data/DataSegment.cpp
		Data/Dictionary.cpp
		Common/Error.cpp
		Process/Getopt.cpp
		Tnsd/MiddleSide.cpp
		Common/Log.cpp
		Tnsd/PubSide.cpp
		Etc/Regex.cpp
		Process/Runner.cpp
		Data/Stream.cpp
		Tnsd/SubSide.cpp
        Tnsd/Topic.cpp
		Data/Json.cpp
		Etc/Bigdecimal.cpp
		CommunicationMember/TCPBuilder.cpp
		CommunicationMember/TCP.cpp
		CommunicationMember/TcpApi.cpp
		CommunicationMember/TcpApi.h
		Stream/Protocol.h
		Stream/Protocol.cpp
		Stream/ProtocolHeader.h
		Stream/ProtocolHeader.cpp
        CommunicationMember/Socket.cpp
		CommunicationMember/Socket.h
		CommunicationMember/EpollApi.cpp CommunicationMember/EpollApi.h)

#add_library(${DOORI_API_LIBRARY_NAME} SHARED ${HEADER_FILES} ${SRC_FILES}) # shared library
add_library(${DOORI_API_LIBRARY_NAME} STATIC ${HEADER_FILES} ${SRC_FILES}) # static library

set_target_properties(${DOORI_API_LIBRARY_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

install(TARGETS ${DOORI_API_LIBRARY_NAME}
        LIBRARY DESTINATION "${DOORI_PROJECT}/lib"
        ARCHIVE DESTINATION "${DOORI_PROJECT}/lib"
        PUBLIC_HEADER DESTINATION "${DOORI_PROJECT}/inc"
        )
