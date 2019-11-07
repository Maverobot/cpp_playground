cmake_minimum_required(VERSION 2.8.3)
project(indices_trick)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(indices_trick src/main.cpp)

target_include_directories(indices_trick PUBLIC include)
