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

cmake_minimum_required(VERSION 3.27.0 FATAL_ERROR)

project(ToyGine2 LANGUAGES C CXX VERSION 26.16.0 HOMEPAGE_URL https://github.com/ToymanInteractive/toygine2)

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

# Disable automatic re-run of CMake when project files change (drops the ZERO_CHECK target on all generators)
set(CMAKE_SUPPRESS_REGENERATION TRUE)

# Enable projects folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#-----------------------------------------------------------------------------------------------------------------------
# User-provided options
#-----------------------------------------------------------------------------------------------------------------------

# Setup configuration types
set(CMAKE_CONFIGURATION_TYPES "Debug;RelWithDebInfo;Release" CACHE STRING "")

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
include(ConfigureCompiler)

# Enable and copy compile commands file in fixed space for static analysis tools. The copy runs at build time
# because CMake writes compile_commands.json during the generate phase, after this script is processed.
# Only Makefile and Ninja generators export compile_commands.json; Xcode and Visual Studio ignore the variable.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if (CMAKE_GENERATOR MATCHES "Makefiles|Ninja")
  add_custom_target(copy_compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_BINARY_DIR}/compile_commands.json
      ${CMAKE_BINARY_DIR}/../compile_commands.json
    VERBATIM)
endif ()

#-----------------------------------------------------------------------------------------------------------------------

set(SRC_CORE_LIST
    src/core/utils.cpp)
set(HDR_CORE_LIST
    include/core.hpp
    include/core/assertion.hpp)
set(INL_CORE_LIST )

source_group("Core" FILES ${SRC_CORE_LIST} ${HDR_CORE_LIST} ${INL_CORE_LIST})

#-----------------------------------------------------------------------------------------------------------------------

list(APPEND SRC_LIST ${SRC_CORE_LIST})
list(APPEND HDR_LIST ${HDR_CORE_LIST} include/toygine.hpp)
list(APPEND INL_LIST ${INL_CORE_LIST})
set(LIB_LIST ${LIB_LIST})

add_library(${TOYGINE_LIBRARY_NAME} STATIC ${SRC_LIST} ${HDR_LIST} ${INL_LIST})

target_include_directories(${TOYGINE_LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (PREPROCESSOR_DEFINITIONS)
  target_compile_definitions(${TOYGINE_LIBRARY_NAME} PRIVATE ${PREPROCESSOR_DEFINITIONS})
endif ()

target_link_libraries(${TOYGINE_LIBRARY_NAME} ${LIB_LIST})

#-----------------------------------------------------------------------------------------------------------------------
# Installation
#-----------------------------------------------------------------------------------------------------------------------

# Only a top-level build installs. The engine exports no CMake package yet, so these rules exist to feed CPack (see
# Packaging below); a parent project embedding toygine keeps its own install layout untouched.
if (TOP_PROJECT)
  install(TARGETS ${TOYGINE_LIBRARY_NAME}
    ARCHIVE DESTINATION lib
    COMPONENT library)

  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION include
    COMPONENT headers
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.inl")

  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
    DESTINATION .
    COMPONENT license)
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Benchmarks
#-----------------------------------------------------------------------------------------------------------------------

if (TOYGINE_BUILD_BENCHMARKS)
  add_subdirectory(benchmarks)
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Documentation
#-----------------------------------------------------------------------------------------------------------------------

if (TOYGINE_BUILD_DOCS)
  add_subdirectory(docs)
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Samples
#-----------------------------------------------------------------------------------------------------------------------

if (TOYGINE_BUILD_SAMPLES)
  add_subdirectory(samples)
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Unit tests
#-----------------------------------------------------------------------------------------------------------------------

if (TOYGINE_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Packaging
#-----------------------------------------------------------------------------------------------------------------------

# Packs whatever the build installed: the library, its headers and the license. The 'runtime' component arrives with the
# sample install rules. include(CPack) must stay last, after every install() rule, otherwise the component list it
# captures is incomplete.
if (TOP_PROJECT)
  set(CPACK_GENERATOR "ZIP")
  set(CPACK_PACKAGE_NAME "${PROJECT_NAME}${PACKAGE_NAME_SUFFIX}")
  set(CPACK_PACKAGE_VENDOR "Toyman Interactive")
  set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
  set(CPACK_PACKAGE_HOMEPAGE_URL ${PROJECT_HOMEPAGE_URL})
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ToyGine 2 static library, headers and samples")
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
  # Component install mode is what filters the archive down to CPACK_COMPONENTS_ALL: without it CPack packs every
  # install() rule in the build, including those the FetchContent dependencies declare for themselves. Grouping puts
  # the components back into a single archive instead of one per component.
  set(CPACK_COMPONENTS_ALL headers library license)
  set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
  set(CPACK_COMPONENTS_GROUPING ALL_COMPONENTS_IN_ONE)

  # Component mode reads this one instead of CPACK_INCLUDE_TOPLEVEL_DIRECTORY, and defaults it off: unpacking would
  # spill include/, lib/ and LICENSE into the current directory.
  set(CPACK_COMPONENT_INCLUDE_TOPLEVEL_DIRECTORY ON)
  set(CPACK_VERBATIM_VARIABLES ON)

  # PACKAGE_FILE_NAME arrives from CI through the preset ($env{BUILD_FILE_NAME}) and carries the configuration and the
  # commit, for example 'linux-x64-a131ded'. Project name and version are prepended here because CI cannot read them
  # out of project(). Local builds get a descriptive name instead.
  if (PACKAGE_FILE_NAME)
    set(_base_name "${CPACK_PACKAGE_NAME}-${PROJECT_VERSION}-${PACKAGE_FILE_NAME}")
  else ()
    set(_base_name "${CPACK_PACKAGE_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
  endif ()

  set(CPACK_PACKAGE_FILE_NAME "${_base_name}")

  include(CPack)
endif ()
