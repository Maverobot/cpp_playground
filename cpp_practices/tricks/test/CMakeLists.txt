# Download catch2 and prepare it for usage
include(../cmake/FetchCatch2.cmake)

# Build tests
set(TEST_SOURCES tests_main.cpp delegate_benchmark_test.cpp)
add_executable(tests ${TEST_SOURCES})

target_link_libraries(tests Catch)
target_include_directories(tests PRIVATE ../include)
target_compile_features(tests PRIVATE cxx_std_14)

# Let ctest discover the tests
include(Catch)
catch_discover_tests(tests)
