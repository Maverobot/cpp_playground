# Copyright (c) 2015, Ruslan Baratov All rights reserved.

cmake_minimum_required(VERSION 3.0)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/HunterGate.cmake")
  message(
    STATUS
      "Downloading HunterGate.cmake from https://raw.githubusercontent.com/hunter-packages/gate/master/cmake/HunterGate.cmake"
  )
  file(DOWNLOAD
       "https://raw.githubusercontent.com/hunter-packages/gate/master/cmake/HunterGate.cmake"
       "${CMAKE_BINARY_DIR}/HunterGate.cmake")
endif()
include(${CMAKE_BINARY_DIR}/HunterGate.cmake)
huntergate(
  URL "https://github.com/cpp-pm/hunter/archive/v0.23.215.tar.gz"
  SHA1 "6969b8330f8db140196d567dd0c570f8789c3c4e"
  LOCAL # This tells hunter to search for cmake/Hunter/config.cmake as local configuration
)

project(download-dlib)

hunter_add_package(dlib)
find_package(dlib CONFIG REQUIRED)

add_executable(dlib_gui_ex src/dlib_gui_ex.cpp)
target_link_libraries(dlib_gui_ex PUBLIC dlib::dlib)
