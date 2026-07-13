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

cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)

#[=======================================================================[.rst:
FindDevkitPro
-------------

Detect a devkitPro installation and its platform components.

Locates the devkitPro root and, for each requested component, its platform library and headers (Nintendo GBA,
Nintendo DS, Nintendo 3DS, Nintendo Switch, Nintendo GameCube, Nintendo Wii). The module never fails when a component is
missing: it reports what was found and what was not, leaving the decision to the caller.

The installation root is searched, in order, in the ``DEVKITPRO`` environment variable, ``/opt/devkitpro``,
then ``C:/devkitPro``.

.. code-block:: cmake

  find_package(DevkitPro COMPONENTS gba nds 3ds switch gamecube wii)

Components
^^^^^^^^^^

Each requested component is searched independently:

``gba``
  Game Boy Advance library (``libgba``, header ``gba.h``).

``nds``
  Nintendo DS ARM9 library (``libnds``, header ``nds.h``).

``3ds``
  Nintendo 3DS library (``libctru``, header ``3ds.h``).

``switch``
  Nintendo Switch library (``libnx``, header ``switch.h``).

``gamecube``
  Nintendo GameCube library (``libogc`` from ``lib/cube``, header ``gccore.h``). Built with the devkitPPC toolchain.

``wii``
  Nintendo Wii library (``libogc`` from ``lib/wii``, header ``gccore.h``). Built with the devkitPPC toolchain.

Result Variables
^^^^^^^^^^^^^^^^^

This module defines the following variables:

``DEVKITPRO_FOUND``
  ``TRUE`` if the devkitPro installation root was found.

``DEVKITPRO_ROOT``
  Root directory of the devkitPro installation.

``DEVKITPRO_GBA_FOUND``, ``DEVKITPRO_NDS_FOUND``, ``DEVKITPRO_3DS_FOUND``, ``DEVKITPRO_SWITCH_FOUND``, ``DEVKITPRO_GAMECUBE_FOUND``, ``DEVKITPRO_WII_FOUND``
  ``TRUE`` if the corresponding requested component was found. Defined only for components listed in ``COMPONENTS``.

``DEVKITA64``
  Root of the devkitA64 toolchain, set when its compiler is present.

``DEVKITARM``
  Root of the devkitARM toolchain, set when its compiler is present.

``DEVKITPPC``
  Root of the devkitPPC toolchain, set when its compiler is present.
#]=======================================================================]

# === Helper macro for library detection ===
# Usage: _find_devkitpro_lib(NAME HEADER LIBNAME SUBDIR [LIB_VARIANT])
# Headers are searched in ${SUBDIR}/include. Libraries are searched in ${SUBDIR}/lib, or in
# ${SUBDIR}/lib/${LIB_VARIANT} when an optional variant is given (e.g. libogc's cube/wii subdirs).
macro(_find_devkitpro_lib NAME HEADER LIBNAME SUBDIR)
  set(_lib_dir ${DEVKITPRO_ROOT}/${SUBDIR}/lib)
  set(_label ${SUBDIR})
  if (${ARGC} GREATER 4)
    set(_lib_dir ${_lib_dir}/${ARGV4})
    set(_label "${SUBDIR}/lib/${ARGV4}")
  endif ()

  if (DEVKITPRO_FOUND)
    find_path(${NAME}_INCLUDE_DIR
      ${HEADER}
      PATHS ${DEVKITPRO_ROOT}/${SUBDIR}/include
      NO_DEFAULT_PATH
    )
    find_library(${NAME}_LIBRARY
      NAMES ${LIBNAME}
      PATHS ${_lib_dir}
      NO_DEFAULT_PATH
    )

    if (${NAME}_INCLUDE_DIR AND ${NAME}_LIBRARY)
      set(${NAME}_FOUND TRUE)
      message(STATUS "Found ${_label}: ${${NAME}_LIBRARY}")
    else ()
      set(${NAME}_FOUND FALSE)
      message(STATUS "Not found: ${_label}")
    endif ()
  else ()
    set(${NAME}_FOUND FALSE)
    message(STATUS "Skipping ${_label} search (devkitPro not found)")
  endif ()

  mark_as_advanced(${NAME}_FOUND)
  unset(${NAME}_INCLUDE_DIR)
  unset(${NAME}_LIBRARY)
  unset(_label)
  unset(_lib_dir)
