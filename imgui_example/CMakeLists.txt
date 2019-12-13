cmake_minimum_required(VERSION 3.4)
project(imgui_example)

set(CMAKE_BUILD_TYPE Release)

find_package(glfw3 REQUIRED)
find_package(OpenGL)

add_library(
  imgui SHARED
  imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp
  imgui/imgui_demo.cpp imgui/imgui_impl_opengl2.cpp imgui/imgui_impl_glfw.cpp)
target_link_libraries(imgui PUBLIC glfw ${OPENGL_LIBRARIES})
target_include_directories(imgui PUBLIC imgui)

add_executable(imgui_example src/imgui_example.cpp)
target_include_directories(imgui_example PRIVATE imgui)
target_link_libraries(imgui_example imgui)
set_target_properties(imgui_example PROPERTIES CXX_STANDARD 17)
