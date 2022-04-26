cmake_minimum_required(VERSION 3.11)
project(uuid_examples)

include(FetchContent)
FetchContent_Declare(
  boost_uuid
  GIT_REPOSITORY https://github.com/boostorg/uuid.git
  GIT_TAG 3866e721f00b68070880227652c3ffb462ccee18) # boost-1.65.1
FetchContent_GetProperties(boost_uuid)
if(NOT boost_uuid_POPULATED)
  FetchContent_Populate(boost_uuid)
  add_library(local_boost_uuid INTERFACE)
  target_include_directories(
    local_boost_uuid INTERFACE ${boost_uuid_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIR})
endif()

add_executable(example example.cpp)
target_link_libraries(example PRIVATE local_boost_uuid)
