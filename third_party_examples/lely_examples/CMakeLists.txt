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

function(generate_dcf target)

  find_program(DCFGEN_PROG dcfgen REQUIRED DOC "'dcfgen' executable")
  add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/${target}.dcf
    COMMAND ${CMAKE_COMMAND} -E make_directory /tmp/${target}
    COMMAND ${DCFGEN_PROG} -d /tmp/${target} -r --no-strict ${CMAKE_CURRENT_SOURCE_DIR}/${target}.yml
    COMMAND ${CMAKE_COMMAND} -E copy /tmp/${target}/master.dcf ${CMAKE_BINARY_DIR}/${target}.dcf
    COMMAND ${CMAKE_COMMAND} -E remove /tmp/${target}/master.dcf
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${target}.yml
    COMMENT "Generate ${target}.dcf file from ${target}.yml"
    VERBATIM
  )
  add_custom_target(dcfgen-${target}-config ALL
    DEPENDS ${CMAKE_BINARY_DIR}/${target}.dcf
  )
endfunction()

generate_dcf(master)
generate_dcf(passive-master)

add_executable(tutorial tutorial.cpp)
target_link_libraries(tutorial ${LELY_LIBRARIES})

add_executable(slave slave.cpp)
target_link_libraries(slave ${LELY_LIBRARIES})

add_executable(master master.cpp)
target_link_libraries(master ${LELY_LIBRARIES})
add_dependencies(master dcfgen-master-config dcfgen-passive-master-config)

add_executable(passive_master passive_master.cpp)
target_link_libraries(passive_master ${LELY_LIBRARIES})
add_dependencies(passive_master dcfgen-passive-master-config)

add_executable(master_manual_sync master_manual_sync.cpp)
target_link_libraries(master_manual_sync ${LELY_LIBRARIES})
add_dependencies(master_manual_sync dcfgen-master-config)

add_executable(dcf_parameter_finder dcf_parameter_finder.cpp)
