cmake_minimum_required(VERSION 3.11)
project(uwebsocket)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Fetch uwebsocket
include(FetchContent)
FetchContent_Declare(uwebsocket GIT_REPOSITORY https://github.com/uNetworking/uWebSockets.git)
FetchContent_GetProperties(uwebsocket)
if(NOT uwebsocket_POPULATED)
  FetchContent_Populate(uwebsocket)
endif()

# Add libaries from uwebsocket
add_library(
  usockets
  ${uwebsocket_SOURCE_DIR}/uSockets/src/bsd.c
  ${uwebsocket_SOURCE_DIR}/uSockets/src/context.c
  ${uwebsocket_SOURCE_DIR}/uSockets/src/loop.c
  ${uwebsocket_SOURCE_DIR}/uSockets/src/socket.c
  ${uwebsocket_SOURCE_DIR}/uSockets/src/crypto/openssl.c
  ${uwebsocket_SOURCE_DIR}/uSockets/src/crypto/sni_tree.cpp
  ${uwebsocket_SOURCE_DIR}/uSockets/src/crypto/wolfssl.c
  ${uwebsocket_SOURCE_DIR}/uSockets/src/eventing/epoll_kqueue.c
  ${uwebsocket_SOURCE_DIR}/uSockets/src/eventing/gcd.c
  ${uwebsocket_SOURCE_DIR}/uSockets/src/eventing/libuv.c)
target_include_directories(usockets PUBLIC ${uwebsocket_SOURCE_DIR}/uSockets/src)
target_compile_options(usockets PRIVATE -flto -O3)
target_compile_features(usockets PRIVATE cxx_std_11)
target_compile_definitions(usockets PRIVATE -DLIBUS_NO_SSL)
add_library(uwebsocket INTERFACE)
target_include_directories(uwebsocket INTERFACE ${uwebsocket_SOURCE_DIR}/src)
target_link_libraries(uwebsocket INTERFACE usockets z)
target_compile_features(uwebsocket INTERFACE cxx_std_20)
target_compile_options(uwebsocket INTERFACE -flto -O3)

# Examples
add_executable(hello_world src/hello_world.cpp)
target_link_libraries(hello_world PRIVATE uwebsocket)

add_executable(echo_server src/echo_server.cpp)
target_link_libraries(echo_server PRIVATE uwebsocket)

add_executable(broadcast src/broadcast.cpp)
target_link_libraries(broadcast PRIVATE uwebsocket)

add_executable(broadcast_echo_server src/broadcast_echo_server.cpp)
target_link_libraries(broadcast_echo_server PRIVATE uwebsocket)

add_executable(threaded_broadcast src/threaded_broadcast.cpp)
target_link_libraries(threaded_broadcast PRIVATE uwebsocket pthread)

add_executable(threaded_multiple_broadcasters src/threaded_multiple_broadcasters.cpp)
target_link_libraries(threaded_multiple_broadcasters PRIVATE uwebsocket pthread)
