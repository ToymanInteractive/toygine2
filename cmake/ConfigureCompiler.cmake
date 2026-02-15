#-------------------------------------------------------------------------------------------
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
#-------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.31.0 FATAL_ERROR)

if (TOYGINE_TARGET_PLATFORM STREQUAL "Windows Desktop")

  if (MSVC)
    message(STATUS "Compiler: MSVC, version: " ${MSVC_VERSION})

    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:Debug>")

    # Minimum CPU Architecture select based on https://store.steampowered.com/hwsurvey/

    set(CMAKE_C_FLAGS   "/nologo /Wall /WX /wd4007 /wd4464 /wd4514 /wd4710 /wd4711 /wd4820 /wd5045 /DWIN32 /D_WINDOWS /DUNICODE /D_UNICODE /EHsc /Zc:wchar_t /Zc:forScope /Zc:inline /Zc:rvalueCast /GR- /permissive-")
    set(CMAKE_CXX_FLAGS "/nologo /Wall /WX /wd4007 /wd4464 /wd4514 /wd4710 /wd4711 /wd4820 /wd5045 /DWIN32 /D_WINDOWS /DUNICODE /D_UNICODE /EHsc /Zc:wchar_t /Zc:forScope /Zc:inline /Zc:rvalueCast /GR- /permissive-")

    if (CMAKE_SIZEOF_VOID_P MATCHES "8")

      set(CMAKE_C_FLAGS_DEBUG            "/ZI /diagnostics:caret   /sdl                     /fsanitize=fuzzer /Od /Ob0 /Oi-     /Oy-         /D_DEBUG /GF- /RTCc /D_ALLOW_RTCc_IN_STL /RTCsu /MTd /GS  /guard:cf- /Gy  /Qpar- /arch:SSE2 /fp:strict  /fp:except            /Gr /MP")
      set(CMAKE_CXX_FLAGS_DEBUG          "/ZI /diagnostics:caret   /sdl                     /fsanitize=fuzzer /Od /Ob0 /Oi-     /Oy-         /D_DEBUG /GF- /RTCc /D_ALLOW_RTCc_IN_STL /RTCsu /MTd /GS  /guard:cf- /Gy  /Qpar- /arch:SSE2 /fp:strict  /fp:except            /Gd /MP")

      set(CMAKE_C_FLAGS_RELWITHDEBINFO   "/Zi /diagnostics:column  /sdl  /fsanitize=address /fsanitize=fuzzer /Ox /Ob3 /Oi  /Ot /Oy- /GT /GL /D_DEBUG /GF                                    /MTd /GS  /guard:cf  /Gy- /Qpar- /arch:SSE2 /fp:precise /fp:except            /Gr")
      set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Zi /diagnostics:column  /sdl  /fsanitize=address /fsanitize=fuzzer /Ox /Ob3 /Oi  /Ot /Oy- /GT /GL /D_DEBUG /GF                                    /MTd /GS  /guard:cf  /Gy- /Qpar- /arch:SSE2 /fp:precise /fp:except            /Gr")

      set(CMAKE_C_FLAGS_RELEASE          "    /diagnostics:classic /sdl-                                      /Ox /Ob3 /Oi  /Ot /Oy  /GT /GL /DNDEBUG /GF                                    /MT  /GS- /guard:cf- /Gy- /Qpar  /arch:SSE2 /fp:fast    /fp:except-           /Gr")
      set(CMAKE_CXX_FLAGS_RELEASE        "    /diagnostics:classic /sdl-                                      /Ox /Ob3 /Oi  /Ot /Oy  /GT /GL /DNDEBUG /GF                                    /MT  /GS- /guard:cf- /Gy- /Qpar  /arch:SSE2 /fp:fast    /fp:except-           /Gr")

    else ()

      set(CMAKE_C_FLAGS_DEBUG            "/ZI /diagnostics:caret   /sdl                     /fsanitize=fuzzer /Od /Ob0 /Oi-     /Oy-         /D_DEBUG /GF- /RTCc /D_ALLOW_RTCc_IN_STL /RTCsu /MTd /GS  /guard:cf- /Gy  /Qpar- /arch:IA32 /fp:strict  /fp:except  /hotpatch /Gd /MP")
      set(CMAKE_CXX_FLAGS_DEBUG          "/ZI /diagnostics:caret   /sdl                     /fsanitize=fuzzer /Od /Ob0 /Oi-     /Oy-         /D_DEBUG /GF- /RTCc /D_ALLOW_RTCc_IN_STL /RTCsu /MTd /GS  /guard:cf- /Gy  /Qpar- /arch:IA32 /fp:strict  /fp:except  /hotpatch /Gd /MP")

      set(CMAKE_C_FLAGS_RELWITHDEBINFO   "/Zi /diagnostics:column  /sdl  /fsanitize=address /fsanitize=fuzzer /Ox /Ob3 /Oi  /Ot /Oy- /GT /GL /D_DEBUG /GF                                    /MTd /GS  /guard:cf  /Gy- /Qpar- /arch:SSE2 /fp:precise /fp:except            /Gr")
      set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Zi /diagnostics:column  /sdl  /fsanitize=address /fsanitize=fuzzer /Ox /Ob3 /Oi  /Ot /Oy- /GT /GL /D_DEBUG /GF                                    /MTd /GS  /guard:cf  /Gy- /Qpar- /arch:SSE2 /fp:precise /fp:except            /Gr")

      set(CMAKE_C_FLAGS_RELEASE          "    /diagnostics:classic /sdl-                                      /Ox /Ob3 /Oi  /Ot /Oy  /GT /GL /DNDEBUG /GF                                    /MT  /GS- /guard:cf- /Gy- /Qpar  /arch:SSE2 /fp:fast    /fp:except-           /Gr")
      set(CMAKE_CXX_FLAGS_RELEASE        "    /diagnostics:classic /sdl-                                      /Ox /Ob3 /Oi  /Ot /Oy  /GT /GL /DNDEBUG /GF                                    /MT  /GS- /guard:cf- /Gy- /Qpar  /arch:SSE2 /fp:fast    /fp:except-           /Gr")

    endif ()

    set(CMAKE_STATIC_LINKER_FLAGS "/WX")
    set(CMAKE_EXE_LINKER_FLAGS    "/WX /MANIFEST /MANIFESTUAC:\"/level='asInvoker' /uiAccess='false'\" /ALLOWISOLATION /LARGEADDRESSAWARE /SAFESEH:NO")

    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG           "                /LTCG:OFF")
    set(CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO  "                /LTCG")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE         "                /LTCG")

    set(CMAKE_EXE_LINKER_FLAGS_DEBUG              "/INCREMENTAL:NO /LTCG:OFF /DEBUG:FULL /ASSEMBLYDEBUG")
    set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO     "/INCREMENTAL:NO /LTCG     /DEBUG:FULL /ASSEMBLYDEBUG")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE            "/INCREMENTAL:NO /LTCG     /DEBUG:NONE /ASSEMBLYDEBUG:DISABLE")
  endif ()

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Linux Desktop")

  message(STATUS "${CMAKE_CXX_COMPILER_ID} version: ${CMAKE_CXX_COMPILER_VERSION}")

  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 13.3)
    message(FATAL_ERROR "GCC >= 13.3 required")
  endif ()

