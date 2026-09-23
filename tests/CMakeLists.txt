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
# Test runner selection
#-----------------------------------------------------------------------------------------------------------------------

# doctest needs a host that runs the binary and a hosted standard library, so the consoles have the built-in runner as
# their only choice; desktop picks either and defaults to doctest.
if (TOYGINE_TARGET_PLATFORM_IS_DESKTOP)
  set(TOYGINE_TESTS_RUNNER_DEFAULT "doctest")
else ()
  set(TOYGINE_TESTS_RUNNER_DEFAULT "builtin")
endif ()

set(TOYGINE_TESTS_RUNNER ${TOYGINE_TESTS_RUNNER_DEFAULT} CACHE STRING "Unit test runner: doctest or builtin")
set_property(CACHE TOYGINE_TESTS_RUNNER PROPERTY STRINGS doctest builtin)

if (NOT TOYGINE_TESTS_RUNNER MATCHES "^(doctest|builtin)$")
  message(FATAL_ERROR "Unsupported TOYGINE_TESTS_RUNNER: ${TOYGINE_TESTS_RUNNER}")
endif ()

if (NOT TOYGINE_TARGET_PLATFORM_IS_DESKTOP AND NOT TOYGINE_TESTS_RUNNER STREQUAL "builtin")
  message(FATAL_ERROR "TOYGINE_TESTS_RUNNER=${TOYGINE_TESTS_RUNNER} needs a desktop target; ${TOYGINE_TARGET_PLATFORM} runs the built-in runner")
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Tests translation units
#-----------------------------------------------------------------------------------------------------------------------

# Tests of engine modules. Written against the toy_test.hpp shim, so the same sources build under either runner.
file(GLOB_RECURSE TOYGINE_MODULE_TESTS_SRC CONFIGURE_DEPENDS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.test.cpp")
list(FILTER TOYGINE_MODULE_TESTS_SRC EXCLUDE REGEX "^runner/")

if (TOYGINE_TESTS_RUNNER STREQUAL "doctest")
  # Entry point of the doctest build: applies the command line and runs the registered cases.
  set(TOYGINE_TESTS_ENTRY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/ctest_main.cpp)
else ()
  # The built-in runner's platform layer (output, exit, entry point) is a per-target file, so it has no target branches.
  if (TOYGINE_TARGET_PLATFORM_IS_DESKTOP)
    set(TOYGINE_TESTS_ENTRY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/runner/platforms/desktop.cpp)
  else ()
    set(TOYGINE_TESTS_ENTRY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/runner/platforms/${TOYGINE_TARGET_PLATFORM_SLUG}.cpp)
  endif ()
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# The test binary
#-----------------------------------------------------------------------------------------------------------------------

if (TOYGINE_TESTS_RUNNER STREQUAL "doctest")
  include(FetchContent)

  FetchContent_Declare(doctest GIT_REPOSITORY https://github.com/doctest/doctest GIT_TAG v2.5.3 GIT_SHALLOW TRUE)
  FetchContent_MakeAvailable(doctest)

  list(APPEND CMAKE_MODULE_PATH ${doctest_SOURCE_DIR}/scripts/cmake)

  include(doctest)
endif ()

# Sega MD has no plain executable: the cartridge linker script and its startup stub are what give the binary an entry.
if (TOYGINE_TARGET_PLATFORM STREQUAL "Sega MD")
  add_cartridge_executable(${TOYGINE_LIBRARY_NAME}-units
      ${TOYGINE_TESTS_ENTRY_SRC} ${TOYGINE_MODULE_TESTS_SRC})
else ()
  add_executable(${TOYGINE_LIBRARY_NAME}-units
      ${TOYGINE_TESTS_ENTRY_SRC} ${TOYGINE_MODULE_TESTS_SRC})
endif ()

target_include_directories(${TOYGINE_LIBRARY_NAME}-units PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/runner)
target_link_libraries(${TOYGINE_LIBRARY_NAME}-units PRIVATE ${TOYGINE_LIBRARY_NAME})
set_target_properties(${TOYGINE_LIBRARY_NAME}-units PROPERTIES FOLDER "tests")

if (TOYGINE_TESTS_RUNNER STREQUAL "doctest")
  # The shim resolves its macros to doctest's; without the define it builds the registry the built-in runner walks.
  target_compile_definitions(${TOYGINE_LIBRARY_NAME}-units PRIVATE DOCTEST_CONFIG_SUPER_FAST_ASSERTS
                                                                  TOYGINE_TESTS_USE_DOCTEST)
  target_link_libraries(${TOYGINE_LIBRARY_NAME}-units PRIVATE doctest::doctest)

  # arm64 macOS kills an unsigned binary, and Xcode signs only after the post-build phases, so the linker signs instead.
  set_target_properties(${TOYGINE_LIBRARY_NAME}-units PROPERTIES XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED NO)

  doctest_discover_tests(${TOYGINE_LIBRARY_NAME}-units)

  include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/coverage.cmake)
else ()
  # These consoles cannot load the ELF, so each converts it to its loader's image format; Sega MD is built above.
  if (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GBA")
    gba_create_rom(${TOYGINE_LIBRARY_NAME}-units)
  elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo DS")
    nds_create_rom(${TOYGINE_LIBRARY_NAME}-units)
  elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo 3DS")
    ctr_create_3dsx(${TOYGINE_LIBRARY_NAME}-units)
  elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Switch")
    nx_create_nro(${TOYGINE_LIBRARY_NAME}-units)
  elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GameCube")
    ogc_create_dol(${TOYGINE_LIBRARY_NAME}-units)
  elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Wii")
    ogc_create_dol(${TOYGINE_LIBRARY_NAME}-units)
  elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Wii U")
    wut_create_rpx(${TOYGINE_LIBRARY_NAME}-units)
  endif ()

  # Registered with CTest wherever something can run it: the host on desktop, an emulator on Sega MD and Nintendo GBA.
  include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/builtin_runner_test.cmake)
endif ()

#-----------------------------------------------------------------------------------------------------------------------
# Tests of the built-in runner
#-----------------------------------------------------------------------------------------------------------------------

# They need doctest and a host that runs the binary, so no console builds them.
if (TOYGINE_TARGET_PLATFORM_IS_DESKTOP)
  add_subdirectory(runner/tests)
endif ()
