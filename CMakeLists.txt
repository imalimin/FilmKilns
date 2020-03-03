cmake_minimum_required(VERSION 3.4.1)
set(SRC_PATH_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src/)
option(ENABLE_BAN_WARNING "Enable warning as error." OFF)
option(ENABLE_TINY "Enable tiny so." OFF)
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

add_subdirectory(${SRC_PATH_ROOT}/common)
add_subdirectory(${SRC_PATH_ROOT}/al_bitmap)
add_subdirectory(${SRC_PATH_ROOT}/render)
add_subdirectory(${SRC_PATH_ROOT}/codec)
add_subdirectory(${SRC_PATH_ROOT}/al_image)
add_subdirectory(${SRC_PATH_ROOT}/glm)
#add_subdirectory(${SRC_PATH_ROOT}/sdl2)