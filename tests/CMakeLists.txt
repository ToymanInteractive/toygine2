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

if (TOYGINE_TARGET_PLATFORM STREQUAL "Windows Desktop")

  if (MSVC)

    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   /EHsc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")

  endif ()

endif ()

#-----------------------------------------------------------------------------------------------------------------------
# CTest support
#-----------------------------------------------------------------------------------------------------------------------

set(TOYGINE_DESKTOP_PLATFORMS "Windows Desktop" "Linux Desktop" "macOS Desktop")

# Whether the build host runs the test binaries itself, which is also what the DocTest half of the build needs.
# A console target reaches CTest through an emulator instead, registered below.
set(TOYGINE_TARGET_PLATFORM_SUPPORT_CTEST OFF)
if (TOYGINE_TARGET_PLATFORM IN_LIST TOYGINE_DESKTOP_PLATFORMS)
  set(TOYGINE_TARGET_PLATFORM_SUPPORT_CTEST ON)
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Remove all of the extra targets
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
#-----------------------------------------------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------------------------------------------
# Test translation units
#-----------------------------------------------------------------------------------------------------------------------

# The runner's platform layer: output, exit, and the entry point. A file, not a macro, so the runner itself carries
# no target branches.
if (TOYGINE_TARGET_PLATFORM IN_LIST TOYGINE_DESKTOP_PLATFORMS)
  set(TOYGINE_RUNNER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/runner/platforms/desktop.cpp)
elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Sega MD")
  set(TOYGINE_RUNNER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/runner/platforms/md.cpp)
elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GBA")
  set(TOYGINE_RUNNER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/runner/platforms/gba.cpp)
elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo DS")
  set(TOYGINE_RUNNER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/runner/platforms/nds.cpp)
elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo 3DS")
  set(TOYGINE_RUNNER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/runner/platforms/3ds.cpp)
elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Switch")
  set(TOYGINE_RUNNER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/runner/platforms/switch.cpp)
elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GameCube")
  set(TOYGINE_RUNNER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/runner/platforms/gamecube.cpp)
elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Wii")
  set(TOYGINE_RUNNER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/runner/platforms/wii.cpp)
elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Wii U")
  set(TOYGINE_RUNNER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/runner/platforms/wii_u.cpp)
else ()
  message(FATAL_ERROR "Unsupported platform: ${TOYGINE_TARGET_PLATFORM}")
endif ()

