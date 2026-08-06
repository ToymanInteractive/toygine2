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

#-----------------------------------------------------------------------------------------------------------------------
# CTest support
#-----------------------------------------------------------------------------------------------------------------------

set(TOYGINE_TARGET_PLATFORM_SUPPORT_CTEST OFF)

set(TOYGINE_CTEST_SUPPORT_PLATFORMS "Windows Desktop" "Linux Desktop" "macOS Desktop")
if (TOYGINE_TARGET_PLATFORM IN_LIST TOYGINE_CTEST_SUPPORT_PLATFORMS)
  set(TOYGINE_TARGET_PLATFORM_SUPPORT_CTEST ON)
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Remove all of the extra targets
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
#-----------------------------------------------------------------------------------------------------------------------

if (TOYGINE_TARGET_PLATFORM_SUPPORT_CTEST)
  include(FetchContent)
  include(CTest)

  FetchContent_Declare(doctest GIT_REPOSITORY https://github.com/doctest/doctest GIT_TAG v2.5.3 GIT_SHALLOW TRUE)
  FetchContent_MakeAvailable(doctest)

  list(APPEND CMAKE_MODULE_PATH ${doctest_SOURCE_DIR}/scripts/cmake)

  # Collect all test .cpp files from subdirectories (core/, network/, render/, etc.).
  file(GLOB_RECURSE TESTS_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

  # Root-level .cpp are excluded so TESTS_SRC contains only module test files
  file(GLOB ROOT_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
  list(REMOVE_ITEM TESTS_SRC ${ROOT_SRC})

  list(APPEND TESTS_SRC tests.cpp)

  include(doctest)

  add_executable(${TOYGINE_LIBRARY_NAME}-units ${TESTS_SRC})
  target_compile_definitions(${TOYGINE_LIBRARY_NAME}-units PRIVATE DOCTEST_CONFIG_SUPER_FAST_ASSERTS)

  target_include_directories(${TOYGINE_LIBRARY_NAME}-units PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
  target_link_libraries(${TOYGINE_LIBRARY_NAME}-units PRIVATE doctest::doctest ${TOYGINE_LIBRARY_NAME})
  if (CTEST_SUPPORT)
    doctest_discover_tests(${TOYGINE_LIBRARY_NAME}-units)
  endif ()
endif ()
