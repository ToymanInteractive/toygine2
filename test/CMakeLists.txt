#-------------------------------------------------------------------------------------------
# Copyright (c) 2025-2026 Toyman Interactive
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
set(PLATFORM_CTEST_SUPPORT OFF)
set(PLATFORM_MULTITHREADING_SUPPORT OFF)
set(PLATFORM_POSIX_SIGNALS_SUPPORT OFF)

macro(set_platform_flag_if_supported FLAG_VAR PLATFORMS_LIST)
  if (TOYGINE_TARGET_PLATFORM IN_LIST ${PLATFORMS_LIST})
    set(${FLAG_VAR} ON)
  endif (TOYGINE_TARGET_PLATFORM IN_LIST ${PLATFORMS_LIST})
endmacro (set_platform_flag_if_supported FLAG_VAR PLATFORMS_LIST)

set(TOYGINE_CTEST_SUPPORT_PLATFORMS "Windows Desktop" "Linux Desktop" "macOS Desktop")
set(TOYGINE_MULTITHREADING_SUPPORT_PLATFORMS "Windows Desktop" "Linux Desktop" "macOS Desktop")
set(TOYGINE_POSIX_SIGNALS_SUPPORT_PLATFORMS "Windows Desktop" "Linux Desktop" "macOS Desktop")
set_platform_flag_if_supported(PLATFORM_CTEST_SUPPORT TOYGINE_CTEST_SUPPORT_PLATFORMS)
set_platform_flag_if_supported(PLATFORM_MULTITHREADING_SUPPORT TOYGINE_MULTITHREADING_SUPPORT_PLATFORMS)
set_platform_flag_if_supported(PLATFORM_POSIX_SIGNALS_SUPPORT TOYGINE_POSIX_SIGNALS_SUPPORT_PLATFORMS)
#-------------------------------------------------------------------------------------------
# Remove all of the extra targets
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
#-------------------------------------------------------------------------------------------

include(FetchContent)
include(CTest)

FetchContent_Declare(doctest GIT_REPOSITORY https://github.com/doctest/doctest GIT_TAG v2.4.12)
FetchContent_MakeAvailable(doctest)

list(APPEND CMAKE_MODULE_PATH ${doctest_SOURCE_DIR}/scripts/cmake)

file(GLOB_RECURSE TEST_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

include(doctest)

add_executable(${TOYGINE_LIBRARY_NAME}-units ${TEST_SRC})
target_compile_definitions(${TOYGINE_LIBRARY_NAME}-units PRIVATE DOCTEST_CONFIG_SUPER_FAST_ASSERTS)
if (NOT PLATFORM_MULTITHREADING_SUPPORT)
  target_compile_definitions(${TOYGINE_LIBRARY_NAME}-units PRIVATE DOCTEST_CONFIG_NO_MULTITHREADING)
endif(NOT PLATFORM_MULTITHREADING_SUPPORT)
if (NOT PLATFORM_POSIX_SIGNALS_SUPPORT)
  target_compile_definitions(${TOYGINE_LIBRARY_NAME}-units PRIVATE DOCTEST_CONFIG_NO_POSIX_SIGNALS)
endif(NOT PLATFORM_POSIX_SIGNALS_SUPPORT)
target_include_directories(${TOYGINE_LIBRARY_NAME}-units PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(${TOYGINE_LIBRARY_NAME}-units PRIVATE doctest::doctest ${TOYGINE_LIBRARY_NAME})
if (PLATFORM_CTEST_SUPPORT)
  doctest_discover_tests(${TOYGINE_LIBRARY_NAME}-units)
endif (PLATFORM_CTEST_SUPPORT)

if (TOYGINE_TESTS_ENABLE_COVERAGE)
  # include the cmake code coverage module
  include(CodeCoverage)

  # Update the default compiler flags to enable coverage
  append_coverage_compiler_flags()
  append_coverage_compiler_flags_to_target(${TOYGINE_LIBRARY_NAME})
  append_coverage_compiler_flags_to_target(${TOYGINE_LIBRARY_NAME}-units)

  # exclude /test and /ext directories from coverage analysis
  set(COVERAGE_EXCLUDES "${CMAKE_SOURCE_DIR}/test/*" "${CMAKE_BINARY_DIR}/*" "/usr/*")

  set(COVERAGE_EXECUTABLE_ARGS "")

  # Determine which preset to use for testing
  if(DEFINED CMAKE_TEST_PRESET_NAME)
    set(COVERAGE_EXECUTABLE_ARGS "${COVERAGE_EXECUTABLE_ARGS} --preset ${CMAKE_TEST_PRESET_NAME}")
  elseif(DEFINED CMAKE_BUILD_PRESET)
    set(COVERAGE_EXECUTABLE_ARGS "${COVERAGE_EXECUTABLE_ARGS} --preset ${CMAKE_BUILD_PRESET}")
  endif ()

  # cmake code coverage module target
  setup_target_for_coverage_lcov(NAME unit_tests_coverage EXECUTABLE ctest EXECUTABLE_ARGS ${COVERAGE_EXECUTABLE_ARGS})

endif (TOYGINE_TESTS_ENABLE_COVERAGE)
