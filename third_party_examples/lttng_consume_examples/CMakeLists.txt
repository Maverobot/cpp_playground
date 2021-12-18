cmake_minimum_required(VERSION 3.4)
project(lttng_consume_examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch lttng-consume and build the library
find_package(lttng-consume QUIET CONFIG)
if(NOT lttng-consume_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    lttng-consume
    GIT_REPOSITORY https://github.com/microsoft/lttng-consume.git
    GIT_TAG v0.2.1)
  FetchContent_GetProperties(lttng-consume)
  if(NOT lttng-consume_POPULATED)
    message(STATUS "Fetching lttng-consume...")
    FetchContent_Populate(lttng-consume)
    set(ENABLE_TESTING_SAVED "${ENABLE_TESTING}")
    set(ENABLE_TESTING OFF)
    add_subdirectory(${lttng-consume_SOURCE_DIR} ${lttng-consume_BINARY_DIR})
    set(ENABLE_TESTING "${ENABLE_TESTING_SAVED}")
    message(STATUS "lttng-consume has been fetched.")
  endif()
endif()

add_executable(lttng-consume-example lttng_consume_example.cpp)
target_link_libraries(lttng-consume-example PRIVATE lttng-consume)
