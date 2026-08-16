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

# Minimum CPU Architecture select based on https://store.steampowered.com/hwsurvey/

if (TOYGINE_TARGET_PLATFORM STREQUAL "Windows Desktop")

  if (MSVC)

  # MSVC Compiler Options
  # https://learn.microsoft.com/en-nz/cpp/build/reference/compiler-options-listed-by-category?view=msvc-170#optimization
  # last option is /Gd

  # MSVC Linker Options
  # https://learn.microsoft.com/en-nz/cpp/build/reference/linker-options?view=msvc-170

    set(CMAKE_C_FLAGS                  "/EHsc /GA")
    set(CMAKE_CXX_FLAGS                "/EHsc /GA")

    set(CMAKE_C_FLAGS_DEBUG            "/Od /Ob0 /Oi-     /Oy- /fp:strict /fp:except  /Gd")
    set(CMAKE_CXX_FLAGS_DEBUG          "/Od /Ob0 /Oi-     /Oy- /fp:strict /fp:except  /Gd")

    set(CMAKE_C_FLAGS_RELWITHDEBINFO   "/Ox /Ob3 /Oi  /Ot /Oy- /fp:fast   /fp:except- /Gr")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Ox /Ob3 /Oi  /Ot /Oy- /fp:fast   /fp:except- /Gr")

    set(CMAKE_C_FLAGS_RELEASE          "/Ox /Ob3 /Oi  /Ot /Oy  /fp:fast   /fp:except- /Gr")
    set(CMAKE_CXX_FLAGS_RELEASE        "/Ox /Ob3 /Oi  /Ot /Oy  /fp:fast   /fp:except- /Gr")


    set(CMAKE_STATIC_LINKER_FLAGS                 "")
    set(CMAKE_EXE_LINKER_FLAGS                    "")

    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG           "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG              "/DEBUG:FULL /INCREMENTAL")

    set(CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO  "")
    set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO     "/DEBUG:FULL /INCREMENTAL:NO")

    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE         "")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE            "/DEBUG:NONE /INCREMENTAL:NO")

    if (CMAKE_VS_PLATFORM_NAME STREQUAL "x64")

      string(APPEND CMAKE_C_FLAGS   " /arch:SSE2 /fpcvt:IA")
      string(APPEND CMAKE_CXX_FLAGS " /arch:SSE2 /fpcvt:IA")

      string(APPEND CMAKE_C_FLAGS_RELWITHDEBINFO          " /dynamicdeopt:sync /favor:blend")
      string(APPEND CMAKE_CXX_FLAGS_RELWITHDEBINFO        " /dynamicdeopt:sync /favor:blend")

      string(APPEND CMAKE_C_FLAGS_RELEASE                 "                    /favor:blend")
      string(APPEND CMAKE_CXX_FLAGS_RELEASE               "                    /favor:blend")

      string(APPEND CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO " /DYNAMICDEOPT:SYNC")

    elseif (CMAKE_VS_PLATFORM_NAME STREQUAL "Win32")

      string(APPEND CMAKE_C_FLAGS   " /arch:SSE2 /fpcvt:IA")
      string(APPEND CMAKE_CXX_FLAGS " /arch:SSE2 /fpcvt:IA")

      string(APPEND CMAKE_C_FLAGS_RELWITHDEBINFO          "                    /favor:blend")
      string(APPEND CMAKE_CXX_FLAGS_RELWITHDEBINFO        "                    /favor:blend")

      string(APPEND CMAKE_C_FLAGS_RELEASE                 "                    /favor:blend")
      string(APPEND CMAKE_CXX_FLAGS_RELEASE               "                    /favor:blend")

    elseif (CMAKE_VS_PLATFORM_NAME STREQUAL "ARM64")

      string(APPEND CMAKE_C_FLAGS   " /forceInterlockedFunctions")
      string(APPEND CMAKE_CXX_FLAGS " /forceInterlockedFunctions")

    endif ()

  endif ()

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Linux Desktop")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "macOS Desktop")

  if (CMAKE_GENERATOR STREQUAL "Xcode")
    message(STATUS "Compiler: Xcode, version: " ${XCODE_VERSION})
  endif ()

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Sega MD")

  if (NOT CLOWNMDSDK_FOUND)
    message(FATAL_ERROR "ClownMDSDK not found. Install ClownMDSDK and ensure CLOWNMDSDK is set.")
  endif ()

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GBA")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo DS")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo 3DS")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Switch")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo 64")

  # No toolchain file is wired up for this target yet, so configuring would silently fall back to the host compiler.
  message(FATAL_ERROR "Nintendo 64 toolchain is not integrated yet. The n64-* presets cannot be configured.")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GameCube")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Wii")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

else ()

  message(FATAL_ERROR "Unsupported platform: ${TOYGINE_TARGET_PLATFORM}")

endif ()

message(STATUS "${CMAKE_CXX_COMPILER_ID} version: ${CMAKE_CXX_COMPILER_VERSION}")

# Report the compiler and linker option variables, per configuration
if (CMAKE_CONFIGURATION_TYPES)
  set(REPORTED_CONFIGS ${CMAKE_CONFIGURATION_TYPES})
elseif (CMAKE_BUILD_TYPE)
  set(REPORTED_CONFIGS ${CMAKE_BUILD_TYPE})
else ()
  set(REPORTED_CONFIGS "")
endif ()

foreach (FLAGS_VARIABLE IN ITEMS
  CMAKE_C_FLAGS
  CMAKE_CXX_FLAGS
  CMAKE_STATIC_LINKER_FLAGS
  CMAKE_SHARED_LINKER_FLAGS
  CMAKE_MODULE_LINKER_FLAGS
  CMAKE_EXE_LINKER_FLAGS)

  message(STATUS "${FLAGS_VARIABLE}: ${${FLAGS_VARIABLE}}")

  foreach (REPORTED_CONFIG IN LISTS REPORTED_CONFIGS)
    string(TOUPPER ${REPORTED_CONFIG} REPORTED_CONFIG_UPPER)
    message(STATUS "${FLAGS_VARIABLE}_${REPORTED_CONFIG_UPPER}: ${${FLAGS_VARIABLE}_${REPORTED_CONFIG_UPPER}}")
  endforeach ()

endforeach ()

unset(REPORTED_CONFIG_UPPER)
unset(REPORTED_CONFIGS)

if (ToyGine2_VERSION_MAJOR)
  add_compile_definitions(TOYGINE_VERSION_MAJOR=${ToyGine2_VERSION_MAJOR})
endif ()

if (ToyGine2_VERSION_MINOR)
  add_compile_definitions(TOYGINE_VERSION_MINOR=${ToyGine2_VERSION_MINOR})
endif ()

if (ToyGine2_VERSION_PATCH)
  add_compile_definitions(TOYGINE_VERSION_PATCH=${ToyGine2_VERSION_PATCH})
endif ()
