cmake_minimum_required(VERSION 3.4)
project(design_pattern_in_modern_cpp)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_executable(crtp src/crtp.cpp)
target_compile_features(crtp PRIVATE cxx_std_17)

add_executable(decorator_mixins src/decorator_mixins.cpp)
target_compile_features(decorator_mixins PRIVATE cxx_std_17)

add_executable(builder src/builder.cpp)
target_compile_features(builder PRIVATE cxx_std_17)

add_executable(joshua_bloch_builder src/joshua_bloch_builder.cpp)
target_compile_features(joshua_bloch_builder PRIVATE cxx_std_17)

add_executable(factory src/factory.cpp)
target_compile_features(factory PRIVATE cxx_std_17)

add_executable(factory_experimental src/factory_experimental.cpp)
target_compile_features(factory_experimental PRIVATE cxx_std_17)

add_executable(factory_variant src/factory_variant.cpp)
target_compile_features(factory_variant PRIVATE cxx_std_17)

add_executable(visitor src/visitor.cpp)
target_compile_features(visitor PRIVATE cxx_std_17)
