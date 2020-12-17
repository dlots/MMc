cmake_minimum_required(VERSION 3.1...3.19)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(mmc_lib
        VERSION 0.1
        DESCRIPTION "mmc lib build"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES mmc.cpp)

set(HEADERS mmc.hpp)

add_library(mmc ${SOURCES} ${HEADERS})

target_include_directories(mmc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall  -Wextra")