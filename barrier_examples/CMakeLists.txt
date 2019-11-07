cmake_minimum_required(VERSION 3.8)
project(barrier)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(simple_barrier src/simple_barrier.cpp)
target_link_libraries(simple_barrier Threads::Threads)
