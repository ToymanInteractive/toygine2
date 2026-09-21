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

# Deflate and inflate streams for the asset tools; libpng is the consumer and brings its own zalloc and zfree.
# The copy drops the upstream CMakeLists and autotools build, which would make two libraries, regenerate zconf.h and
# install a CMake package: the shipped zconf.h compiles as is. thirdparty/README.md records the upstream commit.

set(ZLIB_DIR ${CMAKE_CURRENT_LIST_DIR}/zlib)

# gz*.c is out of the copy, so the library opens no file of its own; compress.c, uncompr.c and infback.c are out for
# want of a consumer. gzguts.h stays because zutil.c includes it. Re-check both lists when refreshing the copy.
set(SRC_ZLIB_LIST
  ${ZLIB_DIR}/adler32.c
  ${ZLIB_DIR}/crc32.c
  ${ZLIB_DIR}/deflate.c
  ${ZLIB_DIR}/inffast.c
  ${ZLIB_DIR}/inflate.c
  ${ZLIB_DIR}/inftrees.c
  ${ZLIB_DIR}/trees.c
  ${ZLIB_DIR}/zutil.c
)

# The compiler finds these through the include path; listing them puts the whole copy in the IDE's project tree.
set(HDR_ZLIB_LIST
  ${ZLIB_DIR}/crc32.h
  ${ZLIB_DIR}/deflate.h
  ${ZLIB_DIR}/gzguts.h
  ${ZLIB_DIR}/inffast.h
  ${ZLIB_DIR}/inffixed.h
  ${ZLIB_DIR}/inflate.h
  ${ZLIB_DIR}/inftrees.h
  ${ZLIB_DIR}/trees.h
  ${ZLIB_DIR}/zconf.h
  ${ZLIB_DIR}/zlib.h
  ${ZLIB_DIR}/zutil.h
)

add_library(zlib STATIC ${SRC_ZLIB_LIST} ${HDR_ZLIB_LIST})

# Keeps the vendored target out of the way in the Xcode and Visual Studio trees; other generators ignore it.
set_target_properties(zlib PROPERTIES FOLDER "thirdparty")

# Public and private headers share this directory. SYSTEM keeps the project's warnings off headers that are not ours.
target_include_directories(zlib SYSTEM PUBLIC ${ZLIB_DIR})

add_library(ZLIB::ZLIB ALIAS zlib)
