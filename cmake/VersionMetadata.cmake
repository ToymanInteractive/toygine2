#-----------------------------------------------------------------------------------------------------------------------
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
#-----------------------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.31.0 FATAL_ERROR)

#[=======================================================================[.rst:
VersionMetadata
---------------

Stamp targets with build-time version metadata derived from Git.

.. command:: add_git_revision_definition

  Resolve the short Git commit hash and bake it into a target as a compile definition.

  .. code-block:: cmake

    add_git_revision_definition(<target> <definition>)

  ``<target>``
    Target to receive the definition.

  ``<definition>``
    Name of the compile definition to set to the quoted short commit hash (for example ``"a1b2c3d"``),
    or to an empty string when Git is unavailable or the hash cannot be resolved.
#]=======================================================================]
function(add_git_revision_definition target definition)
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

  if (GIT_COMMIT_HASH)
    target_compile_definitions(${target} PRIVATE ${definition}="${GIT_COMMIT_HASH}")
  else ()
    target_compile_definitions(${target} PRIVATE ${definition}="")
  endif ()
endfunction()
