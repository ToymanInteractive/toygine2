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

#-----------------------------------------------------------------------------------------------------------------------
# Test translation units
#-----------------------------------------------------------------------------------------------------------------------

# The runner's own translation units: its entry point and the platform sink. Task 8 replaces the desktop sink named
# here with the one the target platform provides.
set(TOYGINE_RUNNER_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/runner/toy_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/runner/sink_desktop.cpp)

# Tests of engine modules. They build under both runners, which is what makes a semantic drift between the two
# visible as a failing build or a differing verdict.
file(GLOB_RECURSE TOYGINE_MODULE_TESTS_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.test.cpp")
list(FILTER TOYGINE_MODULE_TESTS_SRC EXCLUDE REGEX "/runner/")

# Tests of the runner itself. They are written against doctest and inspect the runner's internals, so they build
# under doctest only.
file(GLOB TOYGINE_RUNNER_TESTS_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/runner/tests/*.test.cpp")

#-----------------------------------------------------------------------------------------------------------------------
# Tests built with the runner written for consoles
#-----------------------------------------------------------------------------------------------------------------------

add_executable(${TOYGINE_LIBRARY_NAME}-units-builtin ${TOYGINE_RUNNER_SRC} ${TOYGINE_MODULE_TESTS_SRC})
target_include_directories(${TOYGINE_LIBRARY_NAME}-units-builtin
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/runner ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(${TOYGINE_LIBRARY_NAME}-units-builtin PRIVATE ${TOYGINE_LIBRARY_NAME})

if (TOYGINE_TARGET_PLATFORM_SUPPORT_CTEST)
  include(FetchContent)
  include(CTest)

  FetchContent_Declare(doctest GIT_REPOSITORY https://github.com/doctest/doctest GIT_TAG v2.5.3 GIT_SHALLOW TRUE)
  FetchContent_MakeAvailable(doctest)

  list(APPEND CMAKE_MODULE_PATH ${doctest_SOURCE_DIR}/scripts/cmake)

  include(doctest)

  add_executable(${TOYGINE_LIBRARY_NAME}-units
      ${TOYGINE_MODULE_TESTS_SRC}
      ${TOYGINE_RUNNER_TESTS_SRC}
      tests.cpp)
  target_compile_definitions(${TOYGINE_LIBRARY_NAME}-units PRIVATE DOCTEST_CONFIG_SUPER_FAST_ASSERTS
                                                                  TOYGINE_TESTS_USE_DOCTEST)

  target_include_directories(${TOYGINE_LIBRARY_NAME}-units PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
  target_include_directories(${TOYGINE_LIBRARY_NAME}-units PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/runner)
  target_link_libraries(${TOYGINE_LIBRARY_NAME}-units PRIVATE doctest::doctest ${TOYGINE_LIBRARY_NAME})
  doctest_discover_tests(${TOYGINE_LIBRARY_NAME}-units)

  # Exit code 0 is the whole contract: every module test must reach the same verdict under both runners.
  add_test(NAME units-builtin COMMAND ${TOYGINE_LIBRARY_NAME}-units-builtin)

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
