cmake_minimum_required(VERSION 3.8)
project(memory_ordering_examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(relaxed_ordering relaxed_ordering.cpp)
target_link_libraries(relaxed_ordering Threads::Threads)
