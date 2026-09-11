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
# nanobench support
#-----------------------------------------------------------------------------------------------------------------------

set(TOYGINE_DESKTOP_PLATFORMS "Windows Desktop" "Linux Desktop" "macOS Desktop")

# Whether the build host can time a callable: no console toolchain here gives nanobench a steady clock or iostreams.
set(TOYGINE_TARGET_PLATFORM_SUPPORT_NANOBENCH OFF)
if (TOYGINE_TARGET_PLATFORM IN_LIST TOYGINE_DESKTOP_PLATFORMS)
  set(TOYGINE_TARGET_PLATFORM_SUPPORT_NANOBENCH ON)
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Benchmarks translation units
#-----------------------------------------------------------------------------------------------------------------------

# The runner: the entry point that walks the registry and prints a table per benchmark, so no benchmark holds one.
set(TOYGINE_BENCHMARK_RUNNER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/runner/main.cpp)

# Benchmarks of engine modules: the glob adds a file to the target, BENCHMARK_CASE registers the cases inside it.
file(GLOB_RECURSE TOYGINE_MODULE_BENCHMARKS_SRC CONFIGURE_DEPENDS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.benchmark.cpp")

#-----------------------------------------------------------------------------------------------------------------------
# Benchmarks built with nanobench
#-----------------------------------------------------------------------------------------------------------------------

if (TOYGINE_TARGET_PLATFORM_SUPPORT_NANOBENCH)
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty)

  # Declares the nanobench target and the nanobench::nanobench alias over the vendored sources in thirdparty/nanobench.
  include(nanobench)

  add_executable(${TOYGINE_LIBRARY_NAME}-benchmarks ${TOYGINE_BENCHMARK_RUNNER_SRC} ${TOYGINE_MODULE_BENCHMARKS_SRC})

  target_include_directories(${TOYGINE_LIBRARY_NAME}-benchmarks
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/runner ${CMAKE_CURRENT_SOURCE_DIR}/../include)
  target_link_libraries(${TOYGINE_LIBRARY_NAME}-benchmarks PRIVATE nanobench::nanobench ${TOYGINE_LIBRARY_NAME})

  # COMMAND takes no shell: a redirection would reach the binary as a pattern, so file output needs a runner option.
  # The other way is a ${CMAKE_COMMAND} -P wrapper, where execute_process can take OUTPUT_FILE.
  add_custom_target(benchmarks_run
    COMMAND $<TARGET_FILE:${TOYGINE_LIBRARY_NAME}-benchmarks>
    WORKING_DIRECTORY $<TARGET_FILE_DIR:${TOYGINE_LIBRARY_NAME}-benchmarks>
    COMMENT "Running benchmarks"
  )

  # A generator expression names the binary but orders nothing, so the run target asks for the build itself.
  add_dependencies(benchmarks_run ${TOYGINE_LIBRARY_NAME}-benchmarks)
endif ()
