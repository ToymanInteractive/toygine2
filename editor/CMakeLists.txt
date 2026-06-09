#-----------------------------------------------------------------------------------------------------------------------
# Copyright (c) 2025-2026 Toyman Interactive
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

qt_standard_project_setup()

set(SRC_LIST src/main.cpp)
set(HDR_LIST )
set(INL_LIST )
set(RES_LIST )
set(LIB_LIST toygine Qt6::Widgets)
set(BINARY_NAME toygine_editor)

source_group("Source Files" FILES ${SRC_LIST} ${INL_LIST})
source_group("Header Files" FILES ${HDR_LIST})

# qt_add_executable applies the platform GUI flags (WIN32 / MACOSX_BUNDLE), wires the Qt entry point,
# and enables automoc on the target.
qt_add_executable(${BINARY_NAME} ${SRC_LIST} ${HDR_LIST} ${INL_LIST} ${RES_LIST})

target_compile_definitions(${BINARY_NAME} PRIVATE
  TOY_EDITOR_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
  TOY_EDITOR_VERSION_MINOR=${PROJECT_VERSION_MINOR}
  TOY_EDITOR_VERSION_PATCH=${PROJECT_VERSION_PATCH})

find_package(Git)
if (GIT_FOUND)
  execute_process(
      COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_COMMIT_HASH
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_QUIET
  )
endif ()

if (GIT_COMMIT_HASH)
  target_compile_definitions(${BINARY_NAME} PRIVATE TOY_EDITOR_VERSION_REVISION="${GIT_COMMIT_HASH}")
else ()
  target_compile_definitions(${BINARY_NAME} PRIVATE TOY_EDITOR_VERSION_REVISION="")
endif ()


target_link_libraries(${BINARY_NAME} PRIVATE ${LIB_LIST})
