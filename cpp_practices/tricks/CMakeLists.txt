cmake_minimum_required(VERSION 3.11)
project(tricks)

option(ENABLE_TESTING "Enable testing for this project" ON)

add_executable(indices_trick src/indices_trick.cpp)
target_include_directories(indices_trick PUBLIC include)
target_compile_features(indices_trick PRIVATE cxx_std_17)

add_executable(delegate_example src/delegate_example.cpp)
target_include_directories(delegate_example PUBLIC include)
target_compile_features(delegate_example PRIVATE cxx_std_17)

add_executable(unwrap_reference src/unwrap_reference.cpp)
target_compile_features(unwrap_reference PRIVATE cxx_std_17)

add_executable(class_template_specialization_type_list src/class_template_specialization_type_list.cpp)
target_compile_features(class_template_specialization_type_list PRIVATE cxx_std_17)

add_executable(char src/char.cpp)
target_compile_features(char PRIVATE cxx_std_17)

add_executable(sfinae src/sfinae.cpp)
target_compile_features(sfinae PRIVATE cxx_std_17)

add_executable(defer src/defer.cpp)
target_compile_features(defer PRIVATE cxx_std_17)

add_executable(flatten_tuple src/flatten_tuple.cpp)
target_compile_features(flatten_tuple PRIVATE cxx_std_17)

add_executable(is_valid src/is_valid.cpp)
target_compile_features(is_valid PRIVATE cxx_std_17)

add_executable(exit_scope src/exit_scope.cpp)
target_include_directories(exit_scope PRIVATE include)
target_compile_features(exit_scope PRIVATE cxx_std_17)

add_executable(type_erasure src/type_erasure.cpp)
target_compile_features(type_erasure PRIVATE cxx_std_17)

add_executable(integer_to_bool_array src/integer_to_bool_array.cpp)
target_compile_features(integer_to_bool_array PRIVATE cxx_std_17)

add_executable(reservoir_sampling src/reservoir_sampling.cpp)
target_compile_features(reservoir_sampling PRIVATE cxx_std_17)

add_executable(bitset_to_int src/bitset_to_int.cpp)
target_compile_features(bitset_to_int PRIVATE cxx_std_17)

add_executable(perfect_forwarding src/perfect_forwarding.cpp)
target_compile_features(perfect_forwarding PRIVATE cxx_std_17)

add_executable(longjmp src/longjmp.cpp)
add_executable(siglongjmp src/siglongjmp.cpp)

find_package(Eigen3 REQUIRED)

# Download struct_magic if it cannot be found locally
set(struct_magic_VERSION 0.2.1)
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
    set(ENABLE_TESTING_SAVED "${ENABLE_TESTING}")
    set(ENABLE_TESTING OFF)
    add_subdirectory(${struct_magic_SOURCE_DIR} ${struct_magic_BINARY_DIR})
    set(ENABLE_TESTING "${ENABLE_TESTING_SAVED}")
    message(STATUS "struct_magic has been fetched.")
  endif()
endif()

add_executable(struct_magic_example src/struct_magic_example.cpp)
target_include_directories(struct_magic_example PRIVATE ${EIGEN3_INCLUDE_DIR})
target_link_libraries(struct_magic_example PRIVATE struct_magic)
target_compile_features(struct_magic_example PRIVATE cxx_std_14)

# Test
if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()
