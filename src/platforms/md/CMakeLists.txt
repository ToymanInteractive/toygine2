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

set(PLATFORM_SRC_CORE_LIST
    src/platforms/stub/core/assertion_stub.cpp)
set(PLATFORM_HDR_CORE_LIST )
set(PLATFORM_LIB_CORE_LIST )

source_group("Platform\\Core"        FILES ${PLATFORM_SRC_CORE_LIST}        ${PLATFORM_HDR_CORE_LIST})

list(APPEND SRC_LIST ${PLATFORM_SRC_CORE_LIST})
list(APPEND HDR_LIST ${PLATFORM_HDR_CORE_LIST})
list(APPEND LIB_LIST ${PLATFORM_LIB_CORE_LIST})

# Overlay over the installed ClownMDSDK: a file under extern/clownmdsdk wins over the SDK's own copy. The
# include directory goes ahead of every toolchain include path, so <string.h> and <cstring> resolve here,
# and each source is compiled into the engine, so its definitions are linked before libc.a is searched.
# Both lists are globbed, so dropping a file into the overlay needs no change to this file.
set(CLOWNMDSDK_OVERLAY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/clownmdsdk)

file(GLOB_RECURSE PLATFORM_SRC_EXTERN_LIST CONFIGURE_DEPENDS
    ${CLOWNMDSDK_OVERLAY_DIR}/src/*.c
    ${CLOWNMDSDK_OVERLAY_DIR}/src/*.cpp)
file(GLOB_RECURSE PLATFORM_HDR_EXTERN_LIST CONFIGURE_DEPENDS
    ${CLOWNMDSDK_OVERLAY_DIR}/include/*)

source_group("Platform\\Extern"      FILES ${PLATFORM_SRC_EXTERN_LIST}      ${PLATFORM_HDR_EXTERN_LIST})

# The overlay is SDK code and uses the GNU C dialect the SDK builds it in; the engine's own C is ISO
# (CMAKE_C_EXTENSIONS is OFF), which rejects the asm keyword these routines are written with.
set_source_files_properties(${PLATFORM_SRC_EXTERN_LIST} PROPERTIES COMPILE_OPTIONS -std=gnu17)

include_directories(BEFORE ${CLOWNMDSDK_OVERLAY_DIR}/include)

list(APPEND SRC_LIST ${PLATFORM_SRC_EXTERN_LIST})
list(APPEND HDR_LIST ${PLATFORM_HDR_EXTERN_LIST})
