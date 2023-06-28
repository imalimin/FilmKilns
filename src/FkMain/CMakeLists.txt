cmake_minimum_required(VERSION 3.4.1)
project(FilmKilns LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall")

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
message(STATUS "${PROJECT_NAME} CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

if (${ANDROID_ABI} STREQUAL "armeabi-v7a")
    set_property(SOURCE ${SOURCES_ROOT} APPEND_STRING PROPERTY COMPILE_FLAGS " -mfpu=neon")
    add_definitions("-DHAVE_NEON=1")
elseif (${ANDROID_ABI} STREQUAL "x86")
    set_property(SOURCE ${SOURCES_ROOT} APPEND_STRING PROPERTY COMPILE_FLAGS
            " -mssse3  -Wno-unknown-attributes \
                   -Wno-deprecated-declarations \
                   -Wno-constant-conversion \
                   -Wno-static-in-inline")
    add_definitions(-DHAVE_NEON_X86=1 -DHAVE_NEON=1)
endif ()

add_library(${PROJECT_NAME} SHARED empty.cpp)
target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
        FkBase
        FkFramework
        FkBitmap
        FkGraphic
        FkImage
        FkCodec)
