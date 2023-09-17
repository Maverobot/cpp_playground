cmake_minimum_required(VERSION 2.8.3)
project(expression_templates)

add_library(exprtmpl_array INTERFACE)
target_include_directories(exprtmpl_array INTERFACE include)

add_executable(expression_templates src/expression_templates.cpp)
target_compile_features(expression_templates PRIVATE cxx_std_17)
target_compile_options(expression_templates PRIVATE -Werror -Wall)
target_link_libraries(expression_templates PRIVATE exprtmpl_array)


set(ASan_FLAGS -g -O1 -fsanitize=address)
set(UBSan_FLAGS -g -O1 -fsanitize=undefined -fno-sanitize=alignment -fno-sanitize-recover=undefined)

target_link_libraries(expression_templates PUBLIC ${ASan_FLAGS} ${UBSan_FLAGS})
target_compile_options(expression_templates PUBLIC ${ASan_FLAGS} ${UBSan_FLAGS})

enable_testing()
add_subdirectory(test)
