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

# Run by cmake -P after the Sega MD benchmark ROM links. Declares cartridge RAM in the ROM header, which the ClownMDSDK
# crt0 leaves blank, so flash carts and emulators map the RAM the runner writes its report to.
#
# Inputs: ROM (the linked binary, patched in place), ASL and P2BIN (tools of ClownMDSDK), TEMPLATE (the .asm file).

foreach (TOYGINE_INPUT IN ITEMS ROM ASL P2BIN TEMPLATE)
  if (NOT DEFINED ${TOYGINE_INPUT})
    message(FATAL_ERROR "md_sram_header.cmake needs -D${TOYGINE_INPUT}=<value>")
  endif ()
endforeach ()

file(SIZE ${ROM} TOYGINE_ROM_SIZE)

# Past 2 MiB a cartridge maps ROM where the RAM would be; switching needs the 0xA130F1 register the runner never writes.
if (TOYGINE_ROM_SIZE GREATER 2097152)
  message(FATAL_ERROR "${ROM} is ${TOYGINE_ROM_SIZE} bytes; RAM at 0x200001 needs a ROM of at most 2 MiB")
endif ()

get_filename_component(TOYGINE_ROM_DIR ${ROM} DIRECTORY)
set(TOYGINE_WORK_DIR ${TOYGINE_ROM_DIR}/md_sram_header)

file(MAKE_DIRECTORY ${TOYGINE_WORK_DIR})
file(COPY_FILE ${ROM} ${TOYGINE_WORK_DIR}/rom.bin)

execute_process(COMMAND ${ASL} -q -i ${TOYGINE_WORK_DIR} -o ${TOYGINE_WORK_DIR}/patched.p ${TEMPLATE}
                RESULT_VARIABLE TOYGINE_ASL_RESULT)

if (NOT TOYGINE_ASL_RESULT EQUAL 0)
  message(FATAL_ERROR "asl failed on ${TEMPLATE} with ${TOYGINE_ASL_RESULT}")
endif ()

execute_process(COMMAND ${P2BIN} ${TOYGINE_WORK_DIR}/patched.p ${ROM} RESULT_VARIABLE TOYGINE_P2BIN_RESULT)

if (NOT TOYGINE_P2BIN_RESULT EQUAL 0)
  message(FATAL_ERROR "p2bin failed on ${TOYGINE_WORK_DIR}/patched.p with ${TOYGINE_P2BIN_RESULT}")
endif ()

file(SIZE ${ROM} TOYGINE_PATCHED_SIZE)

if (NOT TOYGINE_PATCHED_SIZE EQUAL TOYGINE_ROM_SIZE)
  message(FATAL_ERROR "patching changed ${ROM} from ${TOYGINE_ROM_SIZE} to ${TOYGINE_PATCHED_SIZE} bytes")
endif ()
