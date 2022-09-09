cmake_minimum_required(VERSION 3.8.0)
project(dlib_examples)
include(FetchContent)
FetchContent_Declare(
  dlib
  GIT_REPOSITORY https://github.com/davisking/dlib.git
  GIT_TAG v19.18)
FetchContent_MakeAvailable(dlib)

add_executable(svr_ex svr_ex.cpp)
target_link_libraries(svr_ex dlib::dlib)
