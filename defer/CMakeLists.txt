cmake_minimum_required(VERSION 3.4)
project(defer)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(defer src/defer.cpp)
target_compile_options(defer PRIVATE -Wall -Wextra -pedantic)
