cmake_minimum_required(VERSION 3.4.1)
set(SRC_PATH_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src/)
option(ENABLE_BAN_WARNING "Enable warning as error." OFF)
option(ENABLE_TINY "Enable tiny so." OFF)
option(ENABLE_GLM "Enable glm." OFF)
option(ENABLE_ASAN "Enable ASAN." OFF)
option(ENABLE_MEDIA_CODEC "Enable MediaCodec." ON)
set(WARNING_CONFIGS
        -Werror
        -Wmost
        -Wno-pragmas
        -Wno-unused-function
        -Wno-unused-label
        -Wno-unused-value
        -Wno-unused-variable
        -Wno-c99-extensions
        -Wno-reorder
        -Wno-unused-private-field
        -Wno-deprecated-declarations
        -Wno-unknown-pragmas)

set(CXX_FLAGS_CONFIGS "-std=c++17 -Wall")
set(C_FLAGS_CONFIGS "-std=c11 -Wall")
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CXX_FLAGS_CONFIGS "${CXX_FLAGS_CONFIGS} -O3 -ffast-math -funroll-loops ")
    set(C_FLAGS_CONFIGS "${C_FLAGS_CONFIGS} -O3 -ffast-math -funroll-loops ")
else ()
    set(CXX_FLAGS_CONFIGS "${CXX_FLAGS_CONFIGS} -O1 -g -ggdb ")
    set(C_FLAGS_CONFIGS "${C_FLAGS_CONFIGS} -O1 -g -ggdb ")
    if (ENABLE_ASAN)
        message(STATUS "Enable ASAN!")
        set(CXX_FLAGS_CONFIGS "${CXX_FLAGS_CONFIGS} -fsanitize=address -fno-omit-frame-pointer ")
        set(C_FLAGS_CONFIGS "${C_FLAGS_CONFIGS} -fsanitize=address -fno-omit-frame-pointer ")
    endif (ENABLE_ASAN)
endif ()

#if (${ANDROID_ABI} STREQUAL "armeabi-v7a")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=neon ")
#elseif(${ANDROID_ABI} STREQUAL "arm64-v8a")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#endif ()
#
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message(STATUS "Release mode.")
else ()
    message(STATUS "Debug mode.")
    add_definitions(-D__AL_DEBUG__)
endif ()
if (ENABLE_MEDIA_CODEC)
    message(STATUS "Enable MediaCodec.")
    add_definitions(-D__AL_ENABLE_MEDIA_CODEC__)
endif (ENABLE_MEDIA_CODEC)

add_subdirectory(${SRC_PATH_ROOT}/al_common)
add_subdirectory(${SRC_PATH_ROOT}/al_bitmap)
add_subdirectory(${SRC_PATH_ROOT}/al_graphic)
add_subdirectory(${SRC_PATH_ROOT}/al_image)
add_subdirectory(${SRC_PATH_ROOT}/al_codec)
if (ENABLE_GLM)
    add_definitions(-DENABLE_GLM)
    message(STATUS "Use glm.")
    add_subdirectory(${SRC_PATH_ROOT}/glm)
endif (ENABLE_GLM)
#add_subdirectory(${SRC_PATH_ROOT}/sdl2)