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

# Stuff the average programmer needs to change #
set(SRC_LIST console_app.cpp)
set(HDR_LIST )
set(INL_LIST )
set(LIB_LIST toygine)
set(RES_LIST )
set(MEDIA_PATH )
set(APP_NAME console_app)

# Non Builds programmer should not need to change anything beyond this #
add_executable(${APP_NAME} ${SRC_LIST} ${HDR_LIST} ${INL_LIST} ${RES_LIST})
target_link_libraries(${APP_NAME} ${LIB_LIST})
