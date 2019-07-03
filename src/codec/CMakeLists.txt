cmake_minimum_required(VERSION 3.4.1)
project(hwvc_codec)
set(PREBUILD_STATIC "${CMAKE_CURRENT_SOURCE_DIR}/prebuild/static/${ANDROID_ABI}")
set(PREBUILD_SHARED "${CMAKE_CURRENT_SOURCE_DIR}/prebuild/shared/${ANDROID_ABI}")
include_directories(
        ${PROJECT_SOURCE_DIR}/../common/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/x264
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ff
        ${PROJECT_SOURCE_DIR}/../render/include
)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -O3 -Wall -ffast-math -funroll-loops")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -ffast-math -funroll-loops")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -O0 -Wall -g -ggdb ")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -g -ggdb ")
endif ()
message(STATUS "hwcodec CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
#------------------SHARED START----------------------
#add_library(
#        libx264
#        STATIC
#        IMPORTED)
#set_target_properties(
#        libx264
#        PROPERTIES IMPORTED_LOCATION
#        "${PREBUILD_STATIC}/libx264.a")

add_library(
        libhwffmpeg
        SHARED
        IMPORTED)
set_target_properties(
        libhwffmpeg
        PROPERTIES IMPORTED_LOCATION
        "${PREBUILD_SHARED}/libhwffmpeg.so")

#------------------SHARED END----------------------

file(
        GLOB
        SOURCES
        ${PROJECT_SOURCE_DIR}/*cpp
        ${PROJECT_SOURCE_DIR}/decoder/*.cpp
        ${PROJECT_SOURCE_DIR}/encoder/*.cpp
        ${PROJECT_SOURCE_DIR}/entity/*.cpp
        ${PROJECT_SOURCE_DIR}/utils/*.cpp
        ${PROJECT_SOURCE_DIR}/sources/*.cpp
        ${PROJECT_SOURCE_DIR}/processor/*.cpp
        ${PROJECT_SOURCE_DIR}/units/*.cpp
        ${PROJECT_SOURCE_DIR}/translator/*.cpp
)
add_library( # Sets the name of the library.
        hwvc_codec

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
        hwvc_codec
        hwvcom
        hwvc_render
        libhwffmpeg
        mediandk
        OpenMAXAL
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})