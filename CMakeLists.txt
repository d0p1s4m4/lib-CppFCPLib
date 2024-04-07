cmake_minimum_required(VERSION 3.25)
project(FCP++)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost COMPONENTS system REQUIRED)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    Include(FetchContent)

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.4.0)

    FetchContent_MakeAvailable(Catch2)

    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

    include(CTest)
    include(Catch)
    include_directories(${Catch2_INCLUDE_DIRS})
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(examples)
    add_subdirectory(tests)
endif()