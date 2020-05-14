cmake_minimum_required(VERSION 3.4)
project(attributes_example)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(attributes_example src/attributes_example.cpp)
target_compile_options(attributes_example PRIVATE -Wall -Wextra -pedantic)
