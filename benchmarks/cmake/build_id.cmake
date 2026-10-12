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

# Run by cmake -P: writes the ROM's commit to a header and leaves an unchanged header alone.

set(TOYGINE_BUILD_ID "unknown")

if (GIT_EXECUTABLE)
  execute_process(COMMAND ${GIT_EXECUTABLE} describe --always --dirty
                  WORKING_DIRECTORY ${SOURCE_DIR}
                  OUTPUT_VARIABLE TOYGINE_GIT_DESCRIBE
                  OUTPUT_STRIP_TRAILING_WHITESPACE
                  RESULT_VARIABLE TOYGINE_GIT_RESULT
                  ERROR_QUIET)

  if (TOYGINE_GIT_RESULT EQUAL 0 AND NOT TOYGINE_GIT_DESCRIBE STREQUAL "")
    set(TOYGINE_BUILD_ID ${TOYGINE_GIT_DESCRIBE})
  endif ()
endif ()

set(TOYGINE_BUILD_ID_TEXT "#ifndef BENCHMARK_BUILD_ID_HPP_\n#define BENCHMARK_BUILD_ID_HPP_\n\n")
string(APPEND TOYGINE_BUILD_ID_TEXT "#define TOYGINE_BENCHMARKS_BUILD_ID \"${TOYGINE_BUILD_ID}\"\n\n#endif\n")

set(TOYGINE_BUILD_ID_PREVIOUS "")

if (EXISTS ${OUTPUT})
  file(READ ${OUTPUT} TOYGINE_BUILD_ID_PREVIOUS)
endif ()

if (NOT TOYGINE_BUILD_ID_TEXT STREQUAL TOYGINE_BUILD_ID_PREVIOUS)
  file(WRITE ${OUTPUT} "${TOYGINE_BUILD_ID_TEXT}")
endif ()
