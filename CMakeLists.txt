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

project(ToyGine2 LANGUAGES C CXX VERSION 26.6.0 HOMEPAGE_URL https://github.com/ToymanInteractive/toygine2)

set(TOYGINE_LIBRARY_NAME toygine)
set(TOYGINE_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# Is top level project
set(TOP_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(TOP_PROJECT ON)
endif ()

# To distinguish between debug and release builds
set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_RELWITHDEBINFO_POSTFIX "rd")

# Disable regeneration of build system files by IDEs
set(CMAKE_SUPPRESS_REGENERATION TRUE)

# Enable projects folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#-----------------------------------------------------------------------------------------------------------------------
# User-provided options
#-----------------------------------------------------------------------------------------------------------------------

# Setup configuration types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo" CACHE STRING "")

# Make default 'RelWithDebInfo' build
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif ()

if (TOP_PROJECT)
  include(CMakeDependentOption)

  option(TOYGINE_BUILD_BENCHMARKS "Set when you want to build engine benchmarks" ON)
  option(TOYGINE_BUILD_DOCS "Set when you want to build engine documentation" OFF)
  option(TOYGINE_BUILD_EDITOR "Set when you want to build engine editor (desktop only)" ON)
  option(TOYGINE_BUILD_SAMPLES "Set when you want to build engine samples" ON)
  option(TOYGINE_BUILD_TESTS "Set when you want to build engine unit tests" ON)
  cmake_dependent_option(TOYGINE_TESTS_ENABLE_COVERAGE "Enable code coverage instrumentation for tests" OFF "TOYGINE_BUILD_TESTS" OFF)
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Compile and linking options
#-----------------------------------------------------------------------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(ConfigureEngine)
