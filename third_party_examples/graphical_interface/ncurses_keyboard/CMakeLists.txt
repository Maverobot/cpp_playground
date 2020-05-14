cmake_minimum_required(VERSION 2.8.3)
project(ncurses_keyboard)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

add_executable(ncurses_keyboard src/main.cpp)

target_link_libraries(ncurses_keyboard PUBLIC ${CURSES_LIBRARIES} Threads::Threads)

target_include_directories(ncurses_keyboard SYSTEM PUBLIC ${CURSES_INCLUDE_DIR})
