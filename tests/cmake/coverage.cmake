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

# Coverage of both test binaries under TOYGINE_TESTS_ENABLE_COVERAGE; included by tests/CMakeLists.txt on desktop only.

# CodeCoverage aborts on anything but a gcov-compatible toolchain, so keep the option a no-op elsewhere (MSVC)
set(TOYGINE_COMPILER_SUPPORT_COVERAGE OFF)
if (CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|(Apple)?[Cc]lang)$")
  set(TOYGINE_COMPILER_SUPPORT_COVERAGE ON)
endif ()

if (TOYGINE_TESTS_ENABLE_COVERAGE AND NOT TOYGINE_COMPILER_SUPPORT_COVERAGE)
  message(WARNING "TOYGINE_TESTS_ENABLE_COVERAGE ignored: ${CMAKE_CXX_COMPILER_ID} has no gcov-compatible coverage")
endif ()

if (TOYGINE_TESTS_ENABLE_COVERAGE AND TOYGINE_COMPILER_SUPPORT_COVERAGE)
  include(CodeCoverage)

  # The directory-scope flags also reach the link line, which is what pulls in the gcov runtime under Clang
  append_coverage_compiler_flags()
  append_coverage_compiler_flags_to_target(${TOYGINE_LIBRARY_NAME})
  append_coverage_compiler_flags_to_target(${TOYGINE_LIBRARY_NAME}-units)

  # exclude the tests, the build tree (fetched dependencies live there) and system headers from coverage
  # lcov 2.x fails on a pattern that matches nothing, so every entry must cover a file the capture actually carries:
  # only the two instrumented targets above produce one, which is why benchmarks and the editor name no pattern here
  set(COVERAGE_EXCLUDES "${CMAKE_SOURCE_DIR}/tests/*" "${CMAKE_BINARY_DIR}/*" "/usr/*")

  # The run is a whole ctest pass, so every registered binary has to exist: doctest registers a NOT_BUILT entry for one
  # that does not, and that entry fails the run before lcov sees a report. Which of them a build configures varies.
  set(TOYGINE_COVERAGE_DEPENDENCIES ${TOYGINE_LIBRARY_NAME}-units)
  foreach (candidate IN ITEMS console-test-runner-units benchmarks-runner-units editor-units)
    if (TARGET ${candidate})
      list(APPEND TOYGINE_COVERAGE_DEPENDENCIES ${candidate})

      # Only this directory's flags reach a link line, so a binary outside it links the instrumented library without
      # the gcov runtime it calls into. The flag stays off their compile lines: their own sources need no counters.
      target_link_options(${candidate} PRIVATE --coverage)
    endif ()
  endforeach ()

  # The ctest beside the configuring cmake, not whichever one the build environment's PATH happens to carry; it runs
  # from the build tree, where CMakePresets.json is not visible, so it takes no --preset argument
  setup_target_for_coverage_lcov(NAME unit_tests_coverage EXECUTABLE ${CMAKE_CTEST_COMMAND}
                                 # tests run serially: parallel cases of one binary race over the same .gcda files
                                 EXECUTABLE_ARGS --output-on-failure --no-tests=error
                                 DEPENDENCIES ${TOYGINE_COVERAGE_DEPENDENCIES}
                                 LCOV_ARGS "--ignore-errors" "inconsistent")
  set_target_properties(unit_tests_coverage PROPERTIES FOLDER "tests")
endif ()
