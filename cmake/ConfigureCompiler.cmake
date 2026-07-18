#-----------------------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Toyman Interactive
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
#-----------------------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)

if (TOYGINE_TARGET_PLATFORM STREQUAL "Windows Desktop")

  if (MSVC)
    message(STATUS "Compiler: MSVC, version: " ${MSVC_VERSION})
  endif ()

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Linux Desktop")

  message(STATUS "${CMAKE_CXX_COMPILER_ID} version: ${CMAKE_CXX_COMPILER_VERSION}")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "macOS Desktop")

  if (CMAKE_GENERATOR STREQUAL "Xcode")
    message(STATUS "Compiler: Xcode, version: " ${XCODE_VERSION})
  endif ()

  message(STATUS "${CMAKE_CXX_COMPILER_ID} version: ${CMAKE_CXX_COMPILER_VERSION}")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Sega MD")

  if (NOT CLOWNMDSDK_FOUND)
    message(FATAL_ERROR "ClownMDSDK not found. Install ClownMDSDK and ensure CLOWNMDSDK is set.")
  endif ()

  message(STATUS "${CMAKE_CXX_COMPILER_ID} version: ${CMAKE_CXX_COMPILER_VERSION}")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GBA")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

  message(STATUS "${CMAKE_CXX_COMPILER_ID} version: ${CMAKE_CXX_COMPILER_VERSION}")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo DS")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

  message(STATUS "${CMAKE_CXX_COMPILER_ID} version: ${CMAKE_CXX_COMPILER_VERSION}")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo 3DS")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

  message(STATUS "${CMAKE_CXX_COMPILER_ID} version: ${CMAKE_CXX_COMPILER_VERSION}")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Switch")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

  message(STATUS "${CMAKE_CXX_COMPILER_ID} version: ${CMAKE_CXX_COMPILER_VERSION}")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GameCube")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

  message(STATUS "${CMAKE_CXX_COMPILER_ID} version: ${CMAKE_CXX_COMPILER_VERSION}")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Wii")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

  message(STATUS "${CMAKE_CXX_COMPILER_ID} version: ${CMAKE_CXX_COMPILER_VERSION}")

else ()

  message(FATAL_ERROR "Unsupported platform: ${TOYGINE_TARGET_PLATFORM}")

endif ()

if (ToyGine2_VERSION_MAJOR)
  add_compile_definitions(TOYGINE_VERSION_MAJOR=${ToyGine2_VERSION_MAJOR})
endif ()

if (ToyGine2_VERSION_MINOR)
  add_compile_definitions(TOYGINE_VERSION_MINOR=${ToyGine2_VERSION_MINOR})
endif ()

if (ToyGine2_VERSION_PATCH)
  add_compile_definitions(TOYGINE_VERSION_PATCH=${ToyGine2_VERSION_PATCH})
endif ()
