cmake_minimum_required(VERSION 3.4)
project(sfinae_examples)

# Defines macro for adding examples
macro(add_example name)
  add_executable(${name} src/${name}.cpp)
  set_target_properties(${name} PROPERTIES CXX_STANDARD 17)
endmacro()

add_example(watch_std_io)
