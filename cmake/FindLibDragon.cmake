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

cmake_minimum_required(VERSION 3.27.0 FATAL_ERROR)

#[=======================================================================[.rst:
FindLibDragon
-------------

Detect a libDragon installation for Nintendo 64 development.

Locates the libDragon installation root with its bundled ``mips64-elf`` toolchain and sysroot, the ``libdragon.h``
header, and the ``N64.cmake`` toolchain file. libDragon ships a single target and toolchain, so no per-console
components are exposed. The module never fails when the SDK is missing: it reports what was found, leaving the
decision to the caller.

The installation root is searched, in order, in the ``N64_INST`` environment variable, then ``/opt/libdragon``.

.. code-block:: cmake

  find_package(LibDragon)

Result Variables
^^^^^^^^^^^^^^^^

This module defines the following variables:

``LIBDRAGON_FOUND``
  ``TRUE`` if the libdragon installation root was found.

``LIBDRAGON_ROOT``
  Root directory of the libdragon installation.

``LIBDRAGON_MIPS64_GCC``, ``LIBDRAGON_MIPS64_GXX``
  Paths to the bundled ``mips64-elf-gcc`` and ``mips64-elf-g++`` compilers, set when they are present.

``LIBDRAGON_INCLUDE_DIR``
  Directory containing ``libdragon.h`` (the sysroot target include dir ``mips64-elf/include``), set when the header is
  present.

``LIBDRAGON_TOOLCHAIN_FILE``
  Path to the ``N64.cmake`` toolchain file, set when it is present. Shipped by the toygine2 toolchain image; a plain
  upstream installation does not carry it.
#]=======================================================================]

set(_LIBDRAGON_POSSIBLE_PATHS $ENV{N64_INST} /opt/libdragon)

find_path(LIBDRAGON_ROOT
  NAMES mips64-elf/include/libdragon.h
  PATHS ${_LIBDRAGON_POSSIBLE_PATHS}
  NO_DEFAULT_PATH
  NO_CMAKE_FIND_ROOT_PATH
  DOC "Root directory of libdragon installation"
)

if (LIBDRAGON_ROOT)
  set(LIBDRAGON_FOUND TRUE)
  message(STATUS "Found libdragon at: ${LIBDRAGON_ROOT}")

  # === Bundled mips64-elf toolchain ===
  find_program(LIBDRAGON_MIPS64_GCC mips64-elf-gcc
    PATHS "${LIBDRAGON_ROOT}/bin" NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH
    DOC "Path to libdragon mips64 gcc compiler")
  find_program(LIBDRAGON_MIPS64_GXX mips64-elf-g++
    PATHS "${LIBDRAGON_ROOT}/bin" NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH
    DOC "Path to libdragon mips64 g++ compiler")

  if (LIBDRAGON_MIPS64_GCC AND LIBDRAGON_MIPS64_GXX)
    message(STATUS "Found mips64-elf toolchain: ${LIBDRAGON_MIPS64_GXX}")
  else ()
    message(STATUS "Not found: mips64-elf toolchain")
  endif ()

  mark_as_advanced(LIBDRAGON_MIPS64_GCC LIBDRAGON_MIPS64_GXX)

  # === SDK header ===
  # libdragon.h ships in the sysroot target include dir (mips64-elf/include), not the root include/ that holds n64.mk.
  find_path(LIBDRAGON_INCLUDE_DIR
    libdragon.h
    PATHS ${LIBDRAGON_ROOT}/mips64-elf/include
    NO_DEFAULT_PATH
    NO_CMAKE_FIND_ROOT_PATH
  )

  if (LIBDRAGON_INCLUDE_DIR)
    message(STATUS "Found libdragon.h: ${LIBDRAGON_INCLUDE_DIR}/libdragon.h")
  else ()
    message(STATUS "Not found: libdragon.h")
  endif ()

  mark_as_advanced(LIBDRAGON_INCLUDE_DIR)

  # === Toolchain file ===
  find_file(LIBDRAGON_TOOLCHAIN_FILE
    NAMES N64.cmake
    PATHS ${LIBDRAGON_ROOT}/cmake
    NO_DEFAULT_PATH
    NO_CMAKE_FIND_ROOT_PATH
    DOC "Path to the libdragon N64.cmake toolchain file"
  )

  if (LIBDRAGON_TOOLCHAIN_FILE)
    message(STATUS "Found toolchain file: ${LIBDRAGON_TOOLCHAIN_FILE}")
  else ()
    message(STATUS "Not found: N64.cmake toolchain file")
  endif ()

  mark_as_advanced(LIBDRAGON_TOOLCHAIN_FILE)

else ()
  set(LIBDRAGON_FOUND FALSE)
  message(STATUS "No libdragon found")
  message(STATUS "  Searched in ${_LIBDRAGON_POSSIBLE_PATHS}")
endif ()

mark_as_advanced(LIBDRAGON_ROOT)
