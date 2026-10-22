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
InstallApplication
------------------

Install an application in the form the target platform runs.

.. command:: install_application

  Add install rules for an executable target. Desktop platforms install the target itself; console platforms install
  the packaged file the platform toolchain produces next to it (``.gba``, ``.nds``, ``.dol`` and so on).

  .. code-block:: cmake

    install_application(<target>
                        [DESTINATION <dir>]
                        [COMPONENT <component>])

  ``<target>``
    Executable target to install. Required, given positionally as the first argument.

  The options are:

  ``DESTINATION <dir>``
    Install directory relative to the install prefix (default ``bin``).

  ``COMPONENT <component>``
    Install component the rules belong to. Without it the rules fall into CMake's default component.

  ``TOYGINE_TARGET_PLATFORM`` selects the installed file. On console platforms it is
  ``<BINARY_DIR>/<OUTPUT_NAME><suffix>``, with ``OUTPUT_NAME`` falling back to the target name; the devkitPro
  packaging commands write to the same path. The caller still runs the packaging command (``gba_create_rom()`` and
  the like).
#]=======================================================================]
function(install_application target)
  cmake_parse_arguments(PARSE_ARGV 1 ARG
    "" "DESTINATION;COMPONENT" "")

  if (NOT TARGET ${target})
    message(FATAL_ERROR "install_application: '${target}' is not a target")
  endif ()
  if (ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "install_application: unexpected arguments: ${ARG_UNPARSED_ARGUMENTS}")
  endif ()

  if (NOT DEFINED ARG_DESTINATION)
    set(ARG_DESTINATION bin)
  endif ()
  set(component_args )
  if (DEFINED ARG_COMPONENT)
    set(component_args COMPONENT ${ARG_COMPONENT})
  endif ()

  if (TOYGINE_TARGET_PLATFORM STREQUAL "Windows Desktop"
      OR TOYGINE_TARGET_PLATFORM STREQUAL "Linux Desktop"
      OR TOYGINE_TARGET_PLATFORM STREQUAL "macOS Desktop")
    install(TARGETS ${target}
            RUNTIME DESTINATION ${ARG_DESTINATION} ${component_args}
            BUNDLE DESTINATION ${ARG_DESTINATION} ${component_args})
    return ()
  endif ()

  if (TOYGINE_TARGET_PLATFORM STREQUAL "Sega MD")
    set(suffix .bin)
  elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GBA")
    set(suffix .gba)
  elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo DS")
    set(suffix .nds)
  elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo 3DS")
    set(suffix .3dsx)
  elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Switch")
    set(suffix .nro)
  elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GameCube" OR TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Wii")
    set(suffix .dol)
  elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Wii U")
    set(suffix .rpx)
  else ()
    message(FATAL_ERROR "install_application: no install rule for platform '${TOYGINE_TARGET_PLATFORM}'")
  endif ()

  get_target_property(binary_dir ${target} BINARY_DIR)
  get_target_property(output_name ${target} OUTPUT_NAME)
  if (NOT output_name)
    set(output_name ${target})
  endif ()

  install(FILES "${binary_dir}/${output_name}${suffix}" DESTINATION ${ARG_DESTINATION} ${component_args})
endfunction()
