cmake_minimum_required(VERSION 3.4)
project(smart_rate)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(smart_rate
  src/main.cpp
)
