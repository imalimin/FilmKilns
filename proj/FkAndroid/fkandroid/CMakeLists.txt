cmake_minimum_required(VERSION 3.4.1)

option(ENABLE_G_TEST "Enable gtest" OFF)
option(OPTIMIZE_CLASS_TYPE "Enable optimize class type." OFF)

if (OPTIMIZE_CLASS_TYPE)
    add_definitions(-D__FK_OPTIMIZE_CLASS_TYPE__)
endif ()

set(SOURCE_CPP_PATH ${CMAKE_SOURCE_DIR}/src/main/cpp)
add_subdirectory(${CMAKE_SOURCE_DIR}/../../../ fk.out)
add_subdirectory(${SOURCE_CPP_PATH}/native)
if(ENABLE_G_TEST)
    message(STATUS "ENABLE_G_TEST")
    add_subdirectory(${CMAKE_SOURCE_DIR}/src/test/cpp)
endif ()