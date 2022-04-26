cmake_minimum_required(VERSION 3.4)
project(udp_socket)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(server server.cpp)
add_executable(client client.cpp)
add_executable(client_and_server client_and_server.cpp)
