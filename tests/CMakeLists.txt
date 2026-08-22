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

if (TOYGINE_TARGET_PLATFORM STREQUAL "Windows Desktop")

  if (MSVC)

    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   /EHsc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")

  endif ()

endif ()

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

  # Only files named *.test.cpp are test translation units. The runner's own implementation, its sinks and the
  # self-test binaries carry their own main() and belong to other targets, so the pattern keeps them out.
  file(GLOB_RECURSE TESTS_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.test.cpp")

  list(APPEND TESTS_SRC tests.cpp)

  include(doctest)

  add_executable(${TOYGINE_LIBRARY_NAME}-units ${TESTS_SRC})
  target_compile_definitions(${TOYGINE_LIBRARY_NAME}-units PRIVATE DOCTEST_CONFIG_SUPER_FAST_ASSERTS)

  target_include_directories(${TOYGINE_LIBRARY_NAME}-units PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
  target_include_directories(${TOYGINE_LIBRARY_NAME}-units PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/runner)
  target_link_libraries(${TOYGINE_LIBRARY_NAME}-units PRIVATE doctest::doctest ${TOYGINE_LIBRARY_NAME})
  doctest_discover_tests(${TOYGINE_LIBRARY_NAME}-units)

  # CodeCoverage aborts on anything but a gcov-compatible toolchain, so keep the option a no-op elsewhere (MSVC)
  set(TOYGINE_COMPILER_SUPPORT_COVERAGE OFF)
  if (CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|(Apple)?[Cc]lang)$")
    set(TOYGINE_COMPILER_SUPPORT_COVERAGE ON)
  endif ()

  if (TOYGINE_TESTS_ENABLE_COVERAGE AND NOT TOYGINE_COMPILER_SUPPORT_COVERAGE)
    message(WARNING "TOYGINE_TESTS_ENABLE_COVERAGE ignored: ${CMAKE_CXX_COMPILER_ID} has no gcov-compatible coverage")
  endif ()

  if (TOYGINE_TESTS_ENABLE_COVERAGE AND TOYGINE_COMPILER_SUPPORT_COVERAGE)
    # include the cmake code coverage module
    include(CodeCoverage)

    # Update the default compiler flags to enable coverage
    # The directory-scope flags also reach the link line, which is what pulls in the gcov runtime under Clang
    append_coverage_compiler_flags()
    append_coverage_compiler_flags_to_target(${TOYGINE_LIBRARY_NAME})
    append_coverage_compiler_flags_to_target(${TOYGINE_LIBRARY_NAME}-units)

    # exclude tests, the build tree (fetched dependencies live there) and system headers from coverage analysis
    # lcov 2.x fails on a pattern that matches nothing, so every entry here must name a directory that exists
    set(COVERAGE_EXCLUDES "${CMAKE_SOURCE_DIR}/tests/*" "${CMAKE_BINARY_DIR}/*" "/usr/*")

    # cmake code coverage module target
    # ctest runs from the build tree, where CMakePresets.json is not visible, so it takes no --preset argument
    setup_target_for_coverage_lcov(NAME unit_tests_coverage EXECUTABLE ctest
                                   # tests run serially: parallel cases of one binary race over the same .gcda files
                                   EXECUTABLE_ARGS --output-on-failure --no-tests=error
                                   DEPENDENCIES ${TOYGINE_LIBRARY_NAME}-units
                                   LCOV_ARGS "--ignore-errors" "inconsistent")

  endif ()
endif ()
