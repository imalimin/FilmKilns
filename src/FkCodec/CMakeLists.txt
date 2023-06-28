cmake_minimum_required(VERSION 3.4.1)
project(FkCodec LANGUAGES C CXX)

set(INCLUDES
        ${ANDROID_NDK}/sources/android/cpufeatures
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/../FkBase/include
        )
include_directories(${INCLUDES})

message(STATUS "${PROJECT_NAME} ENABLE_IJK: ${ENABLE_IJK}")
if (ENABLE_IJK)
    include_directories(${THIRD_PARTY_DIR}/ijkffmpeg/include)
    add_definitions("-D__ENABLE_IJK__")
else ()
    include_directories(${THIRD_PARTY_DIR}/ffmpeg/include)
endif()

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
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${var}")
    endforeach (var)
endif ()
message(STATUS "${PROJECT_NAME} CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

#------------------SHARED START----------------------
if (ENABLE_IJK)
    add_library(ffmpeg SHARED IMPORTED)
    set_target_properties(ffmpeg PROPERTIES IMPORTED_LOCATION
            "${THIRD_PARTY_DIR}/ijkffmpeg/shared/${ANDROID_ABI}/libijkffmpeg.so")
else ()
    add_library(ffmpeg SHARED IMPORTED)
    set_target_properties(ffmpeg PROPERTIES IMPORTED_LOCATION
            "${THIRD_PARTY_DIR}/ffmpeg/shared/${ANDROID_ABI}/libhwffmpeg.so")
endif ()

add_library(yuv SHARED IMPORTED)
set_target_properties(yuv PROPERTIES IMPORTED_LOCATION
        "${THIRD_PARTY_DIR}/yuv/shared/${ANDROID_ABI}/libyuv.so")

#------------------SHARED END----------------------

aux_source_directory(${PROJECT_SOURCE_DIR}/codec SOURCES_CODEC)
aux_source_directory(${PROJECT_SOURCE_DIR}/entity SOURCES_ENTITY)
aux_source_directory(${PROJECT_SOURCE_DIR}/transform SOURCES_TRANS)
aux_source_directory(${PROJECT_SOURCE_DIR}/sources SOURCES_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/encoder SOURCES_ENC)
aux_source_directory(${PROJECT_SOURCE_DIR}/muxer SOURCES_MUXER)
aux_source_directory(${PROJECT_SOURCE_DIR}/utils SOURCES_UTILS)
aux_source_directory(${PROJECT_SOURCE_DIR}/utils/fftools SOURCES_FF_TOOLS)

list(APPEND SOURCES_ROOT ${SOURCES_CODEC})
list(APPEND SOURCES_ROOT ${SOURCES_ENTITY})
list(APPEND SOURCES_ROOT ${SOURCES_TRANS})
list(APPEND SOURCES_ROOT ${SOURCES_SRC})
list(APPEND SOURCES_ROOT ${SOURCES_ENC})
list(APPEND SOURCES_ROOT ${SOURCES_MUXER})
list(APPEND SOURCES_ROOT ${SOURCES_UTILS})
list(APPEND SOURCES_ROOT ${SOURCES_FF_TOOLS})

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

find_library(
        log-lib
        log)
add_library(${PROJECT_NAME} STATIC ${SOURCES_ROOT})
target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
        android
        ffmpeg
        yuv
        FkBase
        ${log-lib})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include ${THIRD_PARTY_DIR}/ffmpeg/include ${THIRD_PARTY_DIR}/yuv/include)
