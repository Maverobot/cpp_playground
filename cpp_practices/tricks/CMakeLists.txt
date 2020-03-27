cmake_minimum_required(VERSION 3.11)
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

# Download struct_magic if it cannot be found locally
set(struct_magic_VERSION 0.1)
find_package(struct_magic ${struct_magic_VERSION} EXACT QUIET CONFIG)
if(NOT struct_magic_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    struct_magic
    GIT_REPOSITORY https://github.com/Maverobot/struct_magic.git
    GIT_TAG v${struct_magic_VERSION})
  FetchContent_GetProperties(struct_magic)
  if(NOT struct_magic_POPULATED)
    message(STATUS "Fetching struct_magic...")
    FetchContent_Populate(struct_magic)
    add_subdirectory(${struct_magic_SOURCE_DIR} ${struct_magic_BINARY_DIR})
    message(STATUS "struct_magic has been fetched.")
  endif()
endif()

add_executable(struct_magic_example src/struct_magic_example.cpp)
target_include_directories(struct_magic_example PRIVATE ${EIGEN3_INCLUDE_DIR})
target_link_libraries(struct_magic_example PRIVATE struct_magic)
target_compile_features(struct_magic_example PRIVATE cxx_std_14)
