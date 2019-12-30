cmake_minimum_required(VERSION 3.4.1)
project(hwvcom LANGUAGES C CXX)

option(HWDEBUG "Whether show opengl logs." ON)
if (HWDEBUG)
    add_definitions(-DHWDEBUG)
    message(STATUS "Show debug logs。")
endif ()

set(PREBUILD_STATIC ${PROJECT_SOURCE_DIR}/prebuild/static)
set(JPGE ${PREBUILD_STATIC}/libturbojpeg/libs/${ANDROID_ABI})
set(PNG ${PREBUILD_STATIC}/libpng/libs/${ANDROID_ABI})

# 引入cpufeatures模块
include_directories(
        ${ANDROID_NDK}/sources/android/cpufeatures
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
)

#include_directories(
#        ${PREBUILD_STATIC}/libturbojpeg/include
#        ${PREBUILD_STATIC}/libpng/include
#)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall")
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math -funroll-loops ")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} O3 -ffast-math -funroll-loops ")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb ")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -ggdb ")
endif ()

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
message(STATUS "al_common CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

file(
        GLOB
        SOURCES
        ${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/entity/*.cpp
        ${PROJECT_SOURCE_DIR}/thread/*.cpp
        ${PROJECT_SOURCE_DIR}/utils/*.cpp
        ${PROJECT_SOURCE_DIR}/media/*.cpp
        ${PROJECT_SOURCE_DIR}/media/sles/*.cpp
        ${PROJECT_SOURCE_DIR}/sources/*.cpp
        ${PROJECT_SOURCE_DIR}/math/*.cpp
        ${PROJECT_SOURCE_DIR}/doc/*.cpp
        ${PROJECT_SOURCE_DIR}/doc/tinyxml/*.cpp
        #Header
        ${PROJECT_SOURCE_DIR}/include/*.h
)
if (${ANDROID_ABI} STREQUAL "armeabi-v7a")
    set_property(SOURCE ${SOURCES} APPEND_STRING PROPERTY COMPILE_FLAGS " -mfpu=neon")
    add_definitions("-DHAVE_NEON=1")
elseif (${ANDROID_ABI} STREQUAL "x86")
    set_property(SOURCE ${SOURCES} APPEND_STRING PROPERTY COMPILE_FLAGS
            " -mssse3  -Wno-unknown-attributes \
                   -Wno-deprecated-declarations \
                   -Wno-constant-conversion \
                   -Wno-static-in-inline")
    add_definitions(-DHAVE_NEON_X86=1 -DHAVE_NEON=1)
endif ()
add_library(
        hwvcom
        SHARED
        ${SOURCES}
)

find_library(
        log-lib
        log)

# libpng需要zlib依赖
find_library(
        zlib
        z)

target_link_libraries(
        hwvcom
        OpenSLES
        ${zlib}
        ${log-lib}
)