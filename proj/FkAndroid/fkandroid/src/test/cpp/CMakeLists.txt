cmake_minimum_required(VERSION 3.4.1)
set(MODULE_NAME "FkAndroidTest")
project(${MODULE_NAME} LANGUAGES C CXX)

set(PROJECT_SOURCE_FK_DIR ${CMAKE_SOURCE_DIR}/../../../src)
include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/prebuild/shared/gtest/include
        ${PROJECT_SOURCE_FK_DIR}/glm
        ${PROJECT_SOURCE_FK_DIR}/al_common/include
        ${PROJECT_SOURCE_FK_DIR}/FkBase/include
        ${PROJECT_SOURCE_FK_DIR}/FkFramework/include
        ${PROJECT_SOURCE_FK_DIR}/FkGraphic/include
        ${PROJECT_SOURCE_FK_DIR}/FkBitmap/include
        ${PROJECT_SOURCE_FK_DIR}/FkImage/include
)

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

set(PREBUILD_SHARED "${CMAKE_CURRENT_SOURCE_DIR}/prebuild/static")
set(GTEST ${PREBUILD_SHARED}/gtest)
add_library(gtest_main STATIC IMPORTED)
set_target_properties(gtest_main PROPERTIES IMPORTED_LOCATION
        "${GTEST}/libs/${ANDROID_ABI}/libgtest_main.a")
add_library(gtest STATIC IMPORTED)
set_target_properties(gtest PROPERTIES IMPORTED_LOCATION
        "${GTEST}/libs/${ANDROID_ABI}/libgtest.a")
add_library(gmock STATIC IMPORTED)
set_target_properties(gmock PROPERTIES IMPORTED_LOCATION
        "${GTEST}/libs/${ANDROID_ABI}/libgmock.a")
add_library(gmock_main STATIC IMPORTED)
set_target_properties(gmock_main PROPERTIES IMPORTED_LOCATION
        "${GTEST}/libs/${ANDROID_ABI}/libgmock_main.a")

aux_source_directory(${PROJECT_SOURCE_DIR}/starter SOURCES_STARTER)
aux_source_directory(${PROJECT_SOURCE_DIR}/module SOURCES_MODULE)
aux_source_directory(${PROJECT_SOURCE_DIR}/samples SOURCES_SAMPLES)

list(APPEND SOURCES_ROOT ${SOURCES_STARTER})
list(APPEND SOURCES_ROOT ${SOURCES_MODULE})
list(APPEND SOURCES_ROOT ${SOURCES_SAMPLES})

add_library(${MODULE_NAME} SHARED ${SOURCES_ROOT})

find_library(
        log-lib
        log)

set(MODULES ${MODULE_NAME} gtest_main gtest al_common FkBase FkFramework FkGraphic FkBitmap FkImage ${log-lib})
target_link_libraries(${MODULES})