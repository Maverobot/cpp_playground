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
  URL "https://github.com/cpp-pm/hunter/archive/v0.24.2.tar.gz"
  SHA1 "17fd3b9f0b8c271eac9b71e30a6aa1b9fecab299"
  LOCAL # This tells hunter to search for cmake/Hunter/config.cmake as local configuration
)

project(beast)

# Hunter is needed for Ubuntu older than Ubuntu20.04.
hunter_add_package(Boost COMPONENTS)
find_package(Boost CONFIG REQUIRED)

find_package(Threads REQUIRED)

add_executable(websocket_client websocket_client.cpp)
target_link_libraries(websocket_client Boost::boost Threads::Threads)
