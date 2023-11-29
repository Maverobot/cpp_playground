cmake_minimum_required(VERSION 3.4)
project(bug_examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(dead_condition_variable src/dead_condition_variable.cpp)
target_link_libraries(dead_condition_variable Threads::Threads)
target_compile_options(dead_condition_variable PRIVATE -Wall -Wextra -pedantic)
