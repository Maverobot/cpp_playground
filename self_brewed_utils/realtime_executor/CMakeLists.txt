cmake_minimum_required(VERSION 2.8.3)
project(realtime_executor)

find_package(Threads REQUIRED)

add_executable(realtime_example src/realtime_example.cpp)
target_include_directories(realtime_example PUBLIC include)
target_compile_features(realtime_example PRIVATE cxx_std_14)
target_link_libraries(realtime_example Threads::Threads)
