cmake_minimum_required(VERSION 3.4)
project(nameof)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(nameof_example src/nameof_example.cpp)
target_include_directories(nameof_example PRIVATE include ../indices_trick/include)
target_compile_options(nameof_example PRIVATE -Wall -Wextra -pedantic)
