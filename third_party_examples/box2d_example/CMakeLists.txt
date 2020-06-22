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
target_include_directories(imgui_impl PUBLIC include/imgui_impl)
target_link_libraries(imgui_impl PUBLIC imgui glad glfw)

add_library(box2d_draw src/draw src/settings.cpp)
target_include_directories(box2d_draw PUBLIC ${box2d-extern_SOURCE_DIR}/include include/draw)
target_link_libraries(box2d_draw PUBLIC box2d imgui_impl)

add_library(box2dsim INTERFACE)
target_include_directories(box2dsim INTERFACE ${box2d-extern_SOURCE_DIR}/include include/box2dsim)
target_link_libraries(box2dsim INTERFACE box2d_draw)

add_executable(hello src/hello.cpp)
target_link_libraries(hello PRIVATE box2dsim)