# Option Summary  https://gcc.gnu.org/onlinedocs/gcc-13.3.0/gcc/C_002b_002b-Dialect-Options.html
#                 https://gcc.gnu.org/onlinedocs/gcc-13.3.0/gcc/C_002b_002b-Dialect-Options.html#index-fno-module-lazy
# Option Summary  https://gcc.gnu.org/onlinedocs/gcc-14.2.0/gcc/C_002b_002b-Dialect-Options.html

  set(CMAKE_C_FLAGS   "-std=c17   -foffload=default -fopenmp-simd -fstrict-flex-arrays=2")
  set(CMAKE_CXX_FLAGS "-std=c++23 -foffload=default -fopenmp-simd -fstrict-flex-arrays=2")

  set(CMAKE_C_FLAGS_DEBUG            "    -g -D_DEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG          "    -g -D_DEBUG")

  set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O3 -g -D_DEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -D_DEBUG")

  set(CMAKE_C_FLAGS_RELEASE          "-O3    -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE        "-O3    -DNDEBUG")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "macOS Desktop")

  if (CMAKE_GENERATOR STREQUAL "Xcode")
    message(STATUS "Compiler: Xcode, version: " ${XCODE_VERSION})
  endif ()

  message(STATUS "${CMAKE_CXX_COMPILER_ID} version: ${CMAKE_CXX_COMPILER_VERSION}")

  # XCode 16.4 contains AppleClang version: 17.0.0.17000013
  # Clang 17.0.1 documentation. https://releases.llvm.org/17.0.1/tools/clang/docs/UsersManual.html
  # Clang 17.0.1 diagnostic flags. https://releases.llvm.org/17.0.1/tools/clang/docs/DiagnosticsReference.html

  set(CMAKE_C_FLAGS   "-Werror -Weverything -pedantic-errors -Wno-missing-prototypes                                                                -Wno-missing-include-dirs -Wno-padded -Wno-poison-system-directories -fshow-column -fshow-source-location -fcaret-diagnostics -fcolor-diagnostics -fdiagnostics-format=clang -fdiagnostics-show-option -fdiagnostics-show-category=id -fdiagnostics-fixit-info -fdiagnostics-print-source-range-info -fdiagnostics-parseable-fixits -fno-rounding-math -fexcess-precision=standard")
  set(CMAKE_CXX_FLAGS "-Werror -Weverything -pedantic-errors -Wno-missing-prototypes -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-c++20-compat -Wno-missing-include-dirs -Wno-padded -Wno-poison-system-directories -fshow-column -fshow-source-location -fcaret-diagnostics -fcolor-diagnostics -fdiagnostics-format=clang -fdiagnostics-show-option -fdiagnostics-show-category=id -fdiagnostics-fixit-info -fdiagnostics-print-source-range-info -fdiagnostics-parseable-fixits -fno-rounding-math -fexcess-precision=standard")

  set(CMAKE_C_FLAGS_DEBUG            "    -g -D_DEBUG -fno-fast-math -fstrict-float-cast-overflow    -fmath-errno -fno-unsafe-math-optimizations -ffp-model=precise -ffp-exception-behavior=strict -fprotect-parens                                                                            -fsanitize=address -fsanitize=alignment -fsanitize=bool -fsanitize=builtin -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=function -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=objc-cast                        -fsanitize=pointer-overflow -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=unsigned-shift-base -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -fsanitize=integer -fsanitize=nullability")
  set(CMAKE_CXX_FLAGS_DEBUG          "    -g -D_DEBUG -fno-fast-math -fstrict-float-cast-overflow    -fmath-errno -fno-unsafe-math-optimizations -ffp-model=precise -ffp-exception-behavior=strict -fprotect-parens                                                                            -fsanitize=address -fsanitize=alignment -fsanitize=bool -fsanitize=builtin -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=function -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=objc-cast                        -fsanitize=pointer-overflow -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=unsigned-shift-base -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -fsanitize=integer -fsanitize=nullability")

  set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O3 -g -D_DEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -D_DEBUG")

# set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O3 -g -D_DEBUG -ffast-math    -fno-strict-float-cast-overflow              -funsafe-math-optimizations    -ffp-model=fast    -ffp-exception-behavior=strict -fno-protect-parens -fstrict-vtable-pointers -fwhole-program-vtables -fsplit-lto-unit -flto -fsanitize=address -fsanitize=alignment -fsanitize=bool -fsanitize=builtin -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=function -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=objc-cast -fsanitize=object-size -fsanitize=pointer-overflow -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=unsigned-shift-base -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -fsanitize=integer -fsanitize=nullability")
# set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -D_DEBUG -ffast-math    -fno-strict-float-cast-overflow              -funsafe-math-optimizations    -ffp-model=fast    -ffp-exception-behavior=strict -fno-protect-parens -fstrict-vtable-pointers -fwhole-program-vtables -fsplit-lto-unit -flto -fsanitize=address -fsanitize=alignment -fsanitize=bool -fsanitize=builtin -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=function -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=objc-cast -fsanitize=object-size -fsanitize=pointer-overflow -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=unsigned-shift-base -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -fsanitize=integer -fsanitize=nullability")

  set(CMAKE_C_FLAGS_RELEASE          "-O3    -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE        "-O3    -DNDEBUG")

# set(CMAKE_C_FLAGS_RELEASE          "-O3    -DNDEBUG -ffast-math    -fno-strict-float-cast-overflow              -funsafe-math-optimizations    -ffp-model=fast    -ffp-exception-behavior=ignore -fno-protect-parens -fstrict-vtable-pointers -fwhole-program-vtables -fsplit-lto-unit -flto")
# set(CMAKE_CXX_FLAGS_RELEASE        "-O3    -DNDEBUG -ffast-math    -fno-strict-float-cast-overflow              -funsafe-math-optimizations    -ffp-model=fast    -ffp-exception-behavior=ignore -fno-protect-parens -fstrict-vtable-pointers -fwhole-program-vtables -fsplit-lto-unit -flto")

  set(CMAKE_STATIC_LINKER_FLAGS "")
  set(CMAKE_EXE_LINKER_FLAGS    "")

  set(CMAKE_STATIC_LINKER_FLAGS_DEBUG           "")
  set(CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO  "")
  set(CMAKE_STATIC_LINKER_FLAGS_RELEASE         "")

  set(CMAKE_EXE_LINKER_FLAGS_DEBUG              "-fsanitize=address -fsanitize=alignment -fsanitize=bool -fsanitize=builtin -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=function -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=objc-cast                        -fsanitize=pointer-overflow -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=unsigned-shift-base -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -fsanitize=integer -fsanitize=nullability")
  set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO     "-fsanitize=address -fsanitize=alignment -fsanitize=bool -fsanitize=builtin -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=function -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=objc-cast -fsanitize=object-size -fsanitize=pointer-overflow -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=unsigned-shift-base -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -fsanitize=integer -fsanitize=nullability")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE            "")

# Index                           https://releases.llvm.org/17.0.1/tools/clang/docs/index.html
# Command line argument reference https://releases.llvm.org/17.0.1/tools/clang/docs/ClangCommandLineReference.html
# Diagnostic flags                https://releases.llvm.org/17.0.1/tools/clang/docs/DiagnosticsReference.html
# Compiler User’s Manual          https://releases.llvm.org/17.0.1/tools/clang/docs/UsersManual.html
# разобраться с sanitize

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Game Boy Advance")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

  list(APPEND CMAKE_MODULE_PATH "${DEVKITPRO_ROOT}/cmake")

  include(GBA)

  message(STATUS "${CMAKE_CXX_COMPILER_ID} version: ${CMAKE_CXX_COMPILER_VERSION}")

  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 15.2)
    message(FATAL_ERROR "GCC >= 15.2 required")
  endif ()

