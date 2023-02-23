include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.13.9)
FetchContent_GetProperties(catch2)
if(NOT catch2_POPULATED)
  FetchContent_Populate(catch2)
endif()
# Prepare "Catch" library for other executables
set(CATCH_INCLUDE_DIR ${catch2_SOURCE_DIR}/single_include)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${catch2_SOURCE_DIR}/contrib)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})
target_compile_definitions(Catch INTERFACE CATCH_CONFIG_ENABLE_BENCHMARKING)
