cmake_minimum_required(VERSION 3.1)
include(./cmake/HunterInit.cmake)
hunter_init()
project(download-gRPC)

hunter_add_package(flatbuffers)

set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 11)

find_package(Flatbuffers CONFIG REQUIRED)
add_subdirectory(messages)

add_executable(foo foo.cpp)
target_link_libraries(foo PUBLIC messages flatbuffers::flatbuffers)
