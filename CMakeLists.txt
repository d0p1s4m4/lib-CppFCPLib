cmake_minimum_required(VERSION 3.25)
project(FCP)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS system filesystem thread REQUIRED)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    find_package(Catch2 REQUIRED)
    include(CTest)
    include(Catch)
    include_directories(${Catch2_INCLUDE_DIRS})
endif()

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(examples)
    add_subdirectory(tests)
endif()