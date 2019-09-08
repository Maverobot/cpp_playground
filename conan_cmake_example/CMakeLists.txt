cmake_minimum_required(VERSION 2.8.12)
project(conan_example)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(conan_example src/main.cpp)
target_link_libraries(conan_example ${CONAN_LIBS})
