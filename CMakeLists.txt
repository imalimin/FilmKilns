cmake_minimum_required(VERSION 3.4.1)
set(SRC_PATH_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src/)
option(ENABLE_BAN_WARNING "Enable warning as error." OFF)
option(ENABLE_TINY "Enable tiny so." OFF)
option(ENABLE_GLM "Enable glm." OFF)
option(ENABLE_SKIA "Enable skia." ON)
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

#if (${ANDROID_ABI} STREQUAL "armeabi-v7a")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=neon ")
#elseif(${ANDROID_ABI} STREQUAL "arm64-v8a")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#endif ()
#

add_subdirectory(${SRC_PATH_ROOT}/al_common)
add_subdirectory(${SRC_PATH_ROOT}/al_bitmap)
add_subdirectory(${SRC_PATH_ROOT}/al_graphic)
add_subdirectory(${SRC_PATH_ROOT}/al_codec)
add_subdirectory(${SRC_PATH_ROOT}/al_image)
if (ENABLE_GLM)
    add_definitions(-DENABLE_GLM)
    message(STATUS "Use glm.")
    add_subdirectory(${SRC_PATH_ROOT}/glm)
endif (ENABLE_GLM)
if (ENABLE_SKIA)
    add_definitions(-DENABLE_SKIA)
    message(STATUS "Use skia.")
    add_subdirectory(${SRC_PATH_ROOT}/skia)
    add_subdirectory(${SRC_PATH_ROOT}/skia/third_party/externals/expat)
endif (ENABLE_SKIA)
#add_subdirectory(${SRC_PATH_ROOT}/sdl2)