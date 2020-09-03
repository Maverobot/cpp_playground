cmake_minimum_required(VERSION 3.5.1)

# pre-compiled version of drake was built with gcc-7 on ubuntu 18.04 Errors will happen with higher
# version of gcc and clang
set(CMAKE_C_COMPILER /usr/bin/gcc-7)
set(CMAKE_CXX_COMPILER /usr/bin/g++-7)

project(drake_examples)

find_package(drake CONFIG REQUIRED)
find_package(gflags CONFIG REQUIRED)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(cart_pole)
add_subdirectory(acrobot)
