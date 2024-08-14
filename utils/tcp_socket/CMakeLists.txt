cmake_minimum_required(VERSION 3.4)
project(tcp_socket)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# TCP server and client in C
add_executable(server_c server.c)
add_executable(client_c client.c)


# TCP client in C++
add_executable(client_cpp client.cpp)
