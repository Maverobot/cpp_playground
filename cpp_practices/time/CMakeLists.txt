cmake_minimum_required(VERSION 2.8.3)
project(time)

find_package(Threads REQUIRED)

add_library(catch INTERFACE)
target_include_directories(catch INTERFACE include)
target_compile_definitions(catch INTERFACE CATCH_CONFIG_ENABLE_BENCHMARKING)

add_executable(tests src/tests_main.cpp src/timing_test.cpp)
target_link_libraries(tests PRIVATE catch Threads::Threads)
target_compile_features(tests PRIVATE cxx_std_17)
