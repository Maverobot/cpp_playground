cmake_minimum_required(VERSION 2.8.3)
project(tricks)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(indices_trick src/indices_trick.cpp)
target_include_directories(indices_trick PUBLIC include)

add_executable(unwrap_reference src/unwrap_reference.cpp)
