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

cmake_minimum_required(VERSION 3.31.0 FATAL_ERROR)

function(console_application BINARY_NAME SRC_LIST ${HDR_LIST} ${INL_LIST} ${LIB_LIST})

source_group("Source Files" FILES ${SRC_LIST} ${INL_LIST})
source_group("Header Files" FILES ${HDR_LIST})

add_executable(${BINARY_NAME} ${SRC_LIST} ${HDR_LIST} ${INL_LIST})

if (DEFINED APP_VERSION_MAJOR)
  target_compile_definitions(${BINARY_NAME} PRIVATE APP_VERSION_MAJOR=${APP_VERSION_MAJOR})
else ()
  target_compile_definitions(${BINARY_NAME} PRIVATE APP_VERSION_MAJOR=0)
endif ()
if (DEFINED APP_VERSION_MINOR)
  target_compile_definitions(${BINARY_NAME} PRIVATE APP_VERSION_MINOR=${APP_VERSION_MINOR})
else ()
  target_compile_definitions(${BINARY_NAME} PRIVATE APP_VERSION_MINOR=0)
endif ()
if (DEFINED APP_VERSION_PATCH)
  target_compile_definitions(${BINARY_NAME} PRIVATE APP_VERSION_PATCH=${APP_VERSION_PATCH})
else ()
  target_compile_definitions(${BINARY_NAME} PRIVATE APP_VERSION_PATCH=0)
endif ()

find_package(Git)
if (GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
endif ()

if(DEFINED GIT_COMMIT_HASH)
  target_compile_definitions(${BINARY_NAME} PRIVATE APP_VERSION_REVISION="${GIT_COMMIT_HASH}")
endif()

target_link_libraries(${BINARY_NAME} PRIVATE ${LIB_LIST})

endfunction(console_application)
