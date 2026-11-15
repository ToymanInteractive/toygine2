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

# S3511/S3515 cart RTC driver from agbabi (bit-banged over the cartridge GPIO); see thirdparty/README.md.
# Vendored state is upstream commit 2adf1e6c01f9 with the newlib POSIX glue (_gettimeofday/settimeofday)
# stripped; the patch is marked in the file banner and recorded in thirdparty/README.md.

set(AGBABI_DIR ${CMAKE_CURRENT_LIST_DIR}/agbabi)

set(SRC_AGBABI_LIST
  ${AGBABI_DIR}/src/rtc.c
)

# Listed so the whole copy shows up in the IDE project tree.
set(HDR_AGBABI_LIST
  ${AGBABI_DIR}/include/agbabi.h
)

add_library(agbabi STATIC ${SRC_AGBABI_LIST} ${HDR_AGBABI_LIST})

set_target_properties(agbabi PROPERTIES FOLDER "thirdparty")

# SYSTEM keeps the project's warnings off the vendored header.
target_include_directories(agbabi SYSTEM PUBLIC ${AGBABI_DIR}/include)

add_library(agbabi::agbabi ALIAS agbabi)
