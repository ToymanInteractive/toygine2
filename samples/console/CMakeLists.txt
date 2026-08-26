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

add_executable(console_sample_app console_sample_app.cpp)

target_link_libraries(console_sample_app PRIVATE toygine)

if (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GBA")

  gba_create_rom(console_sample_app)

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/console_sample_app.gba" DESTINATION bin COMPONENT samples)

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo DS")

  nds_create_rom(console_sample_app)

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/console_sample_app.nds" DESTINATION bin COMPONENT samples)

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo 3DS")

  ctr_create_3dsx(console_sample_app)

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/console_sample_app.3dsx" DESTINATION bin COMPONENT samples)

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Switch")

  nx_create_nro(console_sample_app)

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/console_sample_app.nro" DESTINATION bin COMPONENT samples)

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GameCube")

  ogc_create_dol(console_sample_app)

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/console_sample_app.dol" DESTINATION bin COMPONENT samples)

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Wii")

  ogc_create_dol(console_sample_app)

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/console_sample_app.dol" DESTINATION bin COMPONENT samples)

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Wii U")

  wut_create_rpx(console_sample_app)

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/console_sample_app.rpx" DESTINATION bin COMPONENT samples)

else ()

  install(TARGETS console_sample_app RUNTIME DESTINATION bin COMPONENT samples)

endif ()
