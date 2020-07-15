cmake_minimum_required(VERSION 3.15)

project(doori_sample)

set(CMAKE_CXX_STANDARD 17)

set(DOORI_API_LIBRARY_NAME doori)

link_libraries (-lpthread ${DOORI_API_LIBRARY_NAME})

include_directories(${DOORI_PROJECT}/doori_api)

add_executable( ping ping/main.cpp )
add_executable( pong pong/main.cpp )

#target_link_libraries(unittest gtest gtest_main pthread)
