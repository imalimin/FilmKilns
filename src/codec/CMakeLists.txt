cmake_minimum_required(VERSION 3.4.1)
project(hwvc_codec LANGUAGES C CXX)
set(PREBUILD_SHARED "${CMAKE_CURRENT_SOURCE_DIR}/prebuild/shared")
set(FFMPEG ${PREBUILD_SHARED}/ffmpeg)
set(YUV ${PREBUILD_SHARED}/yuv)
if (ANDROID)
    include_directories(
            # common
            ${PROJECT_SOURCE_DIR}/../common/include
            ${CMAKE_CURRENT_SOURCE_DIR}/include/x264
            ${CMAKE_CURRENT_SOURCE_DIR}/include/ff
            ${PROJECT_SOURCE_DIR}/../render/include
            ${YUV}/include
            # android
            ${CMAKE_CURRENT_SOURCE_DIR}/platform/android/encoder
    )
else ()
    include_directories(
            ${PROJECT_SOURCE_DIR}/../common/include
            ${CMAKE_CURRENT_SOURCE_DIR}/include/x264
            ${CMAKE_CURRENT_SOURCE_DIR}/include/ff
            ${PROJECT_SOURCE_DIR}/../render/include
            ${YUV}/include
    )
endif ()

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
        hwffmpeg
        SHARED
        IMPORTED)
set_target_properties(
        hwffmpeg
        PROPERTIES IMPORTED_LOCATION
        "${FFMPEG}/libs/${ANDROID_ABI}/libhwffmpeg.so")

add_library(
        yuv
        SHARED
        IMPORTED)
set_target_properties(
        yuv
        PROPERTIES IMPORTED_LOCATION
        "${YUV}/libs/${ANDROID_ABI}/libyuv.so")

#------------------SHARED END----------------------

if (ANDROID)
    file(
            GLOB
            SOURCES
            # common
            ${PROJECT_SOURCE_DIR}/*cpp
            ${PROJECT_SOURCE_DIR}/decoder/*.cpp
            ${PROJECT_SOURCE_DIR}/encoder/*.cpp
            ${PROJECT_SOURCE_DIR}/entity/*.cpp
            ${PROJECT_SOURCE_DIR}/utils/*.cpp
            ${PROJECT_SOURCE_DIR}/sources/*.cpp
            ${PROJECT_SOURCE_DIR}/processor/*.cpp
            ${PROJECT_SOURCE_DIR}/units/*.cpp
            ${PROJECT_SOURCE_DIR}/translator/*.cpp
            ${PROJECT_SOURCE_DIR}/muxer/*.cpp
            ${PROJECT_SOURCE_DIR}/core/*.cpp
            # android
            ${PROJECT_SOURCE_DIR}/platform/android/decoder/*.cpp
            ${PROJECT_SOURCE_DIR}/platform/android/encoder/*.cpp
            ${PROJECT_SOURCE_DIR}/platform/android/core/*.cpp
    )
else ()
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
            ${PROJECT_SOURCE_DIR}/muxer/*.cpp
            ${PROJECT_SOURCE_DIR}/core/*.cpp
    )
endif ()

add_library( # Sets the name of the library.
        hwvc_codec

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${SOURCES}
        )

if (ANDROID)
    find_library( # Sets the name of the path variable.
            log-lib

            # Specifies the name of the NDK library that
            # you want CMake to locate.
            log)

    target_link_libraries( # Specifies the target library.
            hwvc_codec
            hwvcom
            hwvc_render
            hwffmpeg
            yuv
            mediandk
            OpenMAXAL
            # Links the target library to the log library
            # included in the NDK.
            ${log-lib})
else ()
    target_link_libraries( # Specifies the target library.
            hwvc_codec
            hwvcom
            hwvc_render
            hwffmpeg
            yuv)
endif ()