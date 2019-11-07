cmake_minimum_required(VERSION 3.4)
project(design_pattern_in_modern_cpp)

set(CMAKE_BUILD_TYPE Release)

add_executable(decorator_mixins src/decorator_mixins.cpp)
target_compile_features(decorator_mixins PRIVATE cxx_std_17)

add_executable(builder src/builder.cpp)
target_compile_features(builder PRIVATE cxx_std_17)

add_executable(factory src/factory.cpp)
target_compile_features(factory PRIVATE cxx_std_17)
