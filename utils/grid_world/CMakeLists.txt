cmake_minimum_required(VERSION 3.11)
project(grid_world)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  libfort
  GIT_REPOSITORY https://github.com/seleznevae/libfort.git
  GIT_TAG v0.4.0)
FetchContent_GetProperties(libfort)
if(NOT libfort_POPULATED)
  FetchContent_Populate(libfort)
endif()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${libfort_SOURCE_DIR}/contrib)
add_library(fort ${libfort_SOURCE_DIR}/lib/fort.c)
target_include_directories(fort PUBLIC ${libfort_SOURCE_DIR}/lib)

add_executable(grid_world_example src/grid_world_example.cpp)
target_include_directories(grid_world_example PRIVATE include)
target_link_libraries(grid_world_example fort)
