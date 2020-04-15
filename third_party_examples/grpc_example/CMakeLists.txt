cmake_minimum_required(VERSION 3.1)
include(./cmake/tools.cmake)
hunter_init(LOCAL)
project(download-gRPC)

hunter_add_package(gRPC)

set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 11)

find_package(gRPC CONFIG REQUIRED)

add_subdirectory(messages)

add_executable(greeter_server src/hello_world/greeter_server.cpp)
target_link_libraries(greeter_server messages gRPC::grpc gRPC::grpc++_unsecure)

add_executable(greeter_client src/hello_world/greeter_client.cpp)
target_link_libraries(greeter_client messages gRPC::grpc gRPC::grpc++_unsecure)

add_executable(route_guide_server src/route_guide/helper.cpp src/route_guide/route_guide_server.cpp)
target_link_libraries(route_guide_server messages gRPC::grpc gRPC::grpc++_unsecure)

add_executable(route_guide_client src/route_guide/helper.cpp src/route_guide/route_guide_client.cpp)
target_link_libraries(route_guide_client messages gRPC::grpc gRPC::grpc++_unsecure)
