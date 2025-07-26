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

project(ToyGine2 LANGUAGES CXX VERSION 1.2.3 HOMEPAGE_URL https://github.com/ToymanInteractive/toygine2)

set(TOYGINE_LIBRARY_NAME toygine)
set(TOYGINE_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# Is top level project
set(TOP_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(TOP_PROJECT ON)
endif (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)

# To distinguish between debug and release builds
set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_RELWITHDEBINFO_POSTFIX "rd")

# Set generate relative path  ON
set(CMAKE_SUPPRESS_REGENERATION TRUE)

#-------------------------------------------------------------------------------------------
# User-provided options
#-------------------------------------------------------------------------------------------

# Setup configuration types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo" CACHE STRING "" FORCE)

# Make default 'RelWithDebInfo' build
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif ()

if (TOP_PROJECT)
  option(TOYGINE_BUILD_TESTS "Set when you want to build engine unit tests" ON)
endif ()

#-------------------------------------------------------------------------------------------
# Compile and linking options
#-------------------------------------------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(ConfigureEngine)
include(ConfigureCompiler)

if (TARGET_PLATFORM STREQUAL "Windows Desktop")
  set(TARGET_PLATFORM_CONFIG_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/platforms/windows/platform_config.hpp)
elseif (TARGET_PLATFORM STREQUAL "Linux Desktop")
  set(TARGET_PLATFORM_CONFIG_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/platforms/linux/platform_config.hpp)
elseif (TARGET_PLATFORM STREQUAL "macOS Desktop")
  set(TARGET_PLATFORM_CONFIG_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/platforms/macos/platform_config.hpp)
else ()
  message(FATAL_ERROR "Unsupported TARGET_PLATFORM: ${TARGET_PLATFORM}")
endif ()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/core/core.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/include/core.hpp @ONLY)

#-------------------------------------------------------------------------------------------

set(SRC_CORE_LIST src/core/core_utils.cpp)
set(HDR_CORE_LIST include/core.hpp)
set(INL_CORE_LIST include/core_inline.hpp)

source_group("Core" FILES ${SRC_CORE_LIST} ${HDR_CORE_LIST} ${INL_CORE_LIST})

#-------------------------------------------------------------------------------------------

set(SRC_LIST ${SRC_LIST} ${SRC_CORE_LIST})
set(HDR_LIST ${HDR_LIST} ${HDR_CORE_LIST})
set(INL_LIST ${INL_LIST} ${INL_CORE_LIST})
set(LIB_LIST ${LIB_LIST})

add_library(${TOYGINE_LIBRARY_NAME} STATIC ${SRC_LIST} ${HDR_LIST} ${INL_LIST})

target_include_directories(${TOYGINE_LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (PREPROCESSOR_DEFINITIONS)
  target_compile_definitions(${TOYGINE_LIBRARY_NAME} PRIVATE ${PREPROCESSOR_DEFINITIONS})
endif (PREPROCESSOR_DEFINITIONS)

target_link_libraries(${TOYGINE_LIBRARY_NAME} ${LIB_LIST})

#-------------------------------------------------------------------------------------------
# Unit tests
#-------------------------------------------------------------------------------------------

if (TOYGINE_BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif (TOYGINE_BUILD_TESTS)
