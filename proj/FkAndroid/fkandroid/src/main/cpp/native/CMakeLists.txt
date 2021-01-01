cmake_minimum_required(VERSION 3.4.1)
set(MODULE_NAME "FkAndroid")
project(${MODULE_NAME} LANGUAGES C CXX)

option(ENABLE_G_TEST "Enable gtest" OFF)

set(PROJECT_SOURCE_C_DIR ${CMAKE_SOURCE_DIR}/../../../src)
include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_C_DIR}/FkBase/include
        ${PROJECT_SOURCE_C_DIR}/FkFramework/include
)
if(ENABLE_G_TEST)
    include_directories(
            ${CMAKE_CURRENT_SOURCE_DIR}/prebuild/shared/gtest/include
    )
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall")
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math -funroll-loops ")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -ffast-math -funroll-loops ")
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
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${var}")
    endforeach (var)
endif ()
message(STATUS "al_native CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

set(PREBUILD_SHARED "${CMAKE_CURRENT_SOURCE_DIR}/prebuild/shared")
set(GTEST ${PREBUILD_SHARED}/gtest)
add_library(gtest SHARED IMPORTED)
set_target_properties(gtest PROPERTIES IMPORTED_LOCATION
        "${GTEST}/libs/${ANDROID_ABI}/libgtest.so")
add_library(gtest_main SHARED IMPORTED)
set_target_properties(gtest_main PROPERTIES IMPORTED_LOCATION
        "${GTEST}/libs/${ANDROID_ABI}/libgtest_main.so")

aux_source_directory(${PROJECT_SOURCE_DIR}/utils SOURCES_UTILS)

list(APPEND SOURCES_ROOT ${SOURCES_UTILS})

add_library(${MODULE_NAME} SHARED ${SOURCES_ROOT})

find_library(
        log-lib
        log)

set(MODULES ${MODULE_NAME} al_common FkBase FkFramework ${log-lib})
if(ENABLE_G_TEST)
    message(STATUS "ENABLE_G_TEST")
    list(APPEND MODULES gtest gtest_main)
endif ()
target_link_libraries(${MODULES})