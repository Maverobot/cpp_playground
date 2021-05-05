cmake_minimum_required(VERSION 3.9)
project(cpprestsdk_examples)

find_package(cpprestsdk QUIET)

if(NOT cpprestsdk_FOUND)
  execute_process(COMMAND sudo apt install -y libcpprest-dev COMMAND_ECHO STDOUT)
  find_package(cpprestsdk REQUIRED)
endif()

add_executable(bing_search bing_search.cpp)
target_link_libraries(bing_search PRIVATE cpprestsdk::cpprest)
