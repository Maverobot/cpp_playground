cmake_minimum_required(VERSION 3.10)
project(dynamic_memory_allocation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pmr pmr.cpp)
