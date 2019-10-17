cmake_minimum_required(VERSION 3.4)
project(sfinae_example)

set(CMAKE_BUILD_TYPE Release)

add_executable(sfinae_example
  src/sfinae_example.cpp
)

set_target_properties(sfinae_example PROPERTIES
  CXX_STANDARD 17)
