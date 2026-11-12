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
                        [SOURCES <file>...]
                        [LINK_LIBRARIES <lib>...]
                        [ENGINE_WARNINGS <ON|OFF>]
                        [GBA_TITLE <title>]
                        [GBA_GAMECODE <code>]
                        [GBA_MAKERCODE <code>]
                        [GBA_VERSION <version>]
                        [GBA_MULTIBOOT]
                        [GBA_PAD])

  ``<target>``
    Name of the executable target to create. Required, given positionally as the first argument.

  The options are:

  ``SOURCES <file>...``
    ``.cpp``, ``.hpp``, and ``.inl`` files of the executable.

  ``LINK_LIBRARIES <lib>...``
    Libraries linked ``PRIVATE`` to the target in addition to ``toygine`` and ``toygine::compile::applications``, the
    flags every final application carries; both are always linked.

  ``ENGINE_WARNINGS <ON|OFF>``
    Whether to link ``toygine::compile::warnings``, the engine's warning settings (default ``ON``). Pass ``OFF`` when
    the caller sets the target's warning options itself.

  The ``GBA_*`` options configure the ROM on Nintendo GBA and are silently ignored on other platforms:

  ``GBA_TITLE <title>``
    Game title stored at header offset ``0x0A0``, up to 12 characters, zero-filled when shorter.

  ``GBA_GAMECODE <code>``
    Four-character game code stored at header offset ``0x0AC``; conventionally unique per game.

  ``GBA_MAKERCODE <code>``
    Two-character maker code stored at header offset ``0x0B0``; ``01`` names Nintendo.

  ``GBA_VERSION <version>``
    Version byte stored at header offset ``0x0BC``; start at ``0`` and increment it on revisions.

  ``GBA_MULTIBOOT``
    Builds a multiboot ROM: the image runs from work RAM, so another Game Boy Advance can transfer it over the link
    cable and run it without a cartridge.

  ``GBA_PAD``
    Pads the ROM to the next power-of-two size, which some cartridges and loaders expect.

  The following variables, when set in the calling scope, are baked into the target as compile definitions:

  ``APP_VERSION_MAJOR``, ``APP_VERSION_MINOR``, ``APP_VERSION_PATCH``
    Numeric version components. An unset component falls back to the matching ``PROJECT_VERSION_*`` value of the
    latest ``project()`` call, and to ``0`` when that value is empty.

  ``APP_VERSION_REVISION``
    Short Git commit hash, resolved automatically when Git is available.
#]=======================================================================]
function(console_application target)
  cmake_parse_arguments(PARSE_ARGV 1 ARG
    "GBA_MULTIBOOT;GBA_PAD"
    "ENGINE_WARNINGS;GBA_TITLE;GBA_GAMECODE;GBA_MAKERCODE;GBA_VERSION"
    "SOURCES;LINK_LIBRARIES")

  if ("${target}" STREQUAL "")
    message(FATAL_ERROR "console_application: target name is required")
  endif ()
  if (ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "console_application: unexpected arguments: ${ARG_UNPARSED_ARGUMENTS}")
  endif ()
  if (NOT DEFINED ARG_ENGINE_WARNINGS)
    set(ARG_ENGINE_WARNINGS ON)
  endif ()

  add_executable(${target} ${ARG_SOURCES})

  foreach (component MAJOR MINOR PATCH)
    if (DEFINED APP_VERSION_${component})
      set(value ${APP_VERSION_${component}})
    elseif (NOT "${PROJECT_VERSION_${component}}" STREQUAL "")
      set(value ${PROJECT_VERSION_${component}})
    else ()
      set(value 0)
    endif ()
    target_compile_definitions(${target} PRIVATE APP_VERSION_${component}=${value})
  endforeach ()

  add_git_revision_definition(${target} APP_VERSION_REVISION)

  if (MSVC)
    target_link_options(${target} PRIVATE /SUBSYSTEM:CONSOLE)
  endif ()

  target_link_libraries(${target} PRIVATE toygine toygine::compile::applications ${ARG_LINK_LIBRARIES})
  if (ARG_ENGINE_WARNINGS)
    target_link_libraries(${target} PRIVATE toygine::compile::warnings)
  endif ()

  if (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GBA")
    set(GBA_ARGS "")

    if (ARG_GBA_MULTIBOOT)
      list(APPEND GBA_ARGS MULTIBOOT)
    endif ()

    if (ARG_GBA_PAD)
      list(APPEND GBA_ARGS PAD)
    endif ()

    foreach (keyword TITLE GAMECODE MAKERCODE VERSION)
      if (DEFINED ARG_GBA_${keyword})
        list(APPEND GBA_ARGS "${keyword}" "${ARG_GBA_${keyword}}")
      endif ()
    endforeach ()

    gba_create_rom(${target} ${GBA_ARGS})
  elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo DS")
    nds_create_rom(${target})
  elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo 3DS")
    ctr_create_3dsx(${target})
  elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Switch")
    nx_create_nro(${target})
  elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GameCube" OR TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Wii")
    ogc_create_dol(${target})
  elseif (NOT TOYGINE_TARGET_PLATFORM_IS_DESKTOP)
    message(FATAL_ERROR "Unsupported TOYGINE_TARGET_PLATFORM: ${TOYGINE_TARGET_PLATFORM}")
  endif ()

endfunction()