endmacro()

set(_DEVKITPRO_POSSIBLE_PATHS $ENV{DEVKITPRO} /opt/devkitpro C:/devkitPro)

find_path(DEVKITPRO_ROOT
  NAMES devkitA64 devkitARM devkitPPC
  PATHS ${_DEVKITPRO_POSSIBLE_PATHS}
  NO_DEFAULT_PATH
  DOC "Root directory of devkitPro installation"
)

if (DEVKITPRO_ROOT)
  set(DEVKITPRO_FOUND TRUE)
  message(STATUS "Found devkitPro at: ${DEVKITPRO_ROOT}")

  # === devkitA64 toolchain ===
  find_program(DEVKITA64_GCC aarch64-none-elf-gcc
    PATHS "${DEVKITPRO_ROOT}/devkitA64/bin" NO_DEFAULT_PATH
    DOC "Path to devkitPro ARM64 gcc compiler")

  if (DEVKITA64_GCC)
    get_filename_component(DEVKITA64 "${DEVKITA64_GCC}" DIRECTORY)
    get_filename_component(DEVKITA64 "${DEVKITA64}" DIRECTORY)

    message(STATUS "Found devkitA64 toolchain: ${DEVKITA64}")
  endif ()

  unset(DEVKITA64_GCC)

  # === devkitARM toolchain ===
  find_program(DEVKITARM_GCC arm-none-eabi-gcc
    PATHS "${DEVKITPRO_ROOT}/devkitARM/bin" NO_DEFAULT_PATH
    DOC "Path to devkitPro ARM gcc compiler")

  if (DEVKITARM_GCC)
    get_filename_component(DEVKITARM "${DEVKITARM_GCC}" DIRECTORY)
    get_filename_component(DEVKITARM "${DEVKITARM}" DIRECTORY)

    message(STATUS "Found devkitARM toolchain: ${DEVKITARM}")
  endif ()

  unset(DEVKITARM_GCC)

  # === devkitPPC toolchain ===
  find_program(DEVKITPPC_GCC powerpc-eabi-gcc
    PATHS "${DEVKITPRO_ROOT}/devkitPPC/bin" NO_DEFAULT_PATH
    DOC "Path to devkitPro PPC gcc compiler")

  if (DEVKITPPC_GCC)
    get_filename_component(DEVKITPPC "${DEVKITPPC_GCC}" DIRECTORY)
    get_filename_component(DEVKITPPC "${DEVKITPPC}" DIRECTORY)

    message(STATUS "Found devkitPPC toolchain: ${DEVKITPPC}")
  endif ()

  unset(DEVKITPPC_GCC)

  mark_as_advanced(DEVKITARM DEVKITA64 DEVKITPPC)

  # === Components ===
  if ("gba" IN_LIST DevkitPro_FIND_COMPONENTS)
    _find_devkitpro_lib(DEVKITPRO_GBA gba.h gba libgba)
  endif ()
  if ("nds" IN_LIST DevkitPro_FIND_COMPONENTS)
    _find_devkitpro_lib(DEVKITPRO_NDS nds.h nds9 libnds)
  endif ()
  if ("3ds" IN_LIST DevkitPro_FIND_COMPONENTS)
    _find_devkitpro_lib(DEVKITPRO_3DS 3ds.h ctru libctru)
  endif ()
  if ("switch" IN_LIST DevkitPro_FIND_COMPONENTS)
    _find_devkitpro_lib(DEVKITPRO_SWITCH switch.h nx libnx)
  endif ()
  if ("gamecube" IN_LIST DevkitPro_FIND_COMPONENTS)
    _find_devkitpro_lib(DEVKITPRO_GAMECUBE gccore.h ogc libogc cube)
  endif ()
  if ("wii" IN_LIST DevkitPro_FIND_COMPONENTS)
    _find_devkitpro_lib(DEVKITPRO_WII gccore.h ogc libogc wii)
  endif ()

else ()
  set(DEVKITPRO_FOUND FALSE)
  message(STATUS "No devkitPro found")
  message(STATUS "  Searched in ${_DEVKITPRO_POSSIBLE_PATHS}")
endif ()

mark_as_advanced(DEVKITPRO_ROOT)