# Option Summary  https://gcc.gnu.org/onlinedocs/gcc-15.2.0/gcc/C_002b_002b-Dialect-Options.html

  set(CMAKE_C_FLAGS   "-std=c17   -fstrict-flex-arrays=2")
  set(CMAKE_CXX_FLAGS "-std=c++23 -fstrict-flex-arrays=2")

  set(CMAKE_C_FLAGS_DEBUG            "    -g -D_DEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG          "    -g -D_DEBUG")

  set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O3 -g -D_DEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -D_DEBUG")

  set(CMAKE_C_FLAGS_RELEASE          "-O3    -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE        "-O3    -DNDEBUG")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo DS")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

  list(APPEND CMAKE_MODULE_PATH "${DEVKITPRO_ROOT}/cmake")

  include(NDS)

  message(STATUS "${CMAKE_CXX_COMPILER_ID} version: ${CMAKE_CXX_COMPILER_VERSION}")

  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 15.2)
    message(FATAL_ERROR "GCC >= 15.2 required")
  endif ()

# Option Summary  https://gcc.gnu.org/onlinedocs/gcc-15.2.0/gcc/C_002b_002b-Dialect-Options.html

  set(CMAKE_C_FLAGS   "-std=c17   -fstrict-flex-arrays=2")
  set(CMAKE_CXX_FLAGS "-std=c++23 -fstrict-flex-arrays=2")

  set(CMAKE_C_FLAGS_DEBUG            "    -g -D_DEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG          "    -g -D_DEBUG")

  set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O3 -g -D_DEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -D_DEBUG")

  set(CMAKE_C_FLAGS_RELEASE          "-O3    -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE        "-O3    -DNDEBUG")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo 3DS")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

  list(APPEND CMAKE_MODULE_PATH "${DEVKITPRO_ROOT}/cmake")

  include(3DS)

  message(STATUS "${CMAKE_CXX_COMPILER_ID} version: ${CMAKE_CXX_COMPILER_VERSION}")

  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 15.2)
    message(FATAL_ERROR "GCC >= 15.2 required")
  endif ()

