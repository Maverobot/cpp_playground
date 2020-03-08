cmake_minimum_required(VERSION 2.8.3)
project(ethercat_user)

add_executable(ethercat_user main.cpp)
target_link_libraries(ethercat_user ethercat)
target_compile_features(ethercat_user PRIVATE cxx_std_17)
