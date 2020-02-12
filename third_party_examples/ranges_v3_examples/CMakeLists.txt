cmake_minimum_required(VERSION 3.11)
project(ranges_v3_examples)

set(CMAKE_BUILD_TYPE Release)

include(FetchContent)
FetchContent_Declare(
  range-v3
  GIT_REPOSITORY https://github.com/ericniebler/range-v3.git
  GIT_TAG 0.10.0)
FetchContent_GetProperties(range-v3)
if(NOT range-v3_POPULATED)
  FetchContent_Populate(range-v3)
  add_subdirectory(${range-v3_SOURCE_DIR} ${range-v3_BINARY_DIR})
endif()

add_executable(hello src/hello.cpp)
target_include_directories(hello PRIVATE ${range-v3_SOURCE_DIR}/include)