# Option Summary  https://gcc.gnu.org/onlinedocs/gcc-15.2.0/gcc/C_002b_002b-Dialect-Options.html

  set(CMAKE_C_FLAGS   "-std=c17   -fstrict-flex-arrays=2")
  set(CMAKE_CXX_FLAGS "-std=c++23 -fstrict-flex-arrays=2")

  set(CMAKE_C_FLAGS_DEBUG            "    -g -D_DEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG          "    -g -D_DEBUG")

  set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O3 -g -D_DEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -D_DEBUG")

  set(CMAKE_C_FLAGS_RELEASE          "-O3    -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE        "-O3    -DNDEBUG")

elseif (TOYGINE_TARGET_PLATFORM STREQUAL "Nintendo Switch")

  if (NOT DEVKITPRO_FOUND)
    message(FATAL_ERROR "devkitPro not found. Install devkitPro and ensure DEVKITPRO is set.")
  endif ()

  list(APPEND CMAKE_MODULE_PATH "${DEVKITPRO_ROOT}/cmake")

  include(Switch)

  message(STATUS "${CMAKE_CXX_COMPILER_ID} version: ${CMAKE_CXX_COMPILER_VERSION}")

  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 15.2)
    message(FATAL_ERROR "GCC >= 15.2 required")
  endif ()

# Option Summary  https://gcc.gnu.org/onlinedocs/gcc-15.2.0/gcc/C_002b_002b-Dialect-Options.html

  set(CMAKE_C_FLAGS   "-std=c17   -fstrict-flex-arrays=2")
  set(CMAKE_CXX_FLAGS "-std=c++23 -fstrict-flex-arrays=2")

  set(CMAKE_C_FLAGS_DEBUG            "    -g -D_DEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG          "    -g -D_DEBUG")

  set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O3 -g -D_DEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -D_DEBUG")

  set(CMAKE_C_FLAGS_RELEASE          "-O3    -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE        "-O3    -DNDEBUG")

else ()

  message(FATAL_ERROR "Unsupported platform: ${TOYGINE_TARGET_PLATFORM}")

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

message(STATUS "CMAKE_C_FLAGS=${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_C_FLAGS_DEBUG=${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "CMAKE_C_FLAGS_RELWITHDEBINFO=${CMAKE_C_FLAGS_RELWITHDEBINFO}")
message(STATUS "CMAKE_C_FLAGS_RELEASE=${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO=${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE=${CMAKE_CXX_FLAGS_RELEASE}")

message(STATUS "CMAKE_STATIC_LINKER_FLAGS=${CMAKE_STATIC_LINKER_FLAGS}")
message(STATUS "CMAKE_STATIC_LINKER_FLAGS_DEBUG=${CMAKE_STATIC_LINKER_FLAGS_DEBUG}")
message(STATUS "CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO=${CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO}")
message(STATUS "CMAKE_STATIC_LINKER_FLAGS_RELEASE=${CMAKE_STATIC_LINKER_FLAGS_RELEASE}")

message(STATUS "CMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS_DEBUG=${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO=${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS_RELEASE=${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

message(STATUS "PREPROCESSOR_DEFINITIONS=${PREPROCESSOR_DEFINITIONS}")
