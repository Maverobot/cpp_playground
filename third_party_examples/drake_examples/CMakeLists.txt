cmake_minimum_required(VERSION 3.5.1)
project(drake_examples)

find_package(drake CONFIG REQUIRED)
find_package(gflags CONFIG REQUIRED)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(cart_pole)
