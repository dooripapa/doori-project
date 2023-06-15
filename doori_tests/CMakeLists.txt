cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)

#link_libraries (-lpthread)

#include_directories(${DOORI_PROJECT}/doori_api)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/googletest/lib)

set(TEST_FILES
		../doori_api/Common/Log.h
		../doori_api/Common/Log.hpp
		../doori_api/Common/Log.cpp
		../doori_api/Common/Error.h
		../doori_api/Common/Error.cpp
        ../doori_api/Communication/TCP.h
        ../doori_api/Communication/TCP.cpp
        ../doori_api/Communication/TCPBuilder.h
        ../doori_api/Communication/TCPBuilder.cpp
        ../doori_api/Communication/TcpApi.h
        ../doori_api/Communication/TcpApi.cpp
        ../doori_api/Communication/Socket.h
        ../doori_api/Communication/Socket.cpp
        ../doori_api/Communication/EpollApi.h
        ../doori_api/Communication/EpollApi.cpp
		../doori_api/Data/Fid.h
		../doori_api/Data/Fid.cpp
		../doori_api/Data/FidSegment.h
		../doori_api/Data/FidSegment.hpp
		../doori_api/Data/FidSegment.cpp
        ../doori_api/Data/Json.h
		../doori_api/Data/Json.cpp
		../doori_api/Data/Json.hpp
		../doori_api/Tnsd/Header.h
		../doori_api/Tnsd/Header.cpp
		../doori_api/Tnsd/Body.h
		../doori_api/Tnsd/Body.cpp
		../doori_api/Tnsd/NodeInfo.h
		../doori_api/Tnsd/NodeInfo.cpp
		../doori_api/Tnsd/Topic.h
		../doori_api/Tnsd/Topic.cpp
        ../doori_api/Etc/Bigdecimal.cpp
        ../doori_api/Etc/Bigdecimal.h
		test_Json.cpp
#		test_bigdecimal.cpp
#		test_branch.cpp
        test_TCPBuilder.cpp
        test_TcpApi.cpp
		test_EpollApi.cpp
        test_StreamTemplate.cpp test_Bigdecimal.cpp test_Branch.cpp test_Tree.cpp test_Fid.cpp)

add_executable(unittest test_main.cpp ${TEST_FILES} )

target_link_libraries(unittest gtest gtest_main pthread)

#target_link_libraries(unittest gmock gmock_main)

