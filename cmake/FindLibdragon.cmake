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
FindLibdragon
-------------

Detect a Libdragon installation for Nintendo 64 development.

Locates the Libdragon root and its ``mips64-elf`` toolchain, the header ``libdragon.h``, the runtime libraries, the
makefile fragment ``n64.mk``, the linker scripts for the three output flavors (ROM executable, dynamic overlay, RSP
microcode), the IPL3 ROM header stub, and the host tools that build a ROM and its assets. Libdragon ships a single
target and toolchain, so no per-console components are exposed. The module never fails when the SDK is missing: it
reports what was found, leaving the decision to the caller.

Libdragon carries no CMake toolchain file: its build rules live in ``n64.mk``, so a cross-compiling caller supplies a
toolchain file of its own.

The installation root is searched, in order, in the ``N64_INST`` environment variable, ``/opt/libdragon``, then
``/usr/local``.

.. code-block:: cmake

  find_package(Libdragon)

Result Variables
^^^^^^^^^^^^^^^^^

This module defines the following variables:

``LIBDRAGON_FOUND``
  ``TRUE`` if the Libdragon installation root was found.

``LIBDRAGON_ROOT``
  Root directory of the Libdragon installation (``N64_INST`` in the SDK's own build rules).

``LIBDRAGON_MIPS64_GCC``, ``LIBDRAGON_MIPS64_GXX``
  Paths to the ``mips64-elf-gcc`` and ``mips64-elf-g++`` compilers, set when they are present.

``LIBDRAGON_INCLUDE_DIR``
  Directory containing ``libdragon.h`` (the sysroot target include dir ``mips64-elf/include``), set when the header is
  present.

``LIBDRAGON_LIBRARY``, ``LIBDRAGON_SYS_LIBRARY``
  Paths to ``libdragon.a`` and ``libdragonsys.a``, set when they are present. The latter holds the newlib system stubs
  the former calls into.

``LIBDRAGON_N64_MK``
  Path to the SDK-provided ``n64.mk``, set when it is present. It states the flags a toolchain file has to reproduce.

``LIBDRAGON_LINKER_SCRIPTS``
  List of the linker scripts found in ``mips64-elf/lib`` (``n64.ld``, ``dso.ld``, ``rsp.ld``).

``LIBDRAGON_ROM_HEADER``
  Path to the IPL3 ROM header stub ``mips64-elf/lib/header``, set when it is present. ``n64tool`` writes it into the
  leading bytes of the ROM.

``LIBDRAGON_TOOLS``
  List of the host tools found in ``bin``. Each tool also gets its own variable, upper-cased and prefixed:
  ``LIBDRAGON_N64TOOL``, ``LIBDRAGON_MKDFS``, and so on.
#]=======================================================================]

set(_LIBDRAGON_POSSIBLE_PATHS $ENV{N64_INST} /opt/libdragon /usr/local)

# Libdragon builds one triplet, and its sysroot directory holds the headers, libraries, and linker scripts.
find_path(LIBDRAGON_ROOT
  NAMES mips64-elf
  PATHS ${_LIBDRAGON_POSSIBLE_PATHS}
  NO_DEFAULT_PATH
  DOC "Root directory of Libdragon installation"
)

if (LIBDRAGON_ROOT)
  set(LIBDRAGON_FOUND TRUE)
  message(STATUS "Found Libdragon at: ${LIBDRAGON_ROOT}")

  # === mips64-elf toolchain ===
  find_program(LIBDRAGON_MIPS64_GCC mips64-elf-gcc
    PATHS "${LIBDRAGON_ROOT}/bin" NO_DEFAULT_PATH
    DOC "Path to Libdragon mips64 gcc compiler")
  find_program(LIBDRAGON_MIPS64_GXX mips64-elf-g++
    PATHS "${LIBDRAGON_ROOT}/bin" NO_DEFAULT_PATH
    DOC "Path to Libdragon mips64 g++ compiler")

  if (LIBDRAGON_MIPS64_GCC AND LIBDRAGON_MIPS64_GXX)
    message(STATUS "Found mips64-elf toolchain: ${LIBDRAGON_MIPS64_GXX}")
  else ()
    message(STATUS "Not found: mips64-elf toolchain")
  endif ()

  mark_as_advanced(LIBDRAGON_MIPS64_GCC LIBDRAGON_MIPS64_GXX)

  # === SDK header ===
  # libdragon.h ships in the sysroot target include dir (mips64-elf/include); ${LIBDRAGON_ROOT}/include holds
  # n64.mk, which the SDK's own build rules read.
  find_path(LIBDRAGON_INCLUDE_DIR
    libdragon.h
    PATHS ${LIBDRAGON_ROOT}/mips64-elf/include
    NO_DEFAULT_PATH
  )

  if (LIBDRAGON_INCLUDE_DIR)
    message(STATUS "Found libdragon.h: ${LIBDRAGON_INCLUDE_DIR}/libdragon.h")
  else ()
    message(STATUS "Not found: libdragon.h")
  endif ()

  mark_as_advanced(LIBDRAGON_INCLUDE_DIR)

  # === Runtime libraries ===
  # libdragon.a — the runtime; libdragonsys.a — the newlib system stubs it calls into.
  find_library(LIBDRAGON_LIBRARY
    NAMES dragon
    PATHS ${LIBDRAGON_ROOT}/mips64-elf/lib
    NO_DEFAULT_PATH
  )
  find_library(LIBDRAGON_SYS_LIBRARY
    NAMES dragonsys
    PATHS ${LIBDRAGON_ROOT}/mips64-elf/lib
    NO_DEFAULT_PATH
  )

  if (LIBDRAGON_LIBRARY AND LIBDRAGON_SYS_LIBRARY)
    message(STATUS "Found libdragon: ${LIBDRAGON_LIBRARY}")
  else ()
    message(STATUS "Not found: libdragon")
  endif ()

  mark_as_advanced(LIBDRAGON_LIBRARY LIBDRAGON_SYS_LIBRARY)

  # === SDK-provided makefile fragment ===
  # CMake cannot consume it; it is where the SDK states its compiler, linker, and ROM flags.
  find_file(LIBDRAGON_N64_MK
    NAMES n64.mk
    PATHS ${LIBDRAGON_ROOT}/include
    NO_DEFAULT_PATH
    DOC "Path to Libdragon n64.mk"
  )

  if (LIBDRAGON_N64_MK)
    message(STATUS "Found makefile fragment: ${LIBDRAGON_N64_MK}")
  endif ()

  mark_as_advanced(LIBDRAGON_N64_MK)

  # === Linker scripts for the output flavors ===
  # n64.ld — ROM executable; dso.ld — dynamically loaded overlay; rsp.ld — RSP microcode.
  set(LIBDRAGON_LINKER_SCRIPTS)
  foreach (_script n64 dso rsp)
    if (EXISTS "${LIBDRAGON_ROOT}/mips64-elf/lib/${_script}.ld")
      list(APPEND LIBDRAGON_LINKER_SCRIPTS "${LIBDRAGON_ROOT}/mips64-elf/lib/${_script}.ld")
      message(STATUS "Found linker script: ${_script}.ld")
    else ()
      message(STATUS "Not found: ${_script}.ld")
    endif ()
  endforeach ()

  unset(_script)

  # === IPL3 ROM header stub ===
  # The bootcode n64tool copies into the leading bytes of the ROM; the file has no extension.
  find_file(LIBDRAGON_ROM_HEADER
    NAMES header
    PATHS ${LIBDRAGON_ROOT}/mips64-elf/lib
    NO_DEFAULT_PATH
    DOC "Path to Libdragon IPL3 ROM header stub"
  )

  if (LIBDRAGON_ROM_HEADER)
    message(STATUS "Found ROM header stub: ${LIBDRAGON_ROM_HEADER}")
  else ()
    message(STATUS "Not found: ROM header stub")
  endif ()

  mark_as_advanced(LIBDRAGON_ROM_HEADER)

  # === Host tools that build a ROM and its assets ===
  # n64tool builds the ROM image, n64sym appends the symbol table, n64elfcompress compresses overlays, mkdfs packs a
  # DragonFS filesystem, and the remaining converters bake assets into their runtime formats.
  set(LIBDRAGON_TOOLS)
  foreach (_tool n64tool n64sym n64elfcompress mkdfs mkasset mksprite mkfont mkmodel audioconv64)
    string(TOUPPER ${_tool} _tool_var)

    find_program(LIBDRAGON_${_tool_var} ${_tool}
      PATHS "${LIBDRAGON_ROOT}/bin" NO_DEFAULT_PATH
      DOC "Path to Libdragon ${_tool}")

    if (LIBDRAGON_${_tool_var})
      list(APPEND LIBDRAGON_TOOLS "${LIBDRAGON_${_tool_var}}")
      message(STATUS "Found tool: ${_tool}")
    else ()
      message(STATUS "Not found: ${_tool}")
    endif ()

    mark_as_advanced(LIBDRAGON_${_tool_var})
  endforeach ()

  unset(_tool)
  unset(_tool_var)

else ()
  set(LIBDRAGON_FOUND FALSE)
  message(STATUS "No Libdragon found")
  message(STATUS "  Searched in ${_LIBDRAGON_POSSIBLE_PATHS}")
endif ()

mark_as_advanced(LIBDRAGON_ROOT)
