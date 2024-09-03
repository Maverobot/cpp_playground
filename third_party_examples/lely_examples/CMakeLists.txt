cmake_minimum_required(VERSION 3.11)
project(lely_examples)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LELY REQUIRED liblely-coapp)

if(NOT LELY_FOUND)
  message(FATAL_ERROR "LELY not found. Try with \n
sudo add-apt-repository ppa:lely/ppa \n
sudo apt-get update \n
sudo apt-get install liblely-coapp-dev liblely-co-tools python3-dcf-tools")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cpp-slave.eds ${CMAKE_BINARY_DIR}/cpp-slave.eds COPYONLY)

find_program(DCFGEN_PROG dcfgen REQUIRED DOC "'dcfgen' executable")
add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/master.dcf
  COMMAND ${DCFGEN_PROG} -r --no-strict ${CMAKE_CURRENT_SOURCE_DIR}/cpp-tutorial.yml
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/cpp-tutorial.yml
  COMMENT "Generate master.dcf file from cpp-tutorial.yml"
  VERBATIM
)
add_custom_target(dcfgen-master-config ALL
  DEPENDS ${CMAKE_BINARY_DIR}/master.dcf
)

add_executable(tutorial tutorial.cpp)
target_link_libraries(tutorial ${LELY_LIBRARIES})

add_executable(slave slave.cpp)
target_link_libraries(slave ${LELY_LIBRARIES})

add_executable(master master.cpp)
target_link_libraries(master ${LELY_LIBRARIES})
add_dependencies(master dcfgen-master-config)

add_executable(master_manual_sync master_manual_sync.cpp)
target_link_libraries(master_manual_sync ${LELY_LIBRARIES})
add_dependencies(master_manual_sync dcfgen-master-config)

add_executable(dcf_parameter_finder dcf_parameter_finder.cpp)
