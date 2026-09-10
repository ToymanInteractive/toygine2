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

# Whether the build host can time a callable and print the table: nanobench needs a steady clock and iostreams, which
# no console toolchain here offers. A measurement taken under an emulator would report the host anyway.
set(TOYGINE_TARGET_PLATFORM_SUPPORT_NANOBENCH OFF)
if (TOYGINE_TARGET_PLATFORM IN_LIST TOYGINE_DESKTOP_PLATFORMS)
  set(TOYGINE_TARGET_PLATFORM_SUPPORT_NANOBENCH ON)
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Benchmarks translation units
#-----------------------------------------------------------------------------------------------------------------------

# The runner: the entry point that walks the registry and prints one table per benchmark. A file of its own, so a
# benchmark carries no entry point and no table setup.
set(TOYGINE_BENCHMARK_RUNNER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/runner/main.cpp)

# Benchmarks of engine modules. Each registers its tables through the BENCHMARK_CASE macro, so a new file joins the
# binary without a line of CMake.
file(GLOB_RECURSE TOYGINE_MODULE_BENCHMARKS_SRC CONFIGURE_DEPENDS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.benchmark.cpp")

#-----------------------------------------------------------------------------------------------------------------------
# Benchmarks built with nanobench
#-----------------------------------------------------------------------------------------------------------------------

if (TOYGINE_TARGET_PLATFORM_SUPPORT_NANOBENCH)
  include(FetchContent)

  FetchContent_Declare(nanobench GIT_REPOSITORY https://github.com/martinus/nanobench.git GIT_TAG v4.6.0 GIT_SHALLOW TRUE)
  FetchContent_MakeAvailable(nanobench)

  add_executable(${TOYGINE_LIBRARY_NAME}-benchmarks ${TOYGINE_BENCHMARK_RUNNER_SRC} ${TOYGINE_MODULE_BENCHMARKS_SRC})

  target_include_directories(${TOYGINE_LIBRARY_NAME}-benchmarks
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/runner ${CMAKE_CURRENT_SOURCE_DIR}/../include)
  target_link_libraries(${TOYGINE_LIBRARY_NAME}-benchmarks PRIVATE nanobench::nanobench ${TOYGINE_LIBRARY_NAME})

  add_custom_target(benchmarks_run
    COMMAND $<TARGET_FILE:${TOYGINE_LIBRARY_NAME}-benchmarks> ${BENCHMARK_OUTPUT_REDIRECT}
    WORKING_DIRECTORY $<TARGET_FILE_DIR:${TOYGINE_LIBRARY_NAME}-benchmarks>
    COMMENT "Running benchmarks and saving results"
  )

  # A generator expression names the binary but orders nothing, so the run target asks for the build itself.
  add_dependencies(benchmarks_run ${TOYGINE_LIBRARY_NAME}-benchmarks)
endif ()
