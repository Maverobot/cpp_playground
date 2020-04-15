file(GLOB MESSAGES_PROTOS ${CMAKE_CURRENT_SOURCE_DIR}/*.proto)
set(MESSAGES_SRCS)
set(MESSAGES_HDRS)

set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

# Code Generation
foreach(proto_file ${MESSAGES_PROTOS})
  get_filename_component(proto_file_abs ${proto_file} ABSOLUTE)
  get_filename_component(basename ${proto_file} NAME_WE)
  set(generated_files ${basename}.pb.cc ${basename}.pb.h ${basename}.grpc.pb.cc
                      ${basename}.grpc.pb.h)

  list(APPEND MESSAGES_SRCS ${basename}.pb.cc ${basename}.grpc.pb.cc)
  list(APPEND MESSAGES_HDRS ${basename}.pb.h ${basename}.grpc.pb.h)

  add_custom_command(
    OUTPUT ${generated_files}
    COMMAND
      protobuf::protoc --cpp_out ${CMAKE_CURRENT_BINARY_DIR} --grpc_out ${CMAKE_CURRENT_BINARY_DIR}
      -I ${CMAKE_CURRENT_SOURCE_DIR} --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
      ${proto_file_abs}
    COMMENT "Generating ${generated_files} from ${proto_file}"
    VERBATIM)
endforeach()

add_library(messages ${MESSAGES_SRCS})
target_link_libraries(messages protobuf::libprotobuf gRPC::grpc)
target_include_directories(messages PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/..)
