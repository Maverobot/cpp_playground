cmake_minimum_required(VERSION 3.11)
project(sml_examples)

add_library(sml INTERFACE)
target_include_directories(sml INTERFACE 3rdParty/sml)

add_executable(hello_world src/hello_world.cpp)
target_include_directories(hello_world PRIVATE include)
target_link_libraries(hello_world PRIVATE sml)

add_executable(child_parent_events src/child_parent_events.cpp)
target_include_directories(child_parent_events PRIVATE include)
target_link_libraries(child_parent_events PRIVATE sml)

add_executable(dump_uml src/dump_uml.cpp)
target_include_directories(dump_uml PRIVATE include)
target_link_libraries(dump_uml PRIVATE sml)
target_compile_features(dump_uml PRIVATE cxx_std_17)
