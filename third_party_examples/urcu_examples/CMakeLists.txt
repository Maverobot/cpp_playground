cmake_minimum_required(VERSION 3.11)
project(urcu_examples)

find_package(Threads REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE urcu urcu-bp Threads::Threads)
