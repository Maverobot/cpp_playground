cmake_minimum_required(VERSION 3.4)
project(sfinae_example)

set(CMAKE_BUILD_TYPE Release)

add_executable(default_constructable
  src/default_constructable.cpp
)
set_target_properties(default_constructable PROPERTIES
  CXX_STANDARD 17)

add_executable(is_convertable
  src/is_convertable.cpp
)
set_target_properties(is_convertable PROPERTIES
  CXX_STANDARD 17)
