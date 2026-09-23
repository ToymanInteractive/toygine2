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

# The CTest test that runs the built-in runner's binary; included by tests/CMakeLists.txt for that runner only.

if (TOYGINE_TARGET_PLATFORM_IS_DESKTOP)
  # Exit code 0 is the whole contract: the runner reports a failed assertion through it.
  add_test(NAME units COMMAND ${TOYGINE_LIBRARY_NAME}-units)
elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Sega MD")
  # If find_program locates blastem, CTest runs the Sega MD ROM under it like any other test.
  find_program(TOYGINE_BLASTEM blastem)

  if (TOYGINE_BLASTEM)
    # -b runs headless and exits after the given frame count; the cartridge target file is the ROM itself.
    # -t keeps BlastEm on its own stdout: given a pipe instead of a terminal it forks an xterm and hangs.
    add_test(NAME units
             COMMAND ${TOYGINE_BLASTEM} -t -b 1800 $<TARGET_FILE:${TOYGINE_LIBRARY_NAME}-units>)

    # BlastEm always exits with 0, so the verdict is read from the report it prints as KDEBUG MESSAGE lines.
    # The summary line prints last, so matching it also catches a run that died halfway through the report.
    set_tests_properties(units PROPERTIES TIMEOUT 60
                                                  PASS_REGULAR_EXPRESSION "# assertions passed=[0-9]+ failed=0")
  else ()
    message(STATUS "blastem not found: the test ROM is built but no CTest test runs it")
  endif ()
elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GBA")
  # If find_program locates mgba-headless, CTest runs the GBA ROM under it like any other test.
  find_program(TOYGINE_MGBA_HEADLESS mgba-headless)

  if (TOYGINE_MGBA_HEADLESS)
    find_file(TOYGINE_GBA_BIOS gba_bios.bin HINTS /usr/local/share/emibios NO_CMAKE_FIND_ROOT_PATH)

    if (TOYGINE_GBA_BIOS)
      set(TOYGINE_MGBA_BIOS_ARGS -b ${TOYGINE_GBA_BIOS} -C skipBios=1)
    endif ()

    # The runner stops on SWI 3, leaves the verdict in r0 and logs at info level.
    add_test(NAME units
             COMMAND ${TOYGINE_MGBA_HEADLESS} ${TOYGINE_MGBA_BIOS_ARGS} -S 3 -R r0 -l 15
                     # gba_create_rom records the ROM path in DKP_FILE; the target file is the ELF.
                     $<TARGET_PROPERTY:${TOYGINE_LIBRARY_NAME}-units,DKP_FILE>)

    # A ROM that never reaches SWI 3 would otherwise run until the job's own timeout.
    set_tests_properties(units PROPERTIES TIMEOUT 60)
  else ()
    message(STATUS "mgba-headless not found: the test ROM is built but no CTest test runs it")
  endif ()
endif ()
