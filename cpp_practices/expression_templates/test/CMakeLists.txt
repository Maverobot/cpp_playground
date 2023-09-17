
include(FetchContent)
# FetchContent_Declare(
#   googletest
#   # Specify the commit you depend on and update it regularly.
#   URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
# )
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

Set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
  GUnit
  # Specify the commit you depend on and update it regularly.
  GIT_REPOSITORY https://github.com/cpp-testing/GUnit.git
  GIT_TAG v1.13.0
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(GUnit)

add_executable(expression_templates_tests main.cpp)
target_link_libraries(expression_templates_tests gunit exprtmpl_array)
add_test(NAME expression_templates_tests COMMAND expression_templates_tests)
