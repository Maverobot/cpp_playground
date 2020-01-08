cmake_minimum_required(VERSION 3.1)
include(./cmake/tools.cmake)
hunter_init(LOCAL)
project(download-gRPC)

hunter_add_package(gRPC)

set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 11)

find_package(gRPC CONFIG REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main gRPC::grpc)
