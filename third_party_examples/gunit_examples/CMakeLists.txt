cmake_minimum_required(VERSION 3.11)
project(gunit_examples)

include(FetchContent)
# FetchContent_Declare(
#   googletest
#   # Specify the commit you depend on and update it regularly.
#   URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
# )
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  GUnit
  # Specify the commit you depend on and update it regularly.
  GIT_REPOSITORY https://github.com/cpp-testing/GUnit.git
  GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(GUnit)

add_executable(example example.cpp)
target_link_libraries(example gunit)
add_test(NAME example_test COMMAND example)