# Tests of engine modules. Both runners build them, so a drift between the two splits the verdict or fails the build.
file(GLOB_RECURSE TOYGINE_MODULE_TESTS_SRC CONFIGURE_DEPENDS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.test.cpp")
list(FILTER TOYGINE_MODULE_TESTS_SRC EXCLUDE REGEX "^runner/")

# Tests of the runner itself. Written against doctest and inspecting its internals, so only doctest builds them.
file(GLOB TOYGINE_RUNNER_TESTS_SRC CONFIGURE_DEPENDS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/runner/tests/*.test.cpp")

#-----------------------------------------------------------------------------------------------------------------------
# Tests built with the runner written for consoles
#-----------------------------------------------------------------------------------------------------------------------

# Sega MD has no plain executable: the cartridge linker script and its startup stub are what give the binary an entry.
if (TOYGINE_TARGET_PLATFORM STREQUAL "Sega MD")
  add_cartridge_executable(${TOYGINE_LIBRARY_NAME}-units-builtin ${TOYGINE_RUNNER_SRC} ${TOYGINE_MODULE_TESTS_SRC})
else ()
  add_executable(${TOYGINE_LIBRARY_NAME}-units-builtin ${TOYGINE_RUNNER_SRC} ${TOYGINE_MODULE_TESTS_SRC})
endif ()

target_include_directories(${TOYGINE_LIBRARY_NAME}-units-builtin
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/runner ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(${TOYGINE_LIBRARY_NAME}-units-builtin PRIVATE ${TOYGINE_LIBRARY_NAME})

# The ELF alone is not something the console can load; the ROM is what the next spec hands to an emulator.
if (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GBA")
  gba_create_rom(${TOYGINE_LIBRARY_NAME}-units-builtin)
elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo DS")
  nds_create_rom(${TOYGINE_LIBRARY_NAME}-units-builtin)
elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo 3DS")
  ctr_create_3dsx(${TOYGINE_LIBRARY_NAME}-units-builtin)
elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Switch")
  nx_create_nro(${TOYGINE_LIBRARY_NAME}-units-builtin)
elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GameCube")
  ogc_create_dol(${TOYGINE_LIBRARY_NAME}-units-builtin)
elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Wii")
  ogc_create_dol(${TOYGINE_LIBRARY_NAME}-units-builtin)
elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Wii U")
  wut_create_rpx(${TOYGINE_LIBRARY_NAME}-units-builtin)
endif ()

# The ROM reaches CTest through the emulator the toolchain image ships, so the console target reports the way every
# other one does instead of being launched by hand from CI.
if (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GBA")
  find_program(TOYGINE_MGBA_HEADLESS mgba-headless)

  if (TOYGINE_MGBA_HEADLESS)
    # The runner stops on SWI 3, leaves the verdict in r0 and logs at info level. DKP_FILE is where gba_create_rom
    # records the ROM it wrote, which is not the target file the ELF leaves behind.
    add_test(NAME units-builtin
             COMMAND ${TOYGINE_MGBA_HEADLESS} -S 3 -R r0 -l 15
                     $<TARGET_PROPERTY:${TOYGINE_LIBRARY_NAME}-units-builtin,DKP_FILE>)

    # A ROM that never reaches SWI 3 would otherwise run until the job's own timeout.
    set_tests_properties(units-builtin PROPERTIES TIMEOUT 60)
  else ()
    message(STATUS "mgba-headless not found: the test ROM is built but no CTest test runs it")
  endif ()
endif ()

if (TOYGINE_TARGET_PLATFORM_SUPPORT_CTEST)
  include(FetchContent)
  include(CTest)

  FetchContent_Declare(doctest GIT_REPOSITORY https://github.com/doctest/doctest GIT_TAG v2.5.3 GIT_SHALLOW TRUE)
  FetchContent_MakeAvailable(doctest)

  list(APPEND CMAKE_MODULE_PATH ${doctest_SOURCE_DIR}/scripts/cmake)

  include(doctest)

  add_executable(${TOYGINE_LIBRARY_NAME}-units
      ${TOYGINE_MODULE_TESTS_SRC}
      ${TOYGINE_RUNNER_TESTS_SRC}
      ctest_main.cpp)
  target_compile_definitions(${TOYGINE_LIBRARY_NAME}-units PRIVATE DOCTEST_CONFIG_SUPER_FAST_ASSERTS
                                                                  TOYGINE_TESTS_USE_DOCTEST)

  target_include_directories(${TOYGINE_LIBRARY_NAME}-units PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
  target_include_directories(${TOYGINE_LIBRARY_NAME}-units PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/runner)
  target_link_libraries(${TOYGINE_LIBRARY_NAME}-units PRIVATE doctest::doctest ${TOYGINE_LIBRARY_NAME})
  doctest_discover_tests(${TOYGINE_LIBRARY_NAME}-units)

  # Exit code 0 is the whole contract: every module test must reach the same verdict under both runners.
  add_test(NAME units-builtin COMMAND ${TOYGINE_LIBRARY_NAME}-units-builtin)

  # CodeCoverage aborts on anything but a gcov-compatible toolchain, so keep the option a no-op elsewhere (MSVC)
  set(TOYGINE_COMPILER_SUPPORT_COVERAGE OFF)
  if (CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|(Apple)?[Cc]lang)$")
    set(TOYGINE_COMPILER_SUPPORT_COVERAGE ON)
  endif ()

  if (TOYGINE_TESTS_ENABLE_COVERAGE AND NOT TOYGINE_COMPILER_SUPPORT_COVERAGE)
    message(WARNING "TOYGINE_TESTS_ENABLE_COVERAGE ignored: ${CMAKE_CXX_COMPILER_ID} has no gcov-compatible coverage")
  endif ()

  if (TOYGINE_TESTS_ENABLE_COVERAGE AND TOYGINE_COMPILER_SUPPORT_COVERAGE)
    # include the cmake code coverage module
    include(CodeCoverage)

    # Update the default compiler flags to enable coverage
    # The directory-scope flags also reach the link line, which is what pulls in the gcov runtime under Clang
    append_coverage_compiler_flags()
    append_coverage_compiler_flags_to_target(${TOYGINE_LIBRARY_NAME})
    append_coverage_compiler_flags_to_target(${TOYGINE_LIBRARY_NAME}-units)

    # exclude tests, the build tree (fetched dependencies live there) and system headers from coverage analysis
    # lcov 2.x fails on a pattern that matches nothing, so every entry here must name a directory that exists
    set(COVERAGE_EXCLUDES "${CMAKE_SOURCE_DIR}/tests/*" "${CMAKE_BINARY_DIR}/*" "/usr/*")

    # cmake code coverage module target
    # ctest runs from the build tree, where CMakePresets.json is not visible, so it takes no --preset argument
    setup_target_for_coverage_lcov(NAME unit_tests_coverage EXECUTABLE ctest
                                   # tests run serially: parallel cases of one binary race over the same .gcda files
                                   EXECUTABLE_ARGS --output-on-failure --no-tests=error
                                   DEPENDENCIES ${TOYGINE_LIBRARY_NAME}-units ${TOYGINE_LIBRARY_NAME}-units-builtin
                                   LCOV_ARGS "--ignore-errors" "inconsistent")

  endif ()
endif ()
