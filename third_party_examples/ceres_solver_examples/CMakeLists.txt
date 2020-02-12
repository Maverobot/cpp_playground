cmake_minimum_required(VERSION 3.0)

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
huntergate(URL "https://github.com/cpp-pm/hunter/archive/v0.23.215.tar.gz" SHA1
           "6969b8330f8db140196d567dd0c570f8789c3c4e" LOCAL)

project(ceres_solver_examples)

set(CMAKE_CXX_STANDARD 17)

find_package(PythonLibs 2.7)

hunter_add_package(ceres-solver)
find_package(Ceres CONFIG REQUIRED)

add_executable(hello_world src/hello_world.cpp)
target_link_libraries(hello_world PUBLIC ceres)

add_executable(quintic_polynomial src/quintic_polynomial.cpp)
target_include_directories(quintic_polynomial PRIVATE include
                                                      ${PYTHON_INCLUDE_DIRS})
target_link_libraries(quintic_polynomial PUBLIC ceres ${PYTHON_LIBRARIES})

add_executable(polynomial_trajectory_generator
               src/polynomial_trajectory_generator.cpp)
target_include_directories(polynomial_trajectory_generator
                           PRIVATE include ${PYTHON_INCLUDE_DIRS})
target_link_libraries(polynomial_trajectory_generator
                      PUBLIC ceres ${PYTHON_LIBRARIES})
