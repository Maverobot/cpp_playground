cmake_minimum_required(VERSION 3.11)
project(toml_examples)

include(ExternalProject)
ExternalProject_Add(
  toml
  GIT_REPOSITORY "https://github.com/ToruNiina/toml11.git"
  GIT_TAG "v3.3.1"
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/toml11"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  EXCLUDE_FROM_ALL TRUE)

add_library(toml11 INTERFACE)
target_include_directories(toml11 INTERFACE ${CMAKE_SOURCE_DIR}/3rdparty/toml11)
add_dependencies(toml11 toml)

add_executable(example example.cpp)
target_link_libraries(example PRIVATE toml11)
