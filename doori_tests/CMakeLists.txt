cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

#link_libraries (-lpthread)

include_directories(${DOORI_PROJECT}/doori_api)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/googletest/lib)

set(TEST_FILES
		../doori_api/Common/Log.h
		../doori_api/Common/Log.hpp
		../doori_api/Common/Log.cpp
		../doori_api/CommunicationMember/Endpoint.h
		../doori_api/CommunicationMember/Endpoint.cpp
		../doori_api/CommunicationMember/TCP.h
		../doori_api/CommunicationMember/TCP.cpp
        ../doori_api/CommunicationMember/TCPTopology.h
        ../doori_api/CommunicationMember/TCPTopology.cpp
		../doori_api/CommunicationMember/TCPBuilder.h
		../doori_api/CommunicationMember/TCPBuilder.cpp
		../doori_api/CommunicationMember/TcpApi.cpp
		../doori_api/CommunicationMember/TcpApi.h
        ../doori_api/DataStream/Json.cpp
        ../doori_api/DataStream/Json.h
        ../doori_api/Etc/Bigdecimal.cpp
        ../doori_api/Etc/Bigdecimal.h
    test_json.cpp
    test_bigdecimal.cpp test_branch.cpp test_CommunicationMember.cpp ../doori_api/CommunicationMember/IIPC.h)

add_executable(unittest test_main.cpp ${TEST_FILES} )

target_link_libraries(unittest gtest gtest_main pthread)

#target_link_libraries(unittest gmock gmock_main)

