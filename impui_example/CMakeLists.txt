cmake_minimum_required(VERSION 2.8.3)
project(impui_example LANGUAGES CXX)

# Download impui
include(FetchContent)
FetchContent_Declare(
  impui
  GIT_REPOSITORY https://github.com/Maverobot/impui.git
  GIT_TAG master)
FetchContent_GetProperties(impui)
if(NOT impui_POPULATED)
  FetchContent_Populate(impui)
  add_subdirectory(${impui_SOURCE_DIR})
endif()

add_executable(simple_plot_example src/simple_plot_example.cpp)
target_include_directories(
  simple_plot_example PRIVATE ${impui_SOURCE_DIR}/imgui
                              ${impui_SOURCE_DIR}/include)

target_link_libraries(simple_plot_example ImPui::impui)

target_compile_features(simple_plot_example PRIVATE cxx_std_14)
