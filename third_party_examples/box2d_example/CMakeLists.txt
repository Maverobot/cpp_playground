cmake_minimum_required(VERSION 3.11)
project(box2d_examples)

include(FetchContent)
FetchContent_Declare(box2d-extern GIT_REPOSITORY https://github.com/erincatto/box2d.git)
FetchContent_GetProperties(box2d-extern)
if(NOT box2d-extern_POPULATED)
  FetchContent_Populate(box2d-extern)
  message(STATUS ${box2d-extern_SOURCE_DIR})
  message(STATUS ${box2d-extern_BINARY_DIR})
  add_subdirectory(${box2d-extern_SOURCE_DIR} ${box2d-extern_BINARY_DIR})
endif()

add_library(imgui_impl src/imgui_impl_glfw.cpp src/imgui_impl_opengl3.cpp)
target_link_libraries(imgui_impl PRIVATE imgui glad glfw)

add_executable(hello src/draw.cpp src/hello.cpp src/settings.cpp)
target_include_directories(hello PRIVATE ${box2d-extern_SOURCE_DIR}/include)
target_link_libraries(hello PRIVATE box2d glad imgui imgui_impl)
