cmake_minimum_required(VERSION 3.11)
project(uwebsockets)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Fetch uwebsockets
include(FetchContent)
FetchContent_Declare(uwebsockets GIT_REPOSITORY https://github.com/uNetworking/uWebSockets.git)
FetchContent_GetProperties(uwebsockets)
if(NOT uwebsockets_POPULATED)
  FetchContent_Populate(uwebsockets)
endif()

# Add libaries from uwebsockets
add_library(
  usockets
  ${uwebsockets_SOURCE_DIR}/uSockets/src/bsd.c
  ${uwebsockets_SOURCE_DIR}/uSockets/src/context.c
  ${uwebsockets_SOURCE_DIR}/uSockets/src/loop.c
  ${uwebsockets_SOURCE_DIR}/uSockets/src/socket.c
  ${uwebsockets_SOURCE_DIR}/uSockets/src/crypto/openssl.c
  ${uwebsockets_SOURCE_DIR}/uSockets/src/crypto/sni_tree.cpp
  ${uwebsockets_SOURCE_DIR}/uSockets/src/crypto/wolfssl.c
  ${uwebsockets_SOURCE_DIR}/uSockets/src/eventing/epoll_kqueue.c
  ${uwebsockets_SOURCE_DIR}/uSockets/src/eventing/gcd.c
  ${uwebsockets_SOURCE_DIR}/uSockets/src/eventing/libuv.c)
target_include_directories(usockets PUBLIC ${uwebsockets_SOURCE_DIR}/uSockets/src)
target_compile_options(usockets PRIVATE -flto -O3)
target_compile_features(usockets PRIVATE cxx_std_11)
target_compile_definitions(usockets PRIVATE -DLIBUS_NO_SSL)
add_library(uwebsockets INTERFACE)
target_include_directories(uwebsockets INTERFACE ${uwebsockets_SOURCE_DIR}/src)
target_link_libraries(uwebsockets INTERFACE usockets z)
target_compile_features(uwebsockets INTERFACE cxx_std_20)
target_compile_options(uwebsockets INTERFACE -flto -O3)

# Examples
add_executable(hello_world src/hello_world.cpp)
target_link_libraries(hello_world PRIVATE uwebsockets)

add_executable(echo_server src/echo_server.cpp)
target_link_libraries(echo_server PRIVATE uwebsockets)

add_executable(broadcast src/broadcast.cpp)
target_link_libraries(broadcast PRIVATE uwebsockets)

add_executable(broadcast_echo_server src/broadcast_echo_server.cpp)
target_link_libraries(broadcast_echo_server PRIVATE uwebsockets)

add_executable(threaded_broadcast src/threaded_broadcast.cpp)
target_link_libraries(threaded_broadcast PRIVATE uwebsockets pthread)

add_executable(threaded_multiple_broadcasters src/threaded_multiple_broadcasters.cpp)
target_link_libraries(threaded_multiple_broadcasters PRIVATE uwebsockets pthread)
