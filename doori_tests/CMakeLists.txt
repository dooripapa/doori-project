cmake_minimum_required(VERSION 3.15)

project(doori_tests)

set(CMAKE_CXX_STANDARD 17)

link_libraries (-lpthread)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include)
include_directories(${DOORI_PROJECT}/doori_api)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/googletest/googlemock/include)
#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/googletest/lib)
link_directories(/home/jaeseong/CLionProjects/doori_project/doori_tests/googletest/lib)

set(SOURCE_FILES .)

add_executable(unittest main.cpp ${SOURCE_FILES})


#target_link_libraries(unittest gtest gtest_main)
#target_link_libraries(unittest gmock gmock_main)

