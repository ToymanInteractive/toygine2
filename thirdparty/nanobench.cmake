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

# Microbenchmark library for the benchmark runner. The whole library is one header, with the definitions behind
# ANKERL_NANOBENCH_IMPLEMENT; benchmarks/runner/main.cpp is the only translation unit that defines it. The copy keeps
# src/include/ and LICENSE and drops the upstream CMakeLists, which declares a project of its own and builds a 36-file
# test binary. thirdparty/README.md records the upstream commit and the local patch.

set(NANOBENCH_DIR ${CMAKE_CURRENT_LIST_DIR}/nanobench)

# The compiler finds the header through the include path; listing it puts the copy in the IDE's project tree.
set(HDR_NANOBENCH_LIST
  ${NANOBENCH_DIR}/src/include/nanobench.h
)

add_library(nanobench INTERFACE ${HDR_NANOBENCH_LIST})

# Keeps the vendored target out of the way in the Xcode and Visual Studio trees; other generators ignore it.
set_target_properties(nanobench PROPERTIES FOLDER "thirdparty")

# SYSTEM keeps the project's warnings off headers that are not ours.
target_include_directories(nanobench SYSTEM INTERFACE ${NANOBENCH_DIR}/src/include)

add_library(nanobench::nanobench ALIAS nanobench)
