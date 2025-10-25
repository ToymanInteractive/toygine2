#-------------------------------------------------------------------------------------------
# Copyright (c) 2025 by Toyman Interactive
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

set(PLATFORM_SRC_CORE_LIST  src/platforms/stub/core/assertion_stub.cpp)
set(PLATFORM_HDR_CORE_LIST  )
set(PLATFORM_LIB_CORE_LIST  )

set(PLATFORM_HDR_LIST       src/platforms/linux/platform_config.hpp)

source_group("Platform\\Core" FILES ${PLATFORM_SRC_CORE_LIST} ${PLATFORM_HDR_CORE_LIST})

source_group("Platform" FILES ${PLATFORM_HDR_LIST})

set(SRC_LIST ${SRC_LIST} ${PLATFORM_SRC_CORE_LIST})
set(HDR_LIST ${HDR_LIST} ${PLATFORM_HDR_CORE_LIST} ${PLATFORM_HDR_LIST})
set(LIB_LIST ${LIB_LIST} ${PLATFORM_LIB_CORE_LIST})
