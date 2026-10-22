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

include(ConsoleApplication)
include(InstallApplication)

set(SRC_LIST console_sample_app.cpp)
set(HDR_LIST )
set(INL_LIST )
set(LIB_LIST toygine2)
set(BINARY_NAME console-sample-app)

if (TOYGINE_TARGET_PLATFORM STREQUAL "Sega MD")
  add_cartridge_executable(${BINARY_NAME} ${SRC_LIST} ${HDR_LIST} ${INL_LIST})
elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Linux Desktop" OR TOYGINE_TARGET_PLATFORM STREQUAL "macOS Desktop")
  console_application(${BINARY_NAME} SOURCES ${SRC_LIST} ${HDR_LIST} ${INL_LIST})
else()
  add_executable(${BINARY_NAME} ${SRC_LIST} ${HDR_LIST} ${INL_LIST})
  target_link_libraries(${BINARY_NAME} PRIVATE toygine toygine::compile::warnings)
endif()

set_target_properties(${BINARY_NAME} PROPERTIES FOLDER "samples")

if (TOYGINE_TARGET_PLATFORM STREQUAL "Sega MD")

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${BINARY_NAME}.bin" DESTINATION bin COMPONENT samples)

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GBA")

  gba_create_rom(${BINARY_NAME})

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${BINARY_NAME}.gba" DESTINATION bin COMPONENT samples)

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo DS")

  nds_create_rom(${BINARY_NAME})

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${BINARY_NAME}.nds" DESTINATION bin COMPONENT samples)

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo 3DS")

  ctr_create_3dsx(${BINARY_NAME})

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${BINARY_NAME}.3dsx" DESTINATION bin COMPONENT samples)

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Switch")

  nx_create_nro(${BINARY_NAME})

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${BINARY_NAME}.nro" DESTINATION bin COMPONENT samples)

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GameCube")

  ogc_create_dol(${BINARY_NAME})

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${BINARY_NAME}.dol" DESTINATION bin COMPONENT samples)

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Wii")

  ogc_create_dol(${BINARY_NAME})

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${BINARY_NAME}.dol" DESTINATION bin COMPONENT samples)

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Wii U")

  wut_create_rpx(${BINARY_NAME})

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${BINARY_NAME}.rpx" DESTINATION bin COMPONENT samples)

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Linux Desktop" OR TOYGINE_TARGET_PLATFORM STREQUAL "macOS Desktop")

  install_application(${BINARY_NAME} COMPONENT samples)

else ()

  install(TARGETS ${BINARY_NAME} RUNTIME DESTINATION bin COMPONENT samples)

endif ()
