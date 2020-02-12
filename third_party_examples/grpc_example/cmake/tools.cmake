# MIT License
#
# Copyright (c) 2019 Zheng Qu
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

macro(hunter_init)
  cmake_parse_arguments(HUNTER_INIT "LOCAL" "" "" ${ARGN})
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/HunterGate.cmake")
    message(
      STATUS
        "Downloading HunterGate.cmake from https://raw.githubusercontent.com/hunter-packages/gate/master/cmake/HunterGate.cmake"
    )
    file(
      DOWNLOAD
      "https://raw.githubusercontent.com/hunter-packages/gate/master/cmake/HunterGate.cmake"
      "${CMAKE_BINARY_DIR}/HunterGate.cmake")
  endif()
  include(${CMAKE_BINARY_DIR}/HunterGate.cmake)
  if("${HUNTER_INIT_LOCAL}")
    set(LOCAL_V "LOCAL")
  else()
    set(LOCAL_V "")
  endif()
  huntergate(URL "https://github.com/cpp-pm/hunter/archive/v0.23.215.tar.gz"
             SHA1 "6969b8330f8db140196d567dd0c570f8789c3c4e" ${LOCAL_V})
endmacro()
