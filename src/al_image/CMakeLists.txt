cmake_minimum_required(VERSION 3.4.1)
project(alimage LANGUAGES C CXX)
if (ANDROID)
    include_directories(
            ${PROJECT_SOURCE_DIR}
            ${PROJECT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/../common/include
            ${PROJECT_SOURCE_DIR}/../render/include
            ${PROJECT_SOURCE_DIR}/../al_bitmap/include
    )
else ()
    include_directories(
            ${PROJECT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/../common/include
            ${PROJECT_SOURCE_DIR}/../render/include
            ${PROJECT_SOURCE_DIR}/../al_bitmap/include
    )
endif ()
option(ENABLE_CROP_DEBUG "Enable layer crop debug." OFF)
if (ENABLE_CROP_DEBUG)
    add_definitions(-DENABLE_CROP_DEBUG)
endif ()

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
message(STATUS "al_render CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

if (ANDROID)
    message(STATUS "hwrender android platform.")
    file(
            GLOB
            SOURCES
            ${PROJECT_SOURCE_DIR}/*.cpp
            ${PROJECT_SOURCE_DIR}/core/*.cpp
            ${PROJECT_SOURCE_DIR}/core/file/*.cpp
            ${PROJECT_SOURCE_DIR}/drawer/*.cpp
            ${PROJECT_SOURCE_DIR}/model/*.cpp
            ${PROJECT_SOURCE_DIR}/model/action/*.cpp
            ${PROJECT_SOURCE_DIR}/units/*.cpp
            #Header
            ${PROJECT_SOURCE_DIR}/include/*.h
    )
else ()
    #TODO other platform
    file(
            GLOB
            SOURCES
            ${PROJECT_SOURCE_DIR}/*.cpp
            ${PROJECT_SOURCE_DIR}/core/*.cpp
            ${PROJECT_SOURCE_DIR}/core/file/*.cpp
            ${PROJECT_SOURCE_DIR}/drawer/*.cpp
            ${PROJECT_SOURCE_DIR}/model/*.cpp
            ${PROJECT_SOURCE_DIR}/model/action/*.cpp
            ${PROJECT_SOURCE_DIR}/units/*.cpp
    )
endif ()
message(STATUS "${SOURCES}")
add_library(
        alimage
        SHARED
        ${SOURCES}
)

if (ANDROID)
    find_library(
            log-lib
            log)
    target_link_libraries(
            alimage
            hwvcom
            albitmap
            hwvc_render
            ${log-lib})
else ()
    # TODO other platform
    target_link_libraries(
            alimage
            hwvcom
            albitmap
            hwvc_render)
endif ()