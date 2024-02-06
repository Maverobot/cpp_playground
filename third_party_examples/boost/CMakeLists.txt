cmake_minimum_required(VERSION 3.4)
project(boost)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.7 REQUIRED COMPONENTS thread filesystem iostreams)

function(add_example name)
  add_executable(${name} ${name}.cpp)
  target_include_directories(${name} PRIVATE ${Boost_INCLUDE_DIRS})
  target_link_libraries(${name} PRIVATE ${Boost_LIBRARIES})
  target_compile_options(${name} PRIVATE -Wall -Wextra -pedantic)
endfunction()

add_example(single_process)
add_example(filter_stream)
add_example(compress_command_output)
add_example(async_compress_command_output)
