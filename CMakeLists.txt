cmake_minimum_required(VERSION 3.15)
project(doori_project)

set(CMAKE_CXX_STANDARD 17)

set(DOORI_PROJECT ${CMAKE_CURRENT_SOURCE_DIR})
set(DOORI_API_LIBRARY_NAME doori)

include_directories("${DOORI_PROJECT}/inc")

add_subdirectory("${DOORI_PROJECT}/doori_api")
add_subdirectory("${DOORI_PROJECT}/doori_service")
add_subdirectory("${DOORI_PROJECT}/doori_tests")
