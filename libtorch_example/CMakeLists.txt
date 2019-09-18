cmake_minimum_required(VERSION 3.4)
project(smart_rate)

set(LIBTORCH_PATH "" CACHE PATH "Path to the decompressed libtorch folder.")

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH ${LIBTORCH_PATH})
find_package(Torch CONFIG REQUIRED)

add_executable(libtorch_hello_world
  src/libtorch_hello_world.cpp)
target_include_directories(libtorch_hello_world PRIVATE
  include)
target_link_libraries(libtorch_hello_world ${TORCH_LIBRARIES})

add_executable(dcgan_example
  src/dcgan_example.cpp)
target_include_directories(dcgan_example PRIVATE
  include)
target_link_libraries(dcgan_example ${TORCH_LIBRARIES})
