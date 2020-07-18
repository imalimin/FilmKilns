cmake_minimum_required(VERSION 3.4.1)
project(al_image LANGUAGES C CXX)
set(INCLUDES
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/../al_common/include
        ${PROJECT_SOURCE_DIR}/../al_graphic/include
        ${PROJECT_SOURCE_DIR}/../al_bitmap/include
        )
include_directories(${INCLUDES})

option(ENABLE_CROP_DEBUG "Enable layer crop debug." OFF)
if (ENABLE_CROP_DEBUG)
    add_definitions(-DENABLE_CROP_DEBUG)
endif ()

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
message(STATUS "al_render CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

file(GLOB SOURCES
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/core/*.cpp
        ${PROJECT_SOURCE_DIR}/core/file/*.cpp
        ${PROJECT_SOURCE_DIR}/drawer/*.cpp
        ${PROJECT_SOURCE_DIR}/model/*.cpp
        ${PROJECT_SOURCE_DIR}/model/action/*.cpp
        ${PROJECT_SOURCE_DIR}/model/entity/*.cpp
        ${PROJECT_SOURCE_DIR}/units/*.cpp
        ${PROJECT_SOURCE_DIR}/units/graph/*.cpp
        #Header
        ${PROJECT_SOURCE_DIR}/include/*.h
        )

add_library(al_image SHARED ${SOURCES})

set(MODULES
        al_image
        al_common
        al_bitmap
        al_graphic
        )
if (ANDROID)
    find_library(log-lib log)
    set(MODULES
            ${MODULES}
            ${log-lib}
            )
endif (ANDROID)
target_link_libraries(${MODULES})