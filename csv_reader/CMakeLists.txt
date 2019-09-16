cmake_minimum_required(VERSION 3.4)
project(csv_reader)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(csv_reader_ex
  src/csv_reader_ex.cpp
)
target_include_directories(csv_reader_ex PRIVATE
  include)
target_link_libraries(csv_reader_ex ${CONAN_LIBS})
