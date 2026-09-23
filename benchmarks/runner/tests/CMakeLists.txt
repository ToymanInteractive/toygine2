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
# Tests translation units
#-----------------------------------------------------------------------------------------------------------------------

# Tests of the benchmark runner's host-side logic: plain C++ under doctest, without the timing that needs nanobench.
file(GLOB_RECURSE TOYGINE_BENCHMARK_TESTS_SRC CONFIGURE_DEPENDS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.test.cpp")

#-----------------------------------------------------------------------------------------------------------------------
# Tests built with doctest and discovered by CTest
#-----------------------------------------------------------------------------------------------------------------------

if (TOYGINE_TARGET_PLATFORM_IS_DESKTOP)
  include(FetchContent)

  FetchContent_Declare(doctest GIT_REPOSITORY https://github.com/doctest/doctest GIT_TAG v2.5.3 GIT_SHALLOW TRUE)
  FetchContent_MakeAvailable(doctest)

  list(APPEND CMAKE_MODULE_PATH ${doctest_SOURCE_DIR}/scripts/cmake)

  include(doctest)

  add_executable(benchmarks-runner-units
      ${TOYGINE_BENCHMARK_TESTS_SRC}
      ctest_main.cpp)
  target_compile_definitions(benchmarks-runner-units PRIVATE DOCTEST_CONFIG_SUPER_FAST_ASSERTS)

  target_include_directories(benchmarks-runner-units PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
  target_link_libraries(benchmarks-runner-units PRIVATE doctest::doctest ${TOYGINE_LIBRARY_NAME})
  set_target_properties(benchmarks-runner-units PROPERTIES FOLDER "tests")

  # arm64 macOS kills an unsigned binary, and Xcode signs only after the post-build phases, so the linker signs instead.
  set_target_properties(benchmarks-runner-units PROPERTIES XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED NO)

  doctest_discover_tests(benchmarks-runner-units)
endif ()
