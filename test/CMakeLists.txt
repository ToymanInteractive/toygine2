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
set(CTEST_SUPPORT OFF)

if (TOYGINE_TARGET_PLATFORM STREQUAL "Windows Desktop")

  if (MSVC)
    string(REGEX REPLACE "/Wall " "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    string(REGEX REPLACE "/Wall " "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

    string(REGEX REPLACE "/W." "/W0" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    string(REGEX REPLACE "/W." "/W0" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

    string(REGEX REPLACE "/sdl" "/sdl-" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
    string(REGEX REPLACE "/sdl" "/sdl-" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")

    string(REGEX REPLACE "/sdl" "/sdl-" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    string(REGEX REPLACE "/sdl" "/sdl-" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

    string(REGEX REPLACE "/RTCc" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    string(REGEX REPLACE "/RTCc" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
  endif (MSVC)

  set(CTEST_SUPPORT ON)

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Linux Desktop")

  set(CTEST_SUPPORT ON)

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "macOS Desktop")

  string(REGEX REPLACE "-Werror" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  string(REGEX REPLACE "-Weverything" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  string(REGEX REPLACE "-pedantic-errors" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  set(CMAKE_C_FLAGS "-w ${CMAKE_C_FLAGS}")

  string(REGEX REPLACE "-Werror" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  string(REGEX REPLACE "-Weverything" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  string(REGEX REPLACE "-pedantic-errors" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "-w ${CMAKE_CXX_FLAGS}")

  set(CTEST_SUPPORT ON)

endif ()
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

add_executable(${TOYGINE_LIBRARY_NAME}-tests ${TEST_SRC})
target_compile_definitions(${TOYGINE_LIBRARY_NAME}-tests PRIVATE DOCTEST_CONFIG_SUPER_FAST_ASSERTS DOCTEST_CONFIG_NO_POSIX_SIGNALS)
target_include_directories(${TOYGINE_LIBRARY_NAME}-tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(${TOYGINE_LIBRARY_NAME}-tests PRIVATE doctest::doctest ${TOYGINE_LIBRARY_NAME})
if (CTEST_SUPPORT)
  doctest_discover_tests(${TOYGINE_LIBRARY_NAME}-tests)
endif (CTEST_SUPPORT)

if (TOYGINE_TESTS_ENABLE_COVERAGE)
  # include the cmake code coverage module
  include(CodeCoverage)

  # Update the default compiler flags to enable coverage
  append_coverage_compiler_flags()
  append_coverage_compiler_flags_to_target(${TOYGINE_LIBRARY_NAME})
  append_coverage_compiler_flags_to_target(${TOYGINE_LIBRARY_NAME}-tests)

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
