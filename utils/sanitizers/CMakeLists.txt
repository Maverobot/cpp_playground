cmake_minimum_required(VERSION 3.4)
project(sanitizers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_CXX_FLAGS_DEBUG
#   "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -static-libasan")
# set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer
# -fsanitize=address -fsanitize=undefined -static-libasan")

include(CheckCXXCompilerFlag)

set(ASan_FLAGS -g -O1 -fsanitize=address)
set(UBSan_FLAGS -g -O1 -fsanitize=undefined -fno-sanitize=alignment -fno-sanitize-recover=undefined)
set(TSan_FLAGS -g -O1 -fsanitize=thread)

add_executable(address_sanitizer address_sanitizer.cpp)
target_link_libraries(address_sanitizer PUBLIC ${ASan_FLAGS} ${UBSan_FLAGS})
target_compile_options(address_sanitizer PUBLIC ${ASan_FLAGS} ${UBSan_FLAGS})


find_package(Threads REQUIRED)
add_executable(thread_sanitizer thread_sanitizer.cpp)
target_link_libraries(thread_sanitizer PUBLIC ${TSan_FLAGS} Threads::Threads)
target_compile_options(thread_sanitizer PUBLIC ${TSan_FLAGS})
