cmake_minimum_required(VERSION 3.1)
include(./cmake/tools.cmake)
hunter_init(LOCAL)
project(download-gRPC)

hunter_add_package(gRPC)

set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 11)

find_package(gRPC CONFIG REQUIRED)

add_subdirectory(messages)

add_executable(greeter_server greeter_server.cpp)
target_link_libraries(greeter_server messages gRPC::grpc gRPC::grpc++_unsecure)

add_executable(greeter_client greeter_client.cpp)
target_link_libraries(greeter_client messages gRPC::grpc gRPC::grpc++_unsecure)
