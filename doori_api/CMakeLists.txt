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
		CommunicationMember/Addr.h
		Process/Application.h DataStructure/Branch.h
		DataStructure/Branch.hpp
#		CommunicationMember/Connection.h
		DataStream/Data.h
		DataStream/DataSegment.h
		DataStream/DataSegment.hpp
		DataStream/Dictionary.h
		CommunicationMember/Endpoint.h
		CommunicationMember/Epoll.h
		CommunicationMember/EpollEvents.h
		CommunicationMember/TCP.h
		Common/Error.h
		Process/Getopt.h
		TnsdDistrict/ICommunication.h
		Common/Log.h
		Common/Log.hpp
		TnsdDistrict/Protocol.h
		TnsdDistrict/Publisher.h
		Etc/Regex.h
		Process/Runner.h
		DataStream/Stream.h
		TnsdDistrict/Subscriber.h
		TnsdDistrict/Topic.h
        DataStructure/Tree.h
		DataStructure/Tree.hpp
		DataStream/Json.h
		CommunicationMember/IIPCBuilder.h
		CommunicationMember/TCPBuilder.h
        CommunicationMember/ITopology.h
		CommunicationMember/TCPTopology.h
		Etc/Bigdecimal.h
        DooriTest.h)

set(SRC_FILES
		CommunicationMember/Addr.cpp
		Process/Application.cpp
#		CommunicationMember/Connection.cpp
		DataStream/Data.cpp
		DataStream/DataSegment.cpp
		DataStream/Dictionary.cpp
		CommunicationMember/Endpoint.cpp
		CommunicationMember/Epoll.cpp
		CommunicationMember/EpollEvents.cpp
		Common/Error.cpp
		Process/Getopt.cpp
		TnsdDistrict/ICommunication.cpp
		Common/Log.cpp
		TnsdDistrict/Protocol.cpp
		TnsdDistrict/Publisher.cpp
		Etc/Regex.cpp
		Process/Runner.cpp
		DataStream/Stream.cpp
		TnsdDistrict/Subscriber.cpp
		TnsdDistrict/Topic.cpp
		DataStream/Json.cpp
		Etc/Bigdecimal.cpp
		DooriTest.h
		CommunicationMember/TCPBuilder.cpp
		CommunicationMember/TCP.cpp
		CommunicationMember/TCPTopology.cpp CommunicationMember/TcpApi.cpp CommunicationMember/TcpApi.h)

#add_library(${DOORI_API_LIBRARY_NAME} SHARED ${HEADER_FILES} ${SRC_FILES}) # shared library
add_library(${DOORI_API_LIBRARY_NAME} STATIC ${HEADER_FILES} ${SRC_FILES}) # static library

set_target_properties(${DOORI_API_LIBRARY_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

install(TARGETS ${DOORI_API_LIBRARY_NAME}
        LIBRARY DESTINATION "${DOORI_PROJECT}/lib"
        ARCHIVE DESTINATION "${DOORI_PROJECT}/lib"
        PUBLIC_HEADER DESTINATION "${DOORI_PROJECT}/inc"
        )
