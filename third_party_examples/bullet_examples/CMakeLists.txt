cmake_minimum_required(VERSION 3.2)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/HunterGate.cmake")
  message(
    STATUS
      "Downloading HunterGate.cmake from https://raw.githubusercontent.com/hunter-packages/gate/master/cmake/HunterGate.cmake"
  )
  file(
    DOWNLOAD
    "https://raw.githubusercontent.com/hunter-packages/gate/master/cmake/HunterGate.cmake"
    "${CMAKE_BINARY_DIR}/HunterGate.cmake")
endif()
include(${CMAKE_BINARY_DIR}/HunterGate.cmake)
HunterGate(
  URL "https://github.com/cpp-pm/hunter/archive/v0.23.251.tar.gz"
  SHA1 "5659b15dc0884d4b03dbd95710e6a1fa0fc3258d")
project(download-magnum)

# The part below is adapted from https://github.com/mosra/magnum-
# examples/blob/master/src/primitives/CMakeLists.txt

project(bullet_examples)

hunter_add_package(bullet)
find_package(bullet CONFIG REQUIRED)

add_executable(boo boo.cpp)
target_link_libraries(
  boo
  PUBLIC
  bullet::BulletCollision
  bullet::BulletDynamics
  bullet::BulletSoftBody
  bullet::LinearMath)
