cmake_minimum_required(VERSION 3.4)
project(unix_domain_socket)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(server server.cpp)
target_link_libraries(server PRIVATE Threads::Threads)

add_executable(client client.cpp)

add_executable(socket_pair_example socket_pair_example.cpp)
