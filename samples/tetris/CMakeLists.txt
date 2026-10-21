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

add_executable(tetris-sample-app
  tetris_sample_app.cpp
  tetris_core.cpp
  terminal_screen.cpp
  terminal_input.cpp
  tetris_view.cpp)

target_link_libraries(tetris-sample-app PRIVATE toygine toygine::compile::warnings)
set_target_properties(tetris-sample-app PROPERTIES FOLDER "samples")

# CMAKE_CXX_EXTENSIONS OFF даёт __STRICT_ANSI__, а glibc за ним прячет cfmakeraw, SIGWINCH и TIOCGWINSZ.
target_compile_definitions(tetris-sample-app PRIVATE _DEFAULT_SOURCE _GNU_SOURCE _DARWIN_C_SOURCE)

install(TARGETS tetris-sample-app RUNTIME DESTINATION bin COMPONENT samples)
