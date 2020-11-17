cmake_minimum_required(VERSION 3.11)
project(sml_examples)

add_library(sml INTERFACE)
target_include_directories(sml INTERFACE 3rdParty/sml)

add_executable(hello_world src/hello_world.cpp)
target_include_directories(hello_world PRIVATE include)
target_link_libraries(hello_world PRIVATE sml)
