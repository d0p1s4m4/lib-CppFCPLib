cmake_minimum_required(VERSION 3.25)
project(FCP)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS system filesystem thread REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(src)
add_subdirectory(examples)
