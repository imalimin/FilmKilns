cmake_minimum_required(VERSION 3.4.1)

option(ENABLE_G_TEST "Enable gtest" OFF)

set(SOURCE_CPP_PATH ${CMAKE_SOURCE_DIR}/src/main/cpp)
add_subdirectory(${CMAKE_SOURCE_DIR}/../../../ fk.out)
add_subdirectory(${SOURCE_CPP_PATH}/native)
if(ENABLE_G_TEST)
    message(STATUS "ENABLE_G_TEST")
    add_subdirectory(${CMAKE_SOURCE_DIR}/src/test/cpp)
endif ()