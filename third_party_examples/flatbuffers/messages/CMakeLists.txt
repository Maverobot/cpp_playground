file(GLOB MESSAGES_FBS ${CMAKE_CURRENT_SOURCE_DIR}/*.fbs)

# Code Generation
foreach(fbs_file ${MESSAGES_FBS})
  message(STATUS ${fbs_file})
  get_filename_component(fb_file_abs ${fbs_file} ABSOLUTE)
  get_filename_component(basename ${fbs_file} NAME_WE)
  set(generated_files ${basename}_generated.h)

  add_custom_command(
    OUTPUT ${generated_files}
    COMMAND flatbuffers::flatc -o ${CMAKE_CURRENT_BINARY_DIR}/messages --cpp ${fb_file_abs}
    COMMENT "Generating ${generated_files} from ${fbs_file}"
    VERBATIM)
endforeach()

add_library(messages INTERFACE)
target_include_directories(messages INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/messages)
target_link_libraries(messages INTERFACE flatbuffers::flatbuffers)
target_include_directories(messages INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/..)
