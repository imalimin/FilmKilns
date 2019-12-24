cmake_minimum_required(VERSION 3.4.1)
project(albitmap LANGUAGES C CXX)

option(HWDEBUG "Whether show opengl logs." ON)
if (HWDEBUG)
    add_definitions(-DHWDEBUG)
    message(STATUS "Show debug logs。")
endif ()

set(PREBUILD_STATIC ${PROJECT_SOURCE_DIR}/prebuild/static)
set(JPGE ${PREBUILD_STATIC}/libturbojpeg/libs/${ANDROID_ABI})
set(PNG ${PREBUILD_STATIC}/libpng/libs/${ANDROID_ABI})
set(WEBP ${PREBUILD_STATIC}/libwebp/libs/${ANDROID_ABI})

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../common/include)
include_directories(${PREBUILD_STATIC}/libturbojpeg/include)
include_directories(${PREBUILD_STATIC}/libpng/include)
include_directories(${PREBUILD_STATIC}/libwebp/include)

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
message(STATUS "al_bitmap CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

#------------------STATIC START----------------------
#libjpeg
#add_library(
#        libjpeg
#        STATIC
#        IMPORTED)
#set_target_properties(
#        libjpeg
#        PROPERTIES IMPORTED_LOCATION
#        "${JPGE}/libjpeg.a")

#libturbojpeg
add_library(
        libturbojpeg
        STATIC
        IMPORTED)
set_target_properties(
        libturbojpeg
        PROPERTIES IMPORTED_LOCATION
        "${JPGE}/libturbojpeg.a")

#libpng16
add_library(
        libpng16
        STATIC
        IMPORTED)
set_target_properties(
        libpng16
        PROPERTIES IMPORTED_LOCATION
        "${PNG}/libpng16.a")

#libwebp
add_library(
        libwebp
        STATIC
        IMPORTED)
set_target_properties(
        libwebp
        PROPERTIES IMPORTED_LOCATION
        "${WEBP}/libwebp.a")

#------------------STATIC END----------------------

file(
        GLOB
        SOURCES
        ${PROJECT_SOURCE_DIR}/core/*.cpp
        ${PROJECT_SOURCE_DIR}/image/*.cpp
)

add_library(
        albitmap
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
        albitmap
        hwvcom
        #        libjpeg
        libturbojpeg
        libpng16
        libwebp
        ${log-lib}
)