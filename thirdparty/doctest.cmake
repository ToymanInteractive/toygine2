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

# Unit test framework for the engine and editor test runners. doctest.h is the amalgamated single header, with the
# definitions behind DOCTEST_CONFIG_IMPLEMENT; each runner's ctest_main.cpp defines it. The copy keeps that header, the
# two CMake scripts behind doctest_discover_tests() and the license, and drops the upstream CMakeLists, which declares a
# project of its own, builds the examples and tests, and installs a package config. thirdparty/README.md records the
# upstream tag.

# Every test runner includes this file, and the target below must be created only once.
include_guard(GLOBAL)

set(DOCTEST_DIR ${CMAKE_CURRENT_LIST_DIR}/doctest)

# Consumers reach this file through include(doctest), the name of the upstream helper that defines
# doctest_discover_tests(), so the helper comes along with the target.
include(${DOCTEST_DIR}/scripts/cmake/doctest.cmake)

# The compiler finds the header through the include path; listing it puts the copy in the IDE's project tree.
set(HDR_DOCTEST_LIST
  ${DOCTEST_DIR}/doctest/doctest.h
)

add_library(doctest INTERFACE ${HDR_DOCTEST_LIST})

# Keeps the vendored target out of the way in the Xcode and Visual Studio trees; other generators ignore it.
set_target_properties(doctest PROPERTIES FOLDER "thirdparty")

# SYSTEM keeps the project's warnings off headers that are not ours.
target_include_directories(doctest SYSTEM INTERFACE ${DOCTEST_DIR})

add_library(doctest::doctest ALIAS doctest)
