cmake_minimum_required(VERSION 3.4)
project(defer)

add_executable(defer src/defer.cpp)
target_compile_options(defer PRIVATE -Wall -Wextra -pedantic)
target_compile_features(defer PRIVATE cxx_std_17)
