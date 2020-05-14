cmake_minimum_required(VERSION 3.4)
project(matplotlib_cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Python_ADDITIONAL_VERSIONS 3.6)
find_package(PythonLibs 3 REQUIRED)

add_executable(matplotlib src/matplotlib.cpp)
target_include_directories(matplotlib PRIVATE include ${PYTHON_INCLUDE_DIRS})
target_link_libraries(matplotlib ${PYTHON_LIBRARIES})
