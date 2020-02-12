cmake_minimum_required(VERSION 3.4)
project(design_pattern_in_modern_cpp)

add_executable(decorator_mixins src/decorator_mixins.cpp)
target_compile_features(decorator_mixins PRIVATE cxx_std_17)

add_executable(builder src/builder.cpp)
target_compile_features(builder PRIVATE cxx_std_17)

add_executable(factory src/factory.cpp)
target_compile_features(factory PRIVATE cxx_std_17)

add_executable(factory_experimental src/factory_experimental.cpp)
target_compile_features(factory_experimental PRIVATE cxx_std_17)

add_executable(visitor src/visitor.cpp)
target_compile_features(visitor PRIVATE cxx_std_17)
