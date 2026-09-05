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

# KDL parser for the project manifest. The editor uses only the streaming C API: nodes map onto the setting registry
# directly, so kdlpp would build a document model only to have it taken apart.

include(FetchContent)
include(CheckSymbolExists)

# Upstream's CMakeLists builds the CLI utilities and the C++ bindings, turns its own tests on and hands them to CTest,
# and adds a global target named `math`, with no namespaced target for consumers. SOURCE_SUBDIR names a directory the
# repository lacks, so MakeAvailable populates the sources and never calls add_subdirectory.
FetchContent_Declare(
  ckdl
  GIT_REPOSITORY https://github.com/tjol/ckdl.git
  GIT_TAG        c9c33fe64446287215e80705545139d92a48f829 # main after 1.0: signed-overflow fix in the parser
  GIT_SHALLOW    TRUE
  SOURCE_SUBDIR  do-not-configure
)
FetchContent_MakeAvailable(ckdl)

# Mirrors KDL_C_SOURCES and KDL_UTF8_C_SOURCES upstream. Re-check when moving the pin.
add_library(ckdl STATIC
  ${ckdl_SOURCE_DIR}/src/bigint.c
  ${ckdl_SOURCE_DIR}/src/compat.c
  ${ckdl_SOURCE_DIR}/src/emitter.c
  ${ckdl_SOURCE_DIR}/src/parser.c
  ${ckdl_SOURCE_DIR}/src/str.c
  ${ckdl_SOURCE_DIR}/src/tokenizer.c
  ${ckdl_SOURCE_DIR}/src/utf8.c
)
set_target_properties(ckdl PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED ON C_EXTENSIONS OFF)

# Public headers live in include/; the sources also pull private headers from src/.
target_include_directories(ckdl PUBLIC ${ckdl_SOURCE_DIR}/include PRIVATE ${ckdl_SOURCE_DIR}/src)

# KDL_STATIC_LIB must reach consumers: it strips the dllimport decoration from exported symbols.
target_compile_definitions(ckdl PUBLIC KDL_STATIC_LIB=1 PRIVATE BUILDING_KDL=1)

# ckdl uses reallocf where it exists. macOS has it, but check rather than assume.
check_symbol_exists(reallocf "stdlib.h" HAVE_REALLOCF)
if(HAVE_REALLOCF)
  target_compile_definitions(ckdl PRIVATE HAVE_REALLOCF)
endif()

# Third-party code is not ours to keep warning-free, and it inherits the editor's flags.
target_compile_options(ckdl PRIVATE -w)

add_library(kdl::kdl ALIAS ckdl)
