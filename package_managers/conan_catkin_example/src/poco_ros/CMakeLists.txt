cmake_minimum_required(VERSION 2.8.3)
project(poco_ros)
add_compile_options(-std=c++11)

# ##################################################################################################
# install conan dependencies ##
# ##################################################################################################

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.13/conan.cmake"
       "${CMAKE_BINARY_DIR}/conan.cmake")
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(CONANFILE conanfile.txt BUILD missing)
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})

# ##################################################################################################
# Find Dependencies ##
# ##################################################################################################
find_package(catkin REQUIRED COMPONENTS roscpp)
find_package(Poco REQUIRED Foundation Net NetSSL JSON)

# ##################################################################################################
# Package ##
# ##################################################################################################

catkin_package(LIBRARIES ${PROJECT_NAME} CATKIN_DEPENDS roscpp)

# ##################################################################################################
# Build ##
# ##################################################################################################

add_executable(${PROJECT_NAME}_node src/main.cpp)
target_include_directories(${PROJECT_NAME}_node PRIVATE ${catkin_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_node PRIVATE Poco::Foundation ${catkin_LIBRARIES})

# ##################################################################################################
# Install ##
# ##################################################################################################

# Mark executables and/or libraries for installation
install(
  TARGETS ${PROJECT_NAME}_node
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
