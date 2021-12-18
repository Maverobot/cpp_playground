cmake_minimum_required(VERSION 3.4)
project(libbabeltrace2_examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/AliasPkgConfigTarget.cmake)

find_package(PkgConfig QUIET)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(babeltrace2 QUIET IMPORTED_TARGET babeltrace2)
  if (TARGET PkgConfig::babeltrace2)
    alias_pkg_config_target(babeltrace2::babeltrace2 PkgConfig::babeltrace2)
  endif ()
endif ()

add_executable(read_trace_file read_trace_file.cpp)
target_link_libraries(read_trace_file PRIVATE babeltrace2::babeltrace2)
