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

if (TOYGINE_TARGET_PLATFORM STREQUAL "Windows Desktop")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Linux Desktop")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-declarations")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "macOS Desktop")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-switch-default")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Game Boy Advance")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-declarations")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo DS")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-declarations")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo 3DS")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-declarations")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Switch")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-declarations")

endif ()

#-------------------------------------------------------------------------------------------
# CTest support
#-------------------------------------------------------------------------------------------

set(CTEST_SUPPORT OFF)
set(TOYGINE_TARGET_PLATFORM_SUPPORT_MULTITHREADING OFF)
set(TOYGINE_TARGET_PLATFORM_SUPPORT_POSIX_SIGNALS OFF)

set(TOYGINE_DESKTOP_PLATFORMS "Windows Desktop" "Linux Desktop" "macOS Desktop")
if (TOYGINE_TARGET_PLATFORM IN_LIST TOYGINE_DESKTOP_PLATFORMS)
  set(CTEST_SUPPORT ON)
  set(TOYGINE_TARGET_PLATFORM_SUPPORT_MULTITHREADING ON)
  set(TOYGINE_TARGET_PLATFORM_SUPPORT_POSIX_SIGNALS ON)
endif ()

#-------------------------------------------------------------------------------------------
# Remove all of the extra targets
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
#-------------------------------------------------------------------------------------------

include(FetchContent)
include(CTest)

FetchContent_Declare(doctest GIT_REPOSITORY https://github.com/doctest/doctest GIT_TAG v2.5.2 GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(doctest)

# doctest 2.5.2 splits implementation into .cpp files built as a separate target.
# Patch it here — after FetchContent_MakeAvailable — so the options reach the target.
if (TARGET doctest_with_main)
  target_compile_options(doctest_with_main PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W0>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-w>
  )
  if (NOT TOYGINE_TARGET_PLATFORM_SUPPORT_MULTITHREADING)
    target_compile_definitions(doctest_with_main PRIVATE DOCTEST_CONFIG_NO_MULTITHREADING)
  endif ()
  if (NOT TOYGINE_TARGET_PLATFORM_SUPPORT_POSIX_SIGNALS)
    target_compile_definitions(doctest_with_main PRIVATE DOCTEST_CONFIG_NO_POSIX_SIGNALS)
  endif ()
endif ()

list(APPEND CMAKE_MODULE_PATH ${doctest_SOURCE_DIR}/scripts/cmake)

# Collect all test .cpp files from subdirectories (core/, network/, render/, etc.).
file(GLOB_RECURSE TESTS_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Root-level .cpp are excluded so TESTS_SRC contains only module test files
file(GLOB ROOT_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(REMOVE_ITEM TESTS_SRC ${ROOT_SRC})

if (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Game Boy Advance")
  list(APPEND TESTS_SRC gba_filtering_stream_buf.cpp)
endif ()

list(APPEND TESTS_SRC tests.cpp)

include(doctest)

add_executable(${TOYGINE_LIBRARY_NAME}-units ${TESTS_SRC})
target_compile_definitions(${TOYGINE_LIBRARY_NAME}-units PRIVATE DOCTEST_CONFIG_SUPER_FAST_ASSERTS)
if (NOT TOYGINE_TARGET_PLATFORM_SUPPORT_MULTITHREADING)
  target_compile_definitions(${TOYGINE_LIBRARY_NAME}-units PRIVATE DOCTEST_CONFIG_NO_MULTITHREADING)
endif ()
if (NOT TOYGINE_TARGET_PLATFORM_SUPPORT_POSIX_SIGNALS)
  target_compile_definitions(${TOYGINE_LIBRARY_NAME}-units PRIVATE DOCTEST_CONFIG_NO_POSIX_SIGNALS)
endif ()

target_include_directories(${TOYGINE_LIBRARY_NAME}-units PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(${TOYGINE_LIBRARY_NAME}-units PRIVATE doctest::doctest ${TOYGINE_LIBRARY_NAME})
if (CTEST_SUPPORT)
  doctest_discover_tests(${TOYGINE_LIBRARY_NAME}-units)
endif ()
if (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Game Boy Advance")
  gba_create_rom(${TOYGINE_LIBRARY_NAME}-units)
endif ()

if (TOYGINE_TESTS_ENABLE_COVERAGE)
  # include the cmake code coverage module
  include(CodeCoverage)

  # Update the default compiler flags to enable coverage
  append_coverage_compiler_flags()
  append_coverage_compiler_flags_to_target(${TOYGINE_LIBRARY_NAME})
  append_coverage_compiler_flags_to_target(${TOYGINE_LIBRARY_NAME}-units)

  # exclude /test and /ext directories from coverage analysis
  set(COVERAGE_EXCLUDES "${CMAKE_SOURCE_DIR}/tests/*" "${CMAKE_BINARY_DIR}/*" "/usr/*")

  set(COVERAGE_EXECUTABLE_ARGS "")

  # Determine which preset to use for testing
  if (DEFINED CMAKE_TEST_PRESET_NAME)
    set(COVERAGE_EXECUTABLE_ARGS "${COVERAGE_EXECUTABLE_ARGS} --preset ${CMAKE_TEST_PRESET_NAME}")
  elseif (DEFINED CMAKE_BUILD_PRESET)
    set(COVERAGE_EXECUTABLE_ARGS "${COVERAGE_EXECUTABLE_ARGS} --preset ${CMAKE_BUILD_PRESET}")
  endif ()

  # cmake code coverage module target
  setup_target_for_coverage_lcov(NAME unit_tests_coverage EXECUTABLE ctest EXECUTABLE_ARGS ${COVERAGE_EXECUTABLE_ARGS})

endif ()
