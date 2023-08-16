cmake_minimum_required(VERSION 3.4.1)
project(FkAndroid LANGUAGES C CXX)

set(PROJECT_SOURCE_FK_DIR ${CMAKE_SOURCE_DIR}/../../../src)
include_directories(
        ${PROJECT_SOURCE_DIR}/include
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall")
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math -funroll-loops ")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -ffast-math -funroll-loops ")
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
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${var}")
    endforeach (var)
endif ()
message(STATUS "${PROJECT_NAME} CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

aux_source_directory(${PROJECT_SOURCE_DIR}/core SOURCES_CORE)
aux_source_directory(${PROJECT_SOURCE_DIR}/engine SOURCES_ENGINE)
aux_source_directory(${PROJECT_SOURCE_DIR}/entity SOURCES_ENTITY)

list(APPEND SOURCES_ROOT ${SOURCES_CORE})
list(APPEND SOURCES_ROOT ${SOURCES_ENGINE})
list(APPEND SOURCES_ROOT ${SOURCES_ENTITY})

find_library(
        log-lib
        log)
add_library(${PROJECT_NAME} SHARED ${SOURCES_ROOT})
target_link_libraries(${PROJECT_NAME} FilmKilns ${log-lib})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

if (ENABLE_ASAN)
    target_compile_options(${PROJECT_NAME} PUBLIC -fsanitize=address -fno-omit-frame-pointer -fsanitize-address-use-after-scope)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS -fsanitize=address)
endif ()