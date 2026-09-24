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

# PNG reader and writer for the editor and its tools. The copy drops the upstream CMakeLists and autotools build, which
# would generate pnglibconf.h and install a CMake package: the prebuilt pnglibconf.h compiles as is and fixes the feature
# set. thirdparty/README.md records the upstream commit.

set(LIBPNG_DIR ${CMAKE_CURRENT_LIST_DIR}/libpng)

# pngstruct.h includes zlib.h, so png needs the zlib target for its include path as well as for linking.
if(NOT TARGET ZLIB::ZLIB)
  include(${CMAKE_CURRENT_LIST_DIR}/zlib.cmake)
endif()

set(SRC_LIBPNG_LIST
  ${LIBPNG_DIR}/png.c
  ${LIBPNG_DIR}/pngerror.c
  ${LIBPNG_DIR}/pngget.c
  ${LIBPNG_DIR}/pngmem.c
  ${LIBPNG_DIR}/pngpread.c
  ${LIBPNG_DIR}/pngread.c
  ${LIBPNG_DIR}/pngrio.c
  ${LIBPNG_DIR}/pngrtran.c
  ${LIBPNG_DIR}/pngrutil.c
  ${LIBPNG_DIR}/pngset.c
  ${LIBPNG_DIR}/pngtrans.c
  ${LIBPNG_DIR}/pngwio.c
  ${LIBPNG_DIR}/pngwrite.c
  ${LIBPNG_DIR}/pngwtran.c
  ${LIBPNG_DIR}/pngwutil.c
)

# The editor runs on desktop and may later reach tablets and the web: x86 and ARM cover all of them, so the copy keeps
# only the SSE2 and NEON filters. Both sets build everywhere; pngpriv.h picks one from the compiler's predefines, and
# the other compiles to nothing, as both do on wasm. A universal macOS binary gets the right set in each slice.
set(SRC_LIBPNG_SIMD_LIST
  ${LIBPNG_DIR}/arm/arm_init.c
  ${LIBPNG_DIR}/arm/filter_neon_intrinsics.c
  ${LIBPNG_DIR}/arm/palette_neon_intrinsics.c
  ${LIBPNG_DIR}/intel/filter_sse2_intrinsics.c
  ${LIBPNG_DIR}/intel/intel_init.c
)

# The compiler finds these through the include path; listing them puts the whole copy in the IDE's project tree.
set(HDR_LIBPNG_LIST
  ${LIBPNG_DIR}/png.h
  ${LIBPNG_DIR}/pngconf.h
  ${LIBPNG_DIR}/pngdebug.h
  ${LIBPNG_DIR}/pnginfo.h
  ${LIBPNG_DIR}/pnglibconf.h
  ${LIBPNG_DIR}/pngpriv.h
  ${LIBPNG_DIR}/pngstruct.h
)

add_library(png STATIC ${SRC_LIBPNG_LIST} ${SRC_LIBPNG_SIMD_LIST} ${HDR_LIBPNG_LIST})

# Keeps the vendored target out of the way in the Xcode and Visual Studio trees; other generators ignore it.
set_target_properties(png PROPERTIES FOLDER "thirdparty")

# Public and private headers share this directory. SYSTEM keeps the project's warnings off headers that are not ours.
target_include_directories(png SYSTEM PUBLIC ${LIBPNG_DIR})

# NEON switches on by itself when the compiler targets it; SSE2 waits for this opt-in, then reads the predefines too.
target_compile_definitions(png PRIVATE PNG_INTEL_SSE)

# No public libpng header includes zlib.h, so consumers link png alone.
target_link_libraries(png PRIVATE ZLIB::ZLIB)

# floor() and pow() from the gamma code live in libm on Linux and Android; Apple and Windows fold it into the C runtime.
if(UNIX AND NOT APPLE)
  target_link_libraries(png PRIVATE m)
endif()

add_library(png::png ALIAS png)
