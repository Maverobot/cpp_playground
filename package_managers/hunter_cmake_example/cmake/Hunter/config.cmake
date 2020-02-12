
set(DLIB_CMAKE_ARGS
  DLIB_PNG_SUPPORT=OFF
  DLIB_NO_GUI_SUPPORT=OFF
  DLIB_HEADER_ONLY=OFF #all previous builds were header on, so that is the default
  DLIB_ENABLE_ASSERTS=OFF #must be set on/off or debug/release build will differ and config will not match one
  HUNTER_INSTALL_LICENSE_FILES=dlib/LICENSE.txt
  )

hunter_config(dlib
  # Version tag can be found in project_name/hunter.cmake at https://github.com/ruslo/hunter/tree/master/cmake/projects
  VERSION "19.17-p0"
  CONFIGURATION_TYPES "Release"
  CMAKE_ARGS "${DLIB_CMAKE_ARGS}")
