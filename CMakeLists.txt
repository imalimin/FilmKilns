cmake_minimum_required(VERSION 3.4.1)
set(SRC_PATH_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src/)
option(ENABLE_BAN_WARNING "Enable warning as error." ON)
option(ENABLE_TINY "Enable tiny so." OFF)
option(ENABLE_GLM "Enable glm." OFF)
option(ENABLE_ASAN "Enable ASAN." OFF)
option(ENABLE_MEDIA_CODEC "Enable MediaCodec." ON)
option(ENABLE_FK "Compile film kilns." OFF)

set(WARNING_CONFIGS
        -Werror
        -Wreturn-type
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
        -Wno-unknown-pragmas
        -Wno-overloaded-virtual
        -Wno-parentheses
        -Wno-pointer-sign
        -Wno-switch
        -Wno-implicit-int)

set(CXX_FLAGS_CONFIGS "-std=c++17 -Wall")
set(C_FLAGS_CONFIGS "-Wall")
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
    add_definitions(-D__FK_DEBUG__)
endif ()
if (ENABLE_MEDIA_CODEC)
    message(STATUS "Enable MediaCodec.")
    add_definitions(-D__AL_ENABLE_MEDIA_CODEC__)
endif (ENABLE_MEDIA_CODEC)

if (ENABLE_FK)
    message(STATUS "Compile film kilns.")
    add_subdirectory(${SRC_PATH_ROOT}/al_common)
    add_subdirectory(${SRC_PATH_ROOT}/al_bitmap)
    add_subdirectory(${SRC_PATH_ROOT}/FkBase)
    add_subdirectory(${SRC_PATH_ROOT}/FkFramework)
    add_subdirectory(${SRC_PATH_ROOT}/FkBitmap)
    add_subdirectory(${SRC_PATH_ROOT}/FkGraphic)
    add_subdirectory(${SRC_PATH_ROOT}/FkImage)
else ()
    add_subdirectory(${SRC_PATH_ROOT}/al_common)
    add_subdirectory(${SRC_PATH_ROOT}/al_bitmap)
    add_subdirectory(${SRC_PATH_ROOT}/al_graphic)
    add_subdirectory(${SRC_PATH_ROOT}/al_image)
    add_subdirectory(${SRC_PATH_ROOT}/al_codec)
endif ()
if (ENABLE_GLM)
    add_definitions(-DENABLE_GLM)
    message(STATUS "Use glm.")
    add_subdirectory(${SRC_PATH_ROOT}/glm)
endif (ENABLE_GLM)
#add_subdirectory(${SRC_PATH_ROOT}/sdl2)