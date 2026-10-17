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

# MSVC Compiler Options
# https://learn.microsoft.com/en-nz/cpp/build/reference/compiler-options-listed-by-category
# MSVC Linker Options
# https://learn.microsoft.com/en-nz/cpp/build/reference/linker-options

# XCode 26.6 contains AppleClang version: 21.0.0.21000101
# Based on https://en.wikipedia.org/wiki/Xcode#Toolchain_versions
# Clang 21.1.0 documentation.     https://llvm.github.io/www-releases/21.1.0/tools/clang/docs/UsersManual.html#options
# Clang 21.1.0 diagnostic flags.  https://llvm.github.io/www-releases/21.1.0/tools/clang/docs/DiagnosticsReference.html
# last option is -Wbool-operation

# DevKitPro based on GCC version: 16.1
# GCC 16.1.0 documentation.   https://gcc.gnu.org/onlinedocs/gcc-16.1.0/gcc/index.html
# GCC 16.1.0 option summary.  https://gcc.gnu.org/onlinedocs/gcc-16.1.0/gcc/Option-Summary.html

if (TOYGINE_TARGET_PLATFORM STREQUAL "Windows Desktop")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Linux Desktop")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "macOS Desktop")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Sega MD")

  if (NOT CLOWNMDSDK_FOUND)
    message(FATAL_ERROR "ClownMDSDK not found. Install ClownMDSDK and ensure CLOWNMDSDK is set.")
  endif ()

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GBA")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

  if (NOT DEVKITPRO_GBA_FOUND)
    message(FATAL_ERROR "Nintendo GBA support not found. Install the devkitPro libgba package.")
  endif ()

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo DS")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

  if (NOT DEVKITPRO_NDS_FOUND)
    message(FATAL_ERROR "Nintendo DS support not found. Install the devkitPro libnds package.")
  endif ()

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo 3DS")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

  if (NOT DEVKITPRO_3DS_FOUND)
    message(FATAL_ERROR "Nintendo 3DS support not found. Install the devkitPro libctru package.")
  endif ()

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Switch")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

  if (NOT DEVKITPRO_SWITCH_FOUND)
    message(FATAL_ERROR "Nintendo Switch support not found. Install the devkitPro libnx package.")
  endif ()

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo 64")

  # No toolchain file is wired up for this target yet, so configuring would silently fall back to the host compiler.
  message(FATAL_ERROR "Nintendo 64 toolchain is not integrated yet. The n64-* presets cannot be configured.")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo GameCube")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

  if (NOT DEVKITPRO_GAMECUBE_FOUND)
    message(FATAL_ERROR "Nintendo GameCube support not found. Install the devkitPro libogc package.")
  endif ()

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Wii")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

  if (NOT DEVKITPRO_WII_FOUND)
    message(FATAL_ERROR "Nintendo Wii support not found. Install the devkitPro libogc package.")
  endif ()

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Wii U")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

  if (NOT DEVKITPRO_WII_U_FOUND)
    message(FATAL_ERROR "Nintendo Wii U support not found. Install the devkitPro wut package.")
  endif ()

else ()

  message(FATAL_ERROR "Unsupported platform: ${TOYGINE_TARGET_PLATFORM}")

endif ()

if (CMAKE_GENERATOR STREQUAL "Xcode")
  message(STATUS "Compiler: Xcode, version: " ${XCODE_VERSION})
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
  CMAKE_OBJC_FLAGS
  CMAKE_OBJCXX_FLAGS
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

option(TOYGINE_ENABLE_WARNINGS          "Build engine targets with the engine warning set and warnings as errors" ${PROJECT_IS_TOP_LEVEL})
option(TOYGINE_ENABLE_ABI_OPTIMIZATIONS "Build opted-in targets with optimizations that change the ABI" ${PROJECT_IS_TOP_LEVEL})

add_library(toygine-compile-defines INTERFACE)
add_library(toygine::compile::defines ALIAS toygine-compile-defines)
target_compile_definitions(toygine-compile-defines INTERFACE $<$<CONFIG:Debug,RelWithDebInfo>:TOYGINE_DEBUG>)

# Reaches engine-owned targets that link it PRIVATE; empty when TOYGINE_ENABLE_WARNINGS is OFF
add_library(toygine-compile-warnings INTERFACE)
add_library(toygine::compile::warnings ALIAS toygine-compile-warnings)
if (TOYGINE_ENABLE_WARNINGS)
  target_compile_options(toygine-compile-warnings INTERFACE
    "$<$<COMPILE_LANG_AND_ID:C,Clang,AppleClang,GNU>:-Wall;-Wpedantic;-Werror>"
    "$<$<COMPILE_LANG_AND_ID:C,MSVC>:/W4;/WX>"
    "$<$<COMPILE_LANG_AND_ID:CXX,Clang,AppleClang,GNU>:-Wall;-Wpedantic;-Werror>"
    "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/W4;/permissive-;/WX>"
    "$<$<COMPILE_LANG_AND_ID:OBJC,Clang,AppleClang,GNU>:-Wall;-Wpedantic;-Werror>"
    "$<$<COMPILE_LANG_AND_ID:OBJCXX,Clang,AppleClang,GNU>:-Wall;-Wpedantic;-Werror>")
endif ()

# Reaches every target created in the engine source tree, third-party ones included
add_compile_options(
  "$<$<AND:$<CONFIG:RelWithDebInfo,Release>,$<COMPILE_LANG_AND_ID:C,Clang,AppleClang,GNU>>:-O3>"
  "$<$<AND:$<CONFIG:RelWithDebInfo,Release>,$<COMPILE_LANG_AND_ID:C,MSVC>>:/O2;/Ot;/Ob3>"
  "$<$<AND:$<CONFIG:RelWithDebInfo,Release>,$<COMPILE_LANG_AND_ID:CXX,Clang,AppleClang,GNU>>:-O3>"
  "$<$<AND:$<CONFIG:RelWithDebInfo,Release>,$<COMPILE_LANG_AND_ID:CXX,MSVC>>:/O2;/Ot;/Ob3>"
  "$<$<AND:$<CONFIG:RelWithDebInfo,Release>,$<COMPILE_LANG_AND_ID:OBJC,Clang,AppleClang,GNU>>:-O3>"
  "$<$<AND:$<CONFIG:RelWithDebInfo,Release>,$<COMPILE_LANG_AND_ID:OBJCXX,Clang,AppleClang,GNU>>:-O3>")

# Reaches every target linking toygine and opted-in targets; empty when TOYGINE_ENABLE_ABI_OPTIMIZATIONS is OFF
add_library(toygine-compile-abi-optimizations INTERFACE)
add_library(toygine::compile::abi-optimizations ALIAS toygine-compile-abi-optimizations)
if (TOYGINE_ENABLE_ABI_OPTIMIZATIONS)
  target_compile_options(toygine-compile-abi-optimizations INTERFACE
    "$<$<CONFIG:RelWithDebInfo,Release>:$<$<COMPILE_LANG_AND_ID:C,Clang,AppleClang,GNU>:>>"
    "$<$<CONFIG:RelWithDebInfo,Release>:$<$<COMPILE_LANG_AND_ID:C,MSVC>:>>"
    "$<$<CONFIG:RelWithDebInfo,Release>:$<$<COMPILE_LANG_AND_ID:CXX,Clang,AppleClang,GNU>:>>"
    "$<$<CONFIG:RelWithDebInfo,Release>:$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:>>"
    "$<$<CONFIG:RelWithDebInfo,Release>:$<$<COMPILE_LANG_AND_ID:OBJC,Clang,AppleClang,GNU>:>>"
    "$<$<CONFIG:RelWithDebInfo,Release>:$<$<COMPILE_LANG_AND_ID:OBJCXX,Clang,AppleClang,GNU>:>>")
endif ()

if (ToyGine2_VERSION_MAJOR)
  add_compile_definitions(TOYGINE_VERSION_MAJOR=${ToyGine2_VERSION_MAJOR})
endif ()

if (ToyGine2_VERSION_MINOR)
  add_compile_definitions(TOYGINE_VERSION_MINOR=${ToyGine2_VERSION_MINOR})
endif ()

if (ToyGine2_VERSION_PATCH)
  add_compile_definitions(TOYGINE_VERSION_PATCH=${ToyGine2_VERSION_PATCH})
endif ()
