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
# directly, so kdlpp would build a document model just to take it apart. The copy drops the upstream CMakeLists, which
# builds CLI utilities and C++ bindings, adds CTest tests and defines a global math target. thirdparty/README.md records
# the upstream commit.

include(CheckSymbolExists)

set(CKDL_DIR ${CMAKE_CURRENT_LIST_DIR}/ckdl)

# Mirrors KDL_C_SOURCES and KDL_UTF8_C_SOURCES upstream. Re-check both lists when refreshing the copy.
set(SRC_CKDL_LIST
  ${CKDL_DIR}/src/bigint.c
  ${CKDL_DIR}/src/compat.c
  ${CKDL_DIR}/src/emitter.c
  ${CKDL_DIR}/src/parser.c
  ${CKDL_DIR}/src/str.c
  ${CKDL_DIR}/src/tokenizer.c
  ${CKDL_DIR}/src/utf8.c
)

# The compiler finds these through the include path; listing them puts the whole copy in the IDE's project tree.
set(HDR_CKDL_LIST
  ${CKDL_DIR}/include/kdl/common-macros.h
  ${CKDL_DIR}/include/kdl/common.h
  ${CKDL_DIR}/include/kdl/emitter.h
  ${CKDL_DIR}/include/kdl/kdl.h
  ${CKDL_DIR}/include/kdl/parser.h
  ${CKDL_DIR}/include/kdl/tokenizer.h
  ${CKDL_DIR}/include/kdl/value.h
  ${CKDL_DIR}/src/bigint.h
  ${CKDL_DIR}/src/compat.h
  ${CKDL_DIR}/src/grammar.h
  ${CKDL_DIR}/src/str.h
  ${CKDL_DIR}/src/utf8.h
)

add_library(ckdl STATIC ${SRC_CKDL_LIST} ${HDR_CKDL_LIST})

# Keeps the vendored target out of the way in the Xcode and Visual Studio trees; other generators ignore it.
set_target_properties(ckdl PROPERTIES FOLDER "thirdparty")

# Public headers live in include/; the private ones sit beside the sources, which reach them by relative include.
# SYSTEM keeps the project's warnings off headers that are not ours.
target_include_directories(ckdl SYSTEM PUBLIC ${CKDL_DIR}/include)

# KDL_STATIC_LIB must reach consumers: without it the Windows headers mark every function dllimport.
target_compile_definitions(ckdl PUBLIC KDL_STATIC_LIB)

# compat.c supplies reallocf where the C library lacks it; macOS and the BSDs have their own, and the check keeps
# compat.c from defining a second one.
check_symbol_exists(reallocf "stdlib.h" HAVE_REALLOCF)
if(HAVE_REALLOCF)
  target_compile_definitions(ckdl PRIVATE HAVE_REALLOCF)
endif()

# pow(), floor() and log10() from the number code live in libm on Linux and Android; Apple and Windows fold it into the
# C runtime.
if(UNIX AND NOT APPLE)
  target_link_libraries(ckdl PRIVATE m)
endif()

# The sources trip the project's -Werror set (-Wbad-function-cast, -Wassign-enum), and vendored code is not ours to patch.
if(MSVC)
  target_compile_options(ckdl PRIVATE /w)
else()
  target_compile_options(ckdl PRIVATE -w)
endif()

add_library(kdl::kdl ALIAS ckdl)
