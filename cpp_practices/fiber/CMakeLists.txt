cmake_minimum_required(VERSION 2.8.3)
project(fiber)

find_package(Boost 1.74 COMPONENTS fiber REQUIRED)

add_executable(fiber_examples src/main.cpp)
target_compile_options(fiber_examples PRIVATE -Werror -Wall)
target_link_libraries(fiber_examples PRIVATE Boost::fiber)
