cmake_minimum_required(VERSION 3.4.1)
project(hwvc_render LANGUAGES C CXX)
if (ANDROID)
    include_directories(
            ${PROJECT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/../common/include
            ${PROJECT_SOURCE_DIR}/../al_bitmap/include
            ${PROJECT_SOURCE_DIR}/platform/android
            ${PROJECT_SOURCE_DIR}/platform/android/buffer
            ${PROJECT_SOURCE_DIR}/platform/android/buffer/loader
            ${PROJECT_SOURCE_DIR}/platform/android/entity
    )
else ()
    include_directories(
            ${PROJECT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/../common/include
            ${PROJECT_SOURCE_DIR}/../al_bitmap/include
    )
endif ()
option(GL_DEBUG "Whether show opengl logs." ON)
option(ENABLE_CROP_DEBUG "Enable layer crop debug." OFF)
if (GL_DEBUG)
    add_definitions(-DGL_DEBUG)
    message(GL_DEBUG "Show opengl logs。")
endif ()
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

if (ANDROID)
    message(STATUS "hwrender android platform.")
    file(
            GLOB
            SOURCES
            ${PROJECT_SOURCE_DIR}/*.cpp
            ${PROJECT_SOURCE_DIR}/entity/*.cpp
            ${PROJECT_SOURCE_DIR}/filter/*.cpp
            ${PROJECT_SOURCE_DIR}/drawer/*.cpp
            ${PROJECT_SOURCE_DIR}/egl/*.cpp
            ${PROJECT_SOURCE_DIR}/units/*.cpp
            ${PROJECT_SOURCE_DIR}/units/image/*.cpp
            ${PROJECT_SOURCE_DIR}/processor/*.cpp
            ${PROJECT_SOURCE_DIR}/processor/model/*.cpp
            ${PROJECT_SOURCE_DIR}/tinyxml/*.cpp
            ${PROJECT_SOURCE_DIR}/platform/*.cpp
            ${PROJECT_SOURCE_DIR}/image/*.cpp
            ${PROJECT_SOURCE_DIR}/image/model/*.cpp
            ${PROJECT_SOURCE_DIR}/image/model/opt/*.cpp
            # Android platform
            ${PROJECT_SOURCE_DIR}/platform/android/*.cpp
            ${PROJECT_SOURCE_DIR}/platform/android/buffer/*.cpp
            ${PROJECT_SOURCE_DIR}/platform/android/buffer/loader/*.cpp
            ${PROJECT_SOURCE_DIR}/platform/android/entity/*.cpp
            ${PROJECT_SOURCE_DIR}/texture/HwAbsFrameBuffer.cpp
            ${PROJECT_SOURCE_DIR}/texture/HwFrameBuffer.cpp
            ${PROJECT_SOURCE_DIR}/texture/HwAndroidFrameBuffer.cpp
    )
else ()
    # TODO other platform
    message(STATUS "hwrender IOS platform.")
    file(
            GLOB
            SOURCES
            ${PROJECT_SOURCE_DIR}/*.cpp
            ${PROJECT_SOURCE_DIR}/entity/*.cpp
            ${PROJECT_SOURCE_DIR}/filter/*.cpp
            ${PROJECT_SOURCE_DIR}/drawer/*.cpp
            ${PROJECT_SOURCE_DIR}/egl/*.cpp
            ${PROJECT_SOURCE_DIR}/units/*.cpp
            ${PROJECT_SOURCE_DIR}/units/image/*.cpp
            ${PROJECT_SOURCE_DIR}/processor/*.cpp
            ${PROJECT_SOURCE_DIR}/processor/model/*.cpp
            ${PROJECT_SOURCE_DIR}/tinyxml/*.cpp
            ${PROJECT_SOURCE_DIR}/platform/*.cpp
            ${PROJECT_SOURCE_DIR}/image/*.cpp
            ${PROJECT_SOURCE_DIR}/image/model/*.cpp
            ${PROJECT_SOURCE_DIR}/image/model/opt/*.cpp
    )
endif ()
message(STATUS "${SOURCES}")
add_library( # Sets the name of the library.
        hwvc_render

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
            hwvc_render
            albitmap
            hwvcom
            # Links the target library to the log library
            # included in the NDK.
            #        GLESv1_CM
            GLESv2
            EGL
            android
            jnigraphics
            ${log-lib})
else ()
    # TODO other platform
    target_link_libraries( # Specifies the target library.
            hwvc_render
            albitmap
            hwvcom)
endif ()