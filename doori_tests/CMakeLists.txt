cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

#link_libraries (-lpthread)

include_directories(${DOORI_PROJECT}/doori_api)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/googletest/lib)

set(TEST_FILES
    ${PROJECT_SOURCE_DIR}/doori_api/Json.cpp
    ${PROJECT_SOURCE_DIR}/doori_api/Json.h
    test_json.cpp
    )

add_executable(unittest test_main.cpp ${TEST_FILES} )

target_link_libraries(unittest gtest gtest_main pthread)

#target_link_libraries(unittest gmock gmock_main)

