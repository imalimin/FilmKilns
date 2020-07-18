cmake_minimum_required(VERSION 3.4.1)
project(al_codec LANGUAGES C CXX)
set(PREBUILD_SHARED "${CMAKE_CURRENT_SOURCE_DIR}/prebuild/shared")
set(FFMPEG ${PREBUILD_SHARED}/ffmpeg)
set(YUV ${PREBUILD_SHARED}/yuv)

set(INCLUDES
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/../al_common
        ${PROJECT_SOURCE_DIR}/../al_common/include
        ${PROJECT_SOURCE_DIR}/../al_graphic/include
        ${PROJECT_SOURCE_DIR}/../al_image/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/x264
        ${CMAKE_CURRENT_SOURCE_DIR}/prebuild/shared/ffmpeg/include
        ${YUV}/include
        )
if (ANDROID)
    set(INCLUDES
            ${INCLUDES}
            # android
            ${CMAKE_CURRENT_SOURCE_DIR}/platform/android/encoder
            )
endif (ANDROID)
include_directories(${INCLUDES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS_CONFIGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS_CONFIGS}")

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    if (${ENABLE_TINY})
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--exclude-libs,ALL,--gc-sections")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/rules-tiny.txt")
    endif ()
endif ()
#将Warning视为Error
if (${ENABLE_BAN_WARNING})
    foreach (var ${WARNING_CONFIGS})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${var}")
        set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} ${var}")
    endforeach (var)
endif ()
message(STATUS "al_codec CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

#------------------SHARED START----------------------
#add_library(
#        libx264
#        STATIC
#        IMPORTED)
#set_target_properties(
#        libx264
#        PROPERTIES IMPORTED_LOCATION
#        "${PREBUILD_STATIC}/libx264.a")

add_library(hwffmpeg SHARED IMPORTED)
set_target_properties(hwffmpeg PROPERTIES IMPORTED_LOCATION
        "${FFMPEG}/libs/${ANDROID_ABI}/libhwffmpeg.so")

add_library(yuv SHARED IMPORTED)
set_target_properties(yuv PROPERTIES IMPORTED_LOCATION
        "${YUV}/libs/${ANDROID_ABI}/libyuv.so")

#------------------SHARED END----------------------

file(GLOB SOURCES
        ${PROJECT_SOURCE_DIR}/*.cpp
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
        #fftools
        ${PROJECT_SOURCE_DIR}/utils/fftools/*.c
        #Header
#        ${PROJECT_SOURCE_DIR}/include/*.h
        )
if (ANDROID)
    file(GLOB
            SOURCES_ANDROID
            ${PROJECT_SOURCE_DIR}/platform/android/decoder/*.cpp
            ${PROJECT_SOURCE_DIR}/platform/android/encoder/*.cpp
            ${PROJECT_SOURCE_DIR}/platform/android/core/*.cpp
            )
    set(SOURCES
            ${SOURCES}
            ${SOURCES_ANDROID}
            )
endif (ANDROID)

add_library(al_codec SHARED ${SOURCES})

set(MODULES
        al_codec
        al_common
        al_graphic
        al_image
        hwffmpeg
        yuv
        )
if (ANDROID)
    find_library(log-lib log)
    set(MODULES
            ${MODULES}
            mediandk
            OpenMAXAL
            android
            ${log-lib}
            )
endif (ANDROID)
target_link_libraries(${MODULES})