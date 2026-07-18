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
FindClownMDSDK
--------------

Detect a ClownMDSDK installation for Sega Mega Drive / Mega-CD development.

Locates the ClownMDSDK root and its bundled ``m68k-elf`` toolchain, the freestanding header ``clownmdsdk.h``, the
bundled ``toolchain.cmake``, and the linker scripts for the three executable flavors (cartridge, Mega-CD initial
program, Mega-CD sub program). ClownMDSDK ships a single target and toolchain, so no per-console components are
exposed. The module never fails when the SDK is missing: it reports what was found, leaving the decision to the
caller.

The installation root is searched, in order, in the ``CLOWNMDSDK`` environment variable, ``/opt/clownmdsdk``, then
``C:/clownmdsdk``.

.. code-block:: cmake

  find_package(ClownMDSDK)

Result Variables
^^^^^^^^^^^^^^^^^

This module defines the following variables:

``CLOWNMDSDK_FOUND``
  ``TRUE`` if the ClownMDSDK installation root was found.

``CLOWNMDSDK_ROOT``
  Root directory of the ClownMDSDK installation.

``CLOWNMDSDK_M68K_GCC``, ``CLOWNMDSDK_M68K_GXX``
  Paths to the bundled ``m68k-elf-gcc`` and ``m68k-elf-g++`` compilers, set when they are present.

``CLOWNMDSDK_INCLUDE_DIR``
  Directory containing ``clownmdsdk.h`` (the sysroot target include dir ``m68k-elf/include``), set when the header is
  present.

``CLOWNMDSDK_TOOLCHAIN_FILE``
  Path to the SDK-provided ``toolchain.cmake``, set when it is present. Pass it via ``--toolchain`` to cross-compile.

``CLOWNMDSDK_LINKER_SCRIPTS``
  List of the linker scripts found in the root (``cartridge.ld``, ``ip.ld``, ``sp.ld``).
#]=======================================================================]

set(_CLOWNMDSDK_POSSIBLE_PATHS $ENV{CLOWNMDSDK} /opt/clownmdsdk C:/clownmdsdk)

find_path(CLOWNMDSDK_ROOT
  NAMES toolchain.cmake
  PATHS ${_CLOWNMDSDK_POSSIBLE_PATHS}
  NO_DEFAULT_PATH
  DOC "Root directory of ClownMDSDK installation"
)

if (CLOWNMDSDK_ROOT)
  set(CLOWNMDSDK_FOUND TRUE)
  message(STATUS "Found ClownMDSDK at: ${CLOWNMDSDK_ROOT}")

  # === Bundled m68k-elf toolchain ===
  find_program(CLOWNMDSDK_M68K_GCC m68k-elf-gcc
    PATHS "${CLOWNMDSDK_ROOT}/bin" NO_DEFAULT_PATH
    DOC "Path to ClownMDSDK m68k gcc compiler")
  find_program(CLOWNMDSDK_M68K_GXX m68k-elf-g++
    PATHS "${CLOWNMDSDK_ROOT}/bin" NO_DEFAULT_PATH
    DOC "Path to ClownMDSDK m68k g++ compiler")

  if (CLOWNMDSDK_M68K_GCC AND CLOWNMDSDK_M68K_GXX)
    message(STATUS "Found m68k-elf toolchain: ${CLOWNMDSDK_M68K_GXX}")
  else ()
    message(STATUS "Not found: m68k-elf toolchain")
  endif ()

  mark_as_advanced(CLOWNMDSDK_M68K_GCC CLOWNMDSDK_M68K_GXX)

  # === Freestanding SDK header ===
  # clownmdsdk.h ships in the sysroot target include dir (m68k-elf/include); ${CLOWNMDSDK_ROOT}/include holds only the
  # bundled auxiliary libraries (asl, clownlzss).
  find_path(CLOWNMDSDK_INCLUDE_DIR
    clownmdsdk.h
    PATHS ${CLOWNMDSDK_ROOT}/m68k-elf/include ${CLOWNMDSDK_ROOT}/include
    NO_DEFAULT_PATH
  )

  if (CLOWNMDSDK_INCLUDE_DIR)
    message(STATUS "Found clownmdsdk.h: ${CLOWNMDSDK_INCLUDE_DIR}/clownmdsdk.h")
  else ()
    message(STATUS "Not found: clownmdsdk.h")
  endif ()

  mark_as_advanced(CLOWNMDSDK_INCLUDE_DIR)

  # === SDK-provided toolchain file ===
  find_file(CLOWNMDSDK_TOOLCHAIN_FILE
    NAMES toolchain.cmake
    PATHS ${CLOWNMDSDK_ROOT}
    NO_DEFAULT_PATH
    DOC "Path to ClownMDSDK toolchain.cmake"
  )

  if (CLOWNMDSDK_TOOLCHAIN_FILE)
    message(STATUS "Found toolchain file: ${CLOWNMDSDK_TOOLCHAIN_FILE}")
  endif ()

  mark_as_advanced(CLOWNMDSDK_TOOLCHAIN_FILE)

  # === Linker scripts for the executable flavors ===
  # cartridge.ld — Mega Drive cartridge; ip.ld — Mega-CD initial program; sp.ld — Mega-CD sub program.
  set(CLOWNMDSDK_LINKER_SCRIPTS)
  foreach (_script cartridge ip sp)
    if (EXISTS "${CLOWNMDSDK_ROOT}/${_script}.ld")
      list(APPEND CLOWNMDSDK_LINKER_SCRIPTS "${CLOWNMDSDK_ROOT}/${_script}.ld")
      message(STATUS "Found linker script: ${_script}.ld")
    else ()
      message(STATUS "Not found: ${_script}.ld")
    endif ()
  endforeach ()

  unset(_script)

else ()
  set(CLOWNMDSDK_FOUND FALSE)
  message(STATUS "No ClownMDSDK found")
  message(STATUS "  Searched in ${_CLOWNMDSDK_POSSIBLE_PATHS}")
endif ()

mark_as_advanced(CLOWNMDSDK_ROOT)
