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

#-----------------------------------------------------------------------------------------------------------------------
# nanobench support
#-----------------------------------------------------------------------------------------------------------------------

# Whether the build host can time a callable: no console toolchain here gives nanobench a steady clock or iostreams.
set(TOYGINE_TARGET_PLATFORM_SUPPORT_NANOBENCH ${TOYGINE_TARGET_PLATFORM_IS_DESKTOP})

#-----------------------------------------------------------------------------------------------------------------------
# Runner platform layer
#-----------------------------------------------------------------------------------------------------------------------

# The entry point, the timer and the output channels live in one file per target, so the runner has no target branches.
if (TOYGINE_TARGET_PLATFORM_IS_DESKTOP)
  set(TOYGINE_BENCHMARK_RUNNER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/runner/platforms/desktop.cpp)
elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GBA")
  set(TOYGINE_BENCHMARK_RUNNER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/runner/platforms/gba.cpp)
elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Sega MD")
  set(TOYGINE_BENCHMARK_RUNNER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/runner/platforms/md.cpp)
else ()
  # No other console has a runner timer yet: an emulator of a CPU with a cache gives timings that mean nothing.
  return ()
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Benchmarks translation units
#-----------------------------------------------------------------------------------------------------------------------

# Benchmarks of engine modules: the glob adds a file to the target, BENCHMARK_CASE registers the cases inside it.
file(GLOB_RECURSE TOYGINE_MODULE_BENCHMARKS_SRC CONFIGURE_DEPENDS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.benchmark.cpp")

#-----------------------------------------------------------------------------------------------------------------------
# Benchmarks built with nanobench
#-----------------------------------------------------------------------------------------------------------------------

if (TOYGINE_TARGET_PLATFORM_SUPPORT_NANOBENCH)
  # Declares the nanobench target and the nanobench::nanobench alias over the vendored sources in thirdparty/nanobench.
  include(nanobench)

  add_executable(${TOYGINE_LIBRARY_NAME}-benchmarks ${TOYGINE_BENCHMARK_RUNNER_SRC} ${TOYGINE_MODULE_BENCHMARKS_SRC})

  target_include_directories(${TOYGINE_LIBRARY_NAME}-benchmarks
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/runner ${CMAKE_CURRENT_SOURCE_DIR}/../include)
  target_link_libraries(${TOYGINE_LIBRARY_NAME}-benchmarks PRIVATE nanobench::nanobench ${TOYGINE_LIBRARY_NAME})
  set_target_properties(${TOYGINE_LIBRARY_NAME}-benchmarks PROPERTIES FOLDER "benchmarks")

  # Bodies spell toy::benchmark::Bench; this define makes it nanobench's table rather than the console runner's.
  target_compile_definitions(${TOYGINE_LIBRARY_NAME}-benchmarks PRIVATE TOYGINE_BENCHMARKS_USE_NANOBENCH)

  # Rows timing the standard library are for comparing on a workstation; console builds leave them out.
  target_compile_definitions(${TOYGINE_LIBRARY_NAME}-benchmarks PRIVATE TOYGINE_BENCHMARKS_STD_REFERENCE)

  # The tables are for a human reading the log, the CSV lines for tools/builder/bmf_from_csv.py reading the same log.
  add_custom_target(${TOYGINE_LIBRARY_NAME}-benchmarks-run
    COMMAND $<TARGET_FILE:${TOYGINE_LIBRARY_NAME}-benchmarks> --csv
    WORKING_DIRECTORY $<TARGET_FILE_DIR:${TOYGINE_LIBRARY_NAME}-benchmarks>
    COMMENT "Running benchmarks"
    VERBATIM
  )

  # A generator expression names the binary but orders nothing, so the run target asks for the build itself.
  add_dependencies(${TOYGINE_LIBRARY_NAME}-benchmarks-run ${TOYGINE_LIBRARY_NAME}-benchmarks)
  set_target_properties(${TOYGINE_LIBRARY_NAME}-benchmarks-run PROPERTIES FOLDER "benchmarks")
else ()
  #---------------------------------------------------------------------------------------------------------------------
  # Benchmarks built with the runner written for consoles
  #---------------------------------------------------------------------------------------------------------------------

  find_package(Git QUIET)

  set(TOYGINE_BENCHMARK_GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)

  # Runs on every build, so the report names the commit the ROM came from; the header changes only with that commit.
  add_custom_target(${TOYGINE_LIBRARY_NAME}-benchmarks-build-id
    COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=${CMAKE_SOURCE_DIR} -DGIT_EXECUTABLE=${GIT_EXECUTABLE}
            -DOUTPUT=${TOYGINE_BENCHMARK_GENERATED_DIR}/benchmark_build_id.hpp
            -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/build_id.cmake
    BYPRODUCTS ${TOYGINE_BENCHMARK_GENERATED_DIR}/benchmark_build_id.hpp
    COMMENT "Recording the commit of the benchmark ROM"
    VERBATIM
  )
  set_target_properties(${TOYGINE_LIBRARY_NAME}-benchmarks-build-id PROPERTIES FOLDER "benchmarks")

  # Sega MD has no plain executable: the cartridge linker script and its startup stub are what give the binary an entry.
  if (TOYGINE_TARGET_PLATFORM STREQUAL "Sega MD")
    add_cartridge_executable(${TOYGINE_LIBRARY_NAME}-benchmarks
        ${TOYGINE_BENCHMARK_RUNNER_SRC} ${TOYGINE_MODULE_BENCHMARKS_SRC})
  else ()
    add_executable(${TOYGINE_LIBRARY_NAME}-benchmarks ${TOYGINE_BENCHMARK_RUNNER_SRC} ${TOYGINE_MODULE_BENCHMARKS_SRC})
  endif ()

  add_dependencies(${TOYGINE_LIBRARY_NAME}-benchmarks ${TOYGINE_LIBRARY_NAME}-benchmarks-build-id)

  target_include_directories(${TOYGINE_LIBRARY_NAME}-benchmarks
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/runner ${CMAKE_CURRENT_SOURCE_DIR}/../include
              ${TOYGINE_BENCHMARK_GENERATED_DIR})
  target_link_libraries(${TOYGINE_LIBRARY_NAME}-benchmarks PRIVATE ${TOYGINE_LIBRARY_NAME})
  set_target_properties(${TOYGINE_LIBRARY_NAME}-benchmarks PROPERTIES FOLDER "benchmarks")

  # The report heading names the target and the build type, so two reports on one flash cart cannot be confused.
  target_compile_definitions(${TOYGINE_LIBRARY_NAME}-benchmarks
      PRIVATE "TOYGINE_BENCHMARKS_PLATFORM=\"${TOYGINE_TARGET_PLATFORM}\""
              "TOYGINE_BENCHMARKS_BUILD_TYPE=\"${CMAKE_BUILD_TYPE}\"")

  if (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GBA")
    # The console cannot load the ELF; flash carts and mGBA take the ROM.
    gba_create_rom(${TOYGINE_LIBRARY_NAME}-benchmarks)

    find_program(TOYGINE_MGBA_HEADLESS mgba-headless)

    if (TOYGINE_MGBA_HEADLESS)
      find_file(TOYGINE_GBA_BIOS gba_bios.bin HINTS /usr/local/share/emibios NO_CMAKE_FIND_ROOT_PATH)

      if (TOYGINE_GBA_BIOS)
        set(TOYGINE_MGBA_BIOS_ARGS -b ${TOYGINE_GBA_BIOS} -C skipBios=1)
      endif ()

      # The runner stops on SWI 3 and logs at info level; DKP_FILE is the ROM gba_create_rom wrote, not the ELF.
      add_custom_target(${TOYGINE_LIBRARY_NAME}-benchmarks-run
        COMMAND ${TOYGINE_MGBA_HEADLESS} ${TOYGINE_MGBA_BIOS_ARGS} -S 3 -R r0 -l 15
                $<TARGET_PROPERTY:${TOYGINE_LIBRARY_NAME}-benchmarks,DKP_FILE>
        COMMENT "Running benchmarks under mgba-headless"
        VERBATIM
      )

      add_dependencies(${TOYGINE_LIBRARY_NAME}-benchmarks-run ${TOYGINE_LIBRARY_NAME}-benchmarks)
      set_target_properties(${TOYGINE_LIBRARY_NAME}-benchmarks-run PROPERTIES FOLDER "benchmarks")
    else ()
      message(STATUS "mgba-headless not found: the benchmark ROM is built without a target that runs it")
    endif ()
  elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Sega MD")
    find_program(TOYGINE_CLOWNMDSDK_ASL asl PATHS ${CLOWNMDSDK_ROOT}/bin NO_DEFAULT_PATH REQUIRED)
    find_program(TOYGINE_CLOWNMDSDK_P2BIN p2bin PATHS ${CLOWNMDSDK_ROOT}/bin NO_DEFAULT_PATH REQUIRED)

    # The crt0 header of ClownMDSDK declares no RAM; without the patch a flash cart would not save the report.
    add_custom_command(TARGET ${TOYGINE_LIBRARY_NAME}-benchmarks POST_BUILD
      COMMAND ${CMAKE_COMMAND} -DROM=$<TARGET_FILE:${TOYGINE_LIBRARY_NAME}-benchmarks>
              -DASL=${TOYGINE_CLOWNMDSDK_ASL} -DP2BIN=${TOYGINE_CLOWNMDSDK_P2BIN}
              -DTEMPLATE=${CMAKE_CURRENT_SOURCE_DIR}/runner/platforms/md_sram_header.asm
              -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/md_sram_header.cmake
      COMMENT "Declaring cartridge RAM in the benchmark ROM header"
      VERBATIM
    )

    # A POST_BUILD step runs only with the link, so a change to the patch relinks the ROM.
    set_property(TARGET ${TOYGINE_LIBRARY_NAME}-benchmarks APPEND PROPERTY LINK_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/runner/platforms/md_sram_header.asm
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/md_sram_header.cmake)

    find_program(TOYGINE_BLASTEM blastem)

    if (TOYGINE_BLASTEM)
      # BlastEm exits after -b frames whatever the ROM prints; 20000 is about four times what the report takes.
      # -t keeps BlastEm on its own stdout: given a pipe instead of a terminal it forks an xterm and hangs.
      add_custom_target(${TOYGINE_LIBRARY_NAME}-benchmarks-run
        COMMAND ${TOYGINE_BLASTEM} -t -b 20000 $<TARGET_FILE:${TOYGINE_LIBRARY_NAME}-benchmarks>
        COMMENT "Running benchmarks under blastem"
        VERBATIM
      )

      add_dependencies(${TOYGINE_LIBRARY_NAME}-benchmarks-run ${TOYGINE_LIBRARY_NAME}-benchmarks)
      set_target_properties(${TOYGINE_LIBRARY_NAME}-benchmarks-run PROPERTIES FOLDER "benchmarks")
    else ()
      message(STATUS "blastem not found: the benchmark ROM is built without a target that runs it")
    endif ()
  endif ()
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Unit tests
#-----------------------------------------------------------------------------------------------------------------------

# The tests need doctest and a host that runs the binary, so the tree is skipped off desktop like the benchmarks above.
if (TOYGINE_BUILD_TESTS AND TOYGINE_TARGET_PLATFORM_IS_DESKTOP)
  enable_testing()
  add_subdirectory(runner/tests)
endif ()
