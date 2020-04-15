cmake_minimum_required(VERSION 3.4)
project(matplotlib_cpp)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PythonLibs 2.7)

add_executable(matplotlib src/matplotlib.cpp)
target_include_directories(matplotlib PRIVATE include ${PYTHON_INCLUDE_DIRS})
target_link_libraries(matplotlib ${PYTHON_LIBRARIES})
