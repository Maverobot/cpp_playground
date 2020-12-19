cmake_minimum_required(VERSION 3.11)
project(behaviortree_cpp)

include(FetchContent)
FetchContent_Declare(behaviortree_cpp-extern
                     GIT_REPOSITORY https://github.com/BehaviorTree/BehaviorTree.CPP.git)
FetchContent_GetProperties(behaviortree_cpp-extern)
if(NOT behaviortree_cpp-extern_POPULATED)
  FetchContent_Populate(behaviortree_cpp-extern)
  add_subdirectory(${behaviortree_cpp-extern_SOURCE_DIR})
endif()

find_package(BehaviorTreeV3)

add_executable(create_a_tree src/create_a_tree.cpp)
target_link_libraries(create_a_tree PRIVATE BT::behaviortree_cpp_v3)
