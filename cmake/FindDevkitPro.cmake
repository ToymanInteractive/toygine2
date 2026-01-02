#-------------------------------------------------------------------------------------------
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
#-------------------------------------------------------------------------------------------

# - FindDevkitPro.cmake
# Detects devkitPro toolchains and components (GBA, NDS, 3DS, SWITCH)
# Usage:
#   find_package(DevkitPro COMPONENTS gba nds 3ds switch)
#
# This script will attempt to find devkitPro components but will not fail if they are not found.
# It will simply report what was found and what was not found.
#
# Provides variables:
#   DEVKITPRO_FOUND - TRUE if devkitPro root was found
#   DEVKITPRO_ROOT - Root directory of devkitPro installation (if found)
#
#   DEVKITPRO_GBA_FOUND - TRUE if GBA libraries were found
#   DEVKITPRO_NDS_FOUND - TRUE if NDS libraries were found
#   DEVKITPRO_3DS_FOUND - TRUE if 3DS libraries were found
#   DEVKITPRO_SWITCH_FOUND - TRUE if SWITCH libraries were found

cmake_minimum_required(VERSION 3.31.0 FATAL_ERROR)

# === Helper macro for library detection ===
macro(_find_devkitpro_lib NAME HEADER LIBNAME SUBDIR)
  if(DEVKITPRO_FOUND)
    find_path(${NAME}_INCLUDE_DIR
      ${HEADER}
      PATHS ${DEVKITPRO_ROOT}/${SUBDIR}/include
      NO_DEFAULT_PATH
    )
    find_library(${NAME}_LIBRARY
      NAMES ${LIBNAME}
      PATHS ${DEVKITPRO_ROOT}/${SUBDIR}/lib
      NO_DEFAULT_PATH
    )

    if(${NAME}_INCLUDE_DIR AND ${NAME}_LIBRARY)
      set(${NAME}_FOUND TRUE)
      message(STATUS "Found ${SUBDIR}: ${${NAME}_LIBRARY}")
    else(${NAME}_INCLUDE_DIR AND ${NAME}_LIBRARY)
      set(${NAME}_FOUND FALSE)
      message(STATUS "Not found: ${SUBDIR}")
    endif(${NAME}_INCLUDE_DIR AND ${NAME}_LIBRARY)
  else(DEVKITPRO_FOUND)
    set(${NAME}_FOUND FALSE)
    message(STATUS "Skipping ${SUBDIR} search (devkitPro not found)")
  endif(DEVKITPRO_FOUND)

  mark_as_advanced(${NAME}_FOUND)
  unset(${NAME}_INCLUDE_DIR)
  unset(${NAME}_LIBRARY)
endmacro()

set(_DEVKITPRO_POSSIBLE_PATHS $ENV{DEVKITPRO} /opt/devkitpro C:/devkitPro)

find_path(DEVKITPRO_ROOT
  NAMES devkitARM devkitA64
  PATHS ${_DEVKITPRO_POSSIBLE_PATHS}
  NO_DEFAULT_PATH
  DOC "Root directory of devkitPro installation"
)

if(DEVKITPRO_ROOT)
  set(DEVKITPRO_FOUND TRUE)
  message(STATUS "Found devkitPro at: ${DEVKITPRO_ROOT}")

  # === devkitARM toolchain ===
  find_program(DEVKITARM_GCC arm-none-eabi-gcc
    PATHS "${DEVKITPRO_ROOT}/devkitARM/bin" NO_DEFAULT_PATH
    DOC "Path to devkitPro ARM gcc compiler")

  if(DEVKITARM_GCC)
    get_filename_component(DEVKITARM "${DEVKITARM_GCC}" DIRECTORY)
    get_filename_component(DEVKITARM "${DEVKITARM}" DIRECTORY)

    message(STATUS "Found devkitARM: ${DEVKITARM}")
  endif(DEVKITARM_GCC)

  unset(DEVKITARM_GCC)

  # === devkitA64 toolchain ===
  find_program(DEVKITA64_GCC aarch64-none-elf-gcc
    PATHS "${DEVKITPRO_ROOT}/devkitA64/bin" NO_DEFAULT_PATH
    DOC "Path to devkitPro ARM64 gcc compiler")

  if(DEVKITA64_GCC)
    get_filename_component(DEVKITA64 "${DEVKITA64_GCC}" DIRECTORY)
    get_filename_component(DEVKITA64 "${DEVKITA64}" DIRECTORY)

    message(STATUS "Found devkitARM: ${DEVKITA64}")
  endif(DEVKITA64_GCC)

  unset(DEVKITA64_GCC)

  mark_as_advanced(DEVKITARM DEVKITA64)

  # === Components ===
  if("gba" IN_LIST DevkitPro_FIND_COMPONENTS)
    _find_devkitpro_lib(DEVKITPRO_GBA gba.h gba libgba)
  endif()
  if("nds" IN_LIST DevkitPro_FIND_COMPONENTS)
    _find_devkitpro_lib(DEVKITPRO_NDS nds.h nds9 libnds)
  endif()
  if("3ds" IN_LIST DevkitPro_FIND_COMPONENTS)
    _find_devkitpro_lib(DEVKITPRO_3DS 3ds.h ctru libctru)
  endif()
  if("switch" IN_LIST DevkitPro_FIND_COMPONENTS)
    _find_devkitpro_lib(DEVKITPRO_SWITCH switch.h nx libnx)
  endif()

else(DEVKITPRO_ROOT)
  set(DEVKITPRO_FOUND FALSE)
  message(STATUS "No devkitPro found")
  message(STATUS "  Searched in ${_DEVKITPRO_POSSIBLE_PATHS}")
endif(DEVKITPRO_ROOT)

mark_as_advanced(DEVKITPRO_ROOT)
