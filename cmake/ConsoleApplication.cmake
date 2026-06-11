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

cmake_minimum_required(VERSION 3.31.0 FATAL_ERROR)

include(VersionMetadata)

#[=======================================================================[.rst:
ConsoleApplication
------------------

Declare console executable targets shared by engine samples and tests.

.. command:: console_application

  Create a console executable, stamp it with application version definitions, and apply per-platform packaging
  (for example, Game Boy Advance ROM generation).

  .. code-block:: cmake

    console_application(<target>
                        [SOURCES   <file>...]
                        [HEADERS   <file>...]
                        [INLINES   <file>...]
                        [RESOURCES <file>...]
                        [LIBRARIES <lib>...]
                        [GBA_MULTIBOOT] [GBA_PAD]
                        [GBA_TITLE     <title>]
                        [GBA_GAMECODE  <code>]
                        [GBA_MAKERCODE <code>])

  ``<target>``
    Name of the executable target to create. Required, given positionally as the first argument.

  The options are:

  ``SOURCES <file>...``
    ``.cpp`` translation units. Grouped under "Source Files" in IDEs.

  ``HEADERS <file>...``
    ``.hpp`` headers. Grouped under "Header Files" in IDEs.

  ``INLINES <file>...``
    ``.inl`` inline-implementation files. Grouped under "Source Files".

  ``RESOURCES <file>...``
    Platform resource files added to the target.

  ``LIBRARIES <lib>...``
    Libraries linked ``PRIVATE`` to the target.

  The following options apply only when the target platform is Nintendo GBA and are ignored otherwise.
  They are forwarded to ``gba_create_rom`` (``gbafix``), which patches the 192-byte GBA ROM header.

  ``GBA_MULTIBOOT``
    Build a multiboot image that executes from EWRAM (``0x02000000``) instead of cartridge ROM (``0x08000000``),
    for booting over the link cable without a cartridge. Limited to the ~256 KB of EWRAM.

  ``GBA_PAD``
    Pad the ROM up to the next exact power of two. No minimum size is enforced.
    Matches the physical ROM chip sizes expected by flashcarts.

  ``GBA_TITLE <title>``
    ROM title at header offset ``0xA0``: up to 12 uppercase ASCII characters.
    The stripped target file name is used when omitted.

  ``GBA_GAMECODE <code>``
    Four-character game code at header offset ``0xAC`` (the ``XXXX`` part of the ``AGB-XXXX-YYY`` product code).

  ``GBA_MAKERCODE <code>``
    Two-character maker (publisher) code at header offset ``0xB0`` (for example, ``01`` for Nintendo).

  The following variables, when set in the calling scope, are baked into the target as compile definitions:

  ``APP_VERSION_MAJOR``, ``APP_VERSION_MINOR``, ``APP_VERSION_PATCH``
    Numeric version components (default ``0`` when unset).

  ``APP_VERSION_REVISION``
    Short Git commit hash, resolved automatically when Git is available.
#]=======================================================================]
function(console_application target)
  cmake_parse_arguments(PARSE_ARGV 1 ARG
    "GBA_MULTIBOOT;GBA_PAD"
    "GBA_TITLE;GBA_GAMECODE;GBA_MAKERCODE"
    "SOURCES;HEADERS;INLINES;RESOURCES;LIBRARIES")

  if ("${target}" STREQUAL "")
    message(FATAL_ERROR "console_application: target name is required")
  endif ()
  if (ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "console_application: unexpected arguments: ${ARG_UNPARSED_ARGUMENTS}")
  endif ()

  source_group("Source Files" FILES ${ARG_SOURCES} ${ARG_INLINES})
  source_group("Header Files" FILES ${ARG_HEADERS})

  add_executable(${target} ${ARG_SOURCES} ${ARG_HEADERS} ${ARG_INLINES} ${ARG_RESOURCES})

  if (DEFINED APP_VERSION_MAJOR)
    target_compile_definitions(${target} PRIVATE APP_VERSION_MAJOR=${APP_VERSION_MAJOR})
  else ()
    target_compile_definitions(${target} PRIVATE APP_VERSION_MAJOR=0)
  endif ()
  if (DEFINED APP_VERSION_MINOR)
    target_compile_definitions(${target} PRIVATE APP_VERSION_MINOR=${APP_VERSION_MINOR})
  else ()
    target_compile_definitions(${target} PRIVATE APP_VERSION_MINOR=0)
  endif ()
  if (DEFINED APP_VERSION_PATCH)
    target_compile_definitions(${target} PRIVATE APP_VERSION_PATCH=${APP_VERSION_PATCH})
  else ()
    target_compile_definitions(${target} PRIVATE APP_VERSION_PATCH=0)
  endif ()

  add_git_revision_definition(${target} APP_VERSION_REVISION)

  if (MSVC)
    target_link_options(${target} PRIVATE /SUBSYSTEM:CONSOLE)
  endif ()

  target_link_libraries(${target} PRIVATE ${ARG_LIBRARIES})

  if (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GBA")
    set(gbaArgs "")

    if (ARG_GBA_MULTIBOOT)
      list(APPEND gbaArgs MULTIBOOT)
    endif ()
    if (ARG_GBA_PAD)
      list(APPEND gbaArgs PAD)
    endif ()
    if (DEFINED ARG_GBA_TITLE)
      string(LENGTH "${ARG_GBA_TITLE}" titleLength)
      if (titleLength GREATER 12)
        message(FATAL_ERROR "console_application: GBA_TITLE \"${ARG_GBA_TITLE}\" exceeds 12 characters")
      endif ()
      if (NOT "${ARG_GBA_TITLE}" MATCHES "^[A-Z0-9 ]+$")
        message(FATAL_ERROR "console_application: GBA_TITLE \"${ARG_GBA_TITLE}\" must use only uppercase letters, digits, and spaces")
      endif ()

      list(APPEND gbaArgs TITLE ${ARG_GBA_TITLE})
    endif ()
    if (DEFINED ARG_GBA_GAMECODE)
      if (DEFINED ARG_GBA_GAMECODE AND NOT "${ARG_GBA_GAMECODE}" MATCHES "^[A-Z0-9][A-Z0-9][A-Z0-9][A-Z0-9]$")
        message(FATAL_ERROR "console_application: GBA_GAMECODE \"${ARG_GBA_GAMECODE}\" must be exactly 4 uppercase letters or digits")
      endif ()

      list(APPEND gbaArgs GAMECODE ${ARG_GBA_GAMECODE})
    endif ()
    if (DEFINED ARG_GBA_MAKERCODE)
      if (DEFINED ARG_GBA_MAKERCODE AND NOT "${ARG_GBA_MAKERCODE}" MATCHES "^[A-Z0-9][A-Z0-9]$")
        message(FATAL_ERROR "console_application: GBA_MAKERCODE \"${ARG_GBA_MAKERCODE}\" must be exactly 2 uppercase letters or digits")
      endif ()

      list(APPEND gbaArgs MAKERCODE ${ARG_GBA_MAKERCODE})
    endif ()
    if (NOT CMAKE_BUILD_TYPE STREQUAL "Release")
      list(APPEND gbaArgs DEBUG 1)
    endif ()

    gba_create_rom(${target} ${gbaArgs})
  endif ()

endfunction()
