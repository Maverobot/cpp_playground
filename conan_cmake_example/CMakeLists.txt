cmake_minimum_required(VERSION 2.8.12)
project(conan_example)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Equivalent to running 'conan install ..' in the binary dir?
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.13/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake")
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(CONANFILE conanfile.txt
  BUILD missing)
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})

# Set properies and INCLUDE_DIRS, therefore no 'target_include_directories' is needed
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(dlib_example src/dlib_example.cpp)
target_link_libraries(dlib_example ${CONAN_LIBS})

add_executable(mlpack_example src/mlpack_example.cpp)
target_link_libraries(mlpack_example ${CONAN_LIBS})
