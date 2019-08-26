cmake_minimum_required(VERSION 3.4.1)
project(hwvc_native LANGUAGES C CXX)
set(HWVC_PATH ${CMAKE_SOURCE_DIR}/../../../src)
include_directories(
        ${HWVC_PATH}/common/include
        ${HWVC_PATH}/codec/include
        ${HWVC_PATH}/render/include
)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -O3 -Wall -ffast-math -funroll-loops")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -ffast-math -funroll-loops")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -O0 -Wall -g -ggdb ")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -g -ggdb ")
endif ()
message(STATUS "hwnative CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

file(
        GLOB
        SOURCES
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/filter/*.cpp
        ${PROJECT_SOURCE_DIR}/processor/*.cpp
        ${PROJECT_SOURCE_DIR}/utils/*.cpp
        ${PROJECT_SOURCE_DIR}/media/*.cpp
        ${PROJECT_SOURCE_DIR}/entity/*.cpp
        ${PROJECT_SOURCE_DIR}/test/*.cpp
)
add_library( # Sets the name of the library.
        hwvc_native

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${SOURCES}
        )

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

target_link_libraries( # Specifies the target library.
        hwvc_native
        hwvcom
        hwvc_codec
        hwvc_render
        android
        ${log-lib})