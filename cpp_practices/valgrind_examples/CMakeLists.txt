cmake_minimum_required(VERSION 2.8.3)
project(valgrind_examples)
set(CMAKE_BUILD_TYPE Debug)

add_executable(memory_leak memory_leak.cpp)
add_executable(fib fib.cpp)
