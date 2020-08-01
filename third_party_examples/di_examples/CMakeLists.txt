cmake_minimum_required(VERSION 3.11)
project(di_examples)

add_library(di INTERFACE)
target_include_directories(di INTERFACE include)
target_compile_features(di INTERFACE cxx_std_20)

add_executable(hello_world hello_world.cpp)
target_link_libraries(hello_world di)
