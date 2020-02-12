cmake_minimum_required(VERSION 2.8.3)
project(lock_free_structure)

add_executable(hello_lock_free src/hello_lock_free.cpp)
target_compile_features(hello_lock_free PRIVATE cxx_std_17)
