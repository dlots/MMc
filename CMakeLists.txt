cmake_minimum_required(VERSION 3.1...3.19)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(mmc_simulation
        VERSION 0.1
        DESCRIPTION "testing mmc lib"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES main.cpp)

set(HEADERS )

add_executable(mmc_simulation ${SOURCES} ${HEADERS})

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    add_subdirectory(mmc)
    target_link_libraries(mmc_simulation mmc)

endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall  -Wextra")