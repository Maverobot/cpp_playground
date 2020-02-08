cmake_minimum_required(VERSION 2.8.3)
project(tricks)

add_executable(indices_trick src/indices_trick.cpp)
target_include_directories(indices_trick PUBLIC include)
target_compile_features(indices_trick PRIVATE cxx_std_17)

add_executable(unwrap_reference src/unwrap_reference.cpp)
target_compile_features(unwrap_reference PRIVATE cxx_std_17)

find_package(Eigen3 REQUIRED)
add_executable(struct_magic_example src/struct_magic_example.cpp)
target_include_directories(
  struct_magic_example PRIVATE include third_party/magic_get/include
                               ${EIGEN3_INCLUDE_DIR})
target_compile_features(struct_magic_example PRIVATE cxx_std_14)
