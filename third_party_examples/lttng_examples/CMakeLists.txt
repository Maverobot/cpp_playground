cmake_minimum_required(VERSION 3.4)
project(lttng_examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LTTNG QUIET lttng-ust)
if(NOT LTTNG_FOUND)
  message(FATAL_ERROR "LTTNG not found. Try with 'sudo apt install liblttng-ust-dev'.")
endif()

add_library(hello-tp src/hello-tp.c)
target_include_directories(hello-tp PUBLIC include)
target_link_libraries(hello-tp PUBLIC lttng-ust dl)

add_executable(hello src/hello.cpp)
target_link_libraries(hello PRIVATE hello-tp)
