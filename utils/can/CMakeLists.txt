cmake_minimum_required(VERSION 3.4)
project(can)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(candump candump.cpp)
