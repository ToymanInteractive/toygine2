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

# Enable C++23 Globally
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(DevkitPro)

if (NOT DEFINED TOYGINE_TARGET_PLATFORM)
  if (CMAKE_SYSTEM_NAME MATCHES Windows)

    set(AVAILABLE_PLATFORMS "Windows Desktop")

  elseif (UNIX AND NOT APPLE)

    set(AVAILABLE_PLATFORMS "Linux Desktop")

  elseif (APPLE)

    set(AVAILABLE_PLATFORMS "macOS Desktop")

  endif ()

  # Add devkitPro platforms if found
  if (DEVKITPRO_GBA_FOUND)
    list(APPEND AVAILABLE_PLATFORMS "Nintendo Game Boy Advance")
  endif (DEVKITPRO_GBA_FOUND)

  if (DEVKITPRO_NDS_FOUND)
    list(APPEND AVAILABLE_PLATFORMS "Nintendo DS")
  endif (DEVKITPRO_NDS_FOUND)

  if (DEVKITPRO_3DS_FOUND)
    list(APPEND AVAILABLE_PLATFORMS "Nintendo 3DS")
  endif (DEVKITPRO_3DS_FOUND)

  if (DEVKITPRO_SWITCH_FOUND)
    list(APPEND AVAILABLE_PLATFORMS "Nintendo Switch")
  endif (DEVKITPRO_SWITCH_FOUND)

  # Report available platforms
  message(STATUS "Available platforms: ${AVAILABLE_PLATFORMS}")

  list(GET AVAILABLE_PLATFORMS 0 FIRST_PLATFORM)
  set(TOYGINE_TARGET_PLATFORM ${FIRST_PLATFORM} CACHE STRING "Select destination TOYGINE_TARGET_PLATFORM for build")
  set_property(CACHE TOYGINE_TARGET_PLATFORM PROPERTY STRINGS ${AVAILABLE_PLATFORMS})
endif ()
