cmake_minimum_required(VERSION 3.4.1)
project(al_graphic LANGUAGES C CXX)

set(INCLUDES
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/../al_common/include
        ${PROJECT_SOURCE_DIR}/../al_bitmap/include
        )
if (ANDROID)
    set(INCLUDES
            ${INCLUDES}
            ${PROJECT_SOURCE_DIR}/platform/android
            ${PROJECT_SOURCE_DIR}/platform/android/buffer
            ${PROJECT_SOURCE_DIR}/platform/android/buffer/loader
            ${PROJECT_SOURCE_DIR}/platform/android/entity
            )
endif (ANDROID)
include_directories(${INCLUDES})

option(GL_DEBUG "Whether show opengl logs." ON)
if (GL_DEBUG)
    add_definitions(-DGL_DEBUG)
    message(GL_DEBUG "Show opengl logs。")
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS_CONFIGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS_CONFIGS}")

#Enable EGL android ext.
if (ANDROID)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEGL_EGLEXT_PROTOTYPES -DGL_GLEXT_PROTOTYPES")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DEGL_EGLEXT_PROTOTYPES -DGL_GLEXT_PROTOTYPES")
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

file(GLOB SOURCES
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/entity/*.cpp
        ${PROJECT_SOURCE_DIR}/filter/*.cpp
        ${PROJECT_SOURCE_DIR}/drawer/*.cpp
        ${PROJECT_SOURCE_DIR}/egl/*.cpp
        ${PROJECT_SOURCE_DIR}/units/*.cpp
        ${PROJECT_SOURCE_DIR}/units/image/*.cpp
        ${PROJECT_SOURCE_DIR}/processor/*.cpp
        ${PROJECT_SOURCE_DIR}/processor/model/*.cpp
        ${PROJECT_SOURCE_DIR}/platform/*.cpp
        ${PROJECT_SOURCE_DIR}/sources/*.cpp
        ${PROJECT_SOURCE_DIR}/core/*.cpp
        ${PROJECT_SOURCE_DIR}/utils/*.cpp
        #Header
        ${PROJECT_SOURCE_DIR}/include/*.h
        )
if (ANDROID)
    file(GLOB
            SOURCES_ANDROID
            # Android platform
            ${PROJECT_SOURCE_DIR}/platform/android/*.cpp
            ${PROJECT_SOURCE_DIR}/platform/android/buffer/*.cpp
            ${PROJECT_SOURCE_DIR}/platform/android/buffer/loader/*.cpp
            ${PROJECT_SOURCE_DIR}/platform/android/entity/*.cpp
            ${PROJECT_SOURCE_DIR}/texture/HwAbsFrameBuffer.cpp
            ${PROJECT_SOURCE_DIR}/texture/HwFrameBuffer.cpp
            ${PROJECT_SOURCE_DIR}/texture/HwAndroidFrameBuffer.cpp
            )
    set(SOURCES
            ${SOURCES}
            ${SOURCES_ANDROID}
            )
endif (ANDROID)

add_library(al_graphic SHARED ${SOURCES})

set(MODULES
        al_graphic
        al_bitmap
        al_common
        )

if (ANDROID)
    find_library(log-lib log)
    set(MODULES
            ${MODULES}
            GLESv2
            EGL
            android
            jnigraphics
            )
endif (ANDROID)
target_link_libraries(${MODULES})