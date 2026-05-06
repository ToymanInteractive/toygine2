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

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4625 /wd4626 /wd5026 /wd5027")

  # MSVC x86
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4738")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Linux Desktop")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-declarations -Wno-useless-cast")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "macOS Desktop")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-switch-default")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Game Boy Advance")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-declarations -Wno-useless-cast")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo DS")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-declarations -Wno-useless-cast")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo 3DS")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-declarations -Wno-useless-cast")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Switch")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-declarations -Wno-useless-cast")

endif ()

set(BENCHMARKS_OUTPUT_FILE "" CACHE STRING "Output file for benchmarks (empty = stdout)")

include(FetchContent)

FetchContent_Declare(nanobench GIT_REPOSITORY https://github.com/martinus/nanobench.git GIT_TAG v4.3.11 GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(nanobench)

file(GLOB_RECURSE BENCHMARKS_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(${TOYGINE_LIBRARY_NAME}-benchmarks ${BENCHMARKS_SRC})
target_link_libraries(${TOYGINE_LIBRARY_NAME}-benchmarks PRIVATE nanobench ${TOYGINE_LIBRARY_NAME})

if(BENCHMARKS_OUTPUT_FILE)
  set(BENCHMARK_OUTPUT_REDIRECT "> ${BENCHMARKS_OUTPUT_FILE}")
else()
  set(BENCHMARK_OUTPUT_REDIRECT "")
endif()

add_custom_target(benchmarks_run
  COMMAND $<TARGET_FILE:${TOYGINE_LIBRARY_NAME}-benchmarks> ${BENCHMARK_OUTPUT_REDIRECT}
  WORKING_DIRECTORY $<TARGET_FILE_DIR:${TOYGINE_LIBRARY_NAME}-benchmarks>
  COMMENT "Running benchmarks and saving results"
)
