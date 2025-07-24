#-------------------------------------------------------------------------------------------
# Copyright (c) 2025 by Toyman Interactive
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this
# software and associated documentation files (the "Software"), to deal in the Software
# without restriction, including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and / or sell copies of the Software, and to permit
# persons to whom the Software is furnished to do so, subject to the following conditions :
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
# PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
# FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#-------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.31.0 FATAL_ERROR)

# Enable C++20 Globally
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable C 17 Globally
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if (NOT DEFINED TARGET_PLATFORM)
  if (CMAKE_SYSTEM_NAME MATCHES Windows)

    set(AVAILABLE_PLATFORMS "Windows Desktop")

  elseif (UNIX AND NOT APPLE)

    set(AVAILABLE_PLATFORMS "Linux Desktop")

  elseif (APPLE)

    set(AVAILABLE_PLATFORMS "macOS Desktop")

  endif ()

  list(GET AVAILABLE_PLATFORMS 0 FIRST_PLATFORM)
  set(TARGET_PLATFORM ${FIRST_PLATFORM} CACHE STRING "Select destination TARGET_PLATFORM for build")
  set_property(CACHE TARGET_PLATFORM PROPERTY STRINGS ${AVAILABLE_PLATFORMS})
endif ()
