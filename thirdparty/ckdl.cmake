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

# KDL parser for the project manifest. Consumers use only the streaming C API: nodes map onto the setting registry
# directly, so kdlpp would build a document model just to take it apart.
#
# The vendored copy drops the upstream CMakeLists, which builds CLI utilities and C++ bindings, adds CTest tests, and
# defines a global `math` target. thirdparty/README.md records the upstream commit.

include(CheckSymbolExists)

set(CKDL_DIR ${CMAKE_CURRENT_LIST_DIR}/ckdl)

# Mirrors KDL_C_SOURCES and KDL_UTF8_C_SOURCES upstream. Re-check when refreshing the vendored copy. src/utils holds
# the CLI tools and stays out of the build.
add_library(ckdl STATIC
  ${CKDL_DIR}/src/bigint.c
  ${CKDL_DIR}/src/compat.c
  ${CKDL_DIR}/src/emitter.c
  ${CKDL_DIR}/src/parser.c
  ${CKDL_DIR}/src/str.c
  ${CKDL_DIR}/src/tokenizer.c
  ${CKDL_DIR}/src/utf8.c
)
set_target_properties(ckdl PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED ON C_EXTENSIONS OFF)

# Public headers live in include/; the sources also pull private headers from src/.
target_include_directories(ckdl PUBLIC ${CKDL_DIR}/include PRIVATE ${CKDL_DIR}/src)

# KDL_STATIC_LIB must reach consumers: it strips the dllimport decoration from exported symbols.
target_compile_definitions(ckdl PUBLIC KDL_STATIC_LIB=1 PRIVATE BUILDING_KDL=1)

# ckdl uses reallocf where it exists. macOS has it, but check rather than assume.
check_symbol_exists(reallocf "stdlib.h" HAVE_REALLOCF)
if(HAVE_REALLOCF)
  target_compile_definitions(ckdl PRIVATE HAVE_REALLOCF)
endif()

# Vendored code is not ours to keep warning-free, and it inherits the including project's flags.
if(MSVC)
  target_compile_options(ckdl PRIVATE /w)
else()
  target_compile_options(ckdl PRIVATE -w)
endif()

add_library(kdl::kdl ALIAS ckdl)
