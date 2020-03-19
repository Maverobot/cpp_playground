cmake_minimum_required(VERSION 2.8.3)
project(tricks)

add_executable(indices_trick src/indices_trick.cpp)
target_include_directories(indices_trick PUBLIC include)
target_compile_features(indices_trick PRIVATE cxx_std_17)

add_executable(unwrap_reference src/unwrap_reference.cpp)
target_compile_features(unwrap_reference PRIVATE cxx_std_17)

add_executable(defer src/defer.cpp)
target_compile_features(defer PRIVATE cxx_std_17)

add_executable(exit_scope src/exit_scope.cpp)
target_include_directories(exit_scope PRIVATE include)
target_compile_features(exit_scope PRIVATE cxx_std_17)

find_package(Eigen3 REQUIRED)

# Download magic_get
include(FetchContent)
FetchContent_Declare(
  magic_get
  GIT_REPOSITORY https://github.com/apolukhin/magic_get.git
  GIT_TAG master)
FetchContent_GetProperties(magic_get)
if(NOT magic_get_POPULATED)
  FetchContent_Populate(magic_get)
endif()

add_executable(struct_magic_example src/struct_magic_example.cpp)
target_include_directories(
  struct_magic_example PRIVATE include ${magic_get_SOURCE_DIR}/include
                               ${EIGEN3_INCLUDE_DIR})
target_compile_features(struct_magic_example PRIVATE cxx_std_14)
