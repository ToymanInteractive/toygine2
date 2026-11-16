//
// Copyright (c) 2026 Toyman Interactive
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and / or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
/*!
  \file   platform_config_md.hpp
  \brief  Values of the platform constants for the Sega Mega Drive target.

  Supplies what \ref toy::platform reads on this target; the dispatcher platforms/platform_config.hpp selects this
  file by the target toolchain's macro.

  \note Included by platforms/platform_config.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_PLATFORMS_PLATFORM_CONFIG_MD_HPP_
#define INCLUDE_PLATFORMS_PLATFORM_CONFIG_MD_HPP_

#include <clownmdsdk.h>

namespace toy::platform::detail {

// GCC for the 68000 copies objects up to 52 bytes inline and calls memcpy() above.
inline constexpr size_t inlineCopyMaxBytes = 52;

// Skips the flag per call site to save ROM and RAM, so Action::IgnoreAlways acts as Action::Ignore.
inline constexpr bool assertIgnoreAlways = false;

// The clock counts V-Int frames: 5 ticks per NTSC frame, 6 per PAL, a nominal 300 Hz both regions share.
inline constexpr int64_t steadyClockPeriodDenominator = 300;

// The clownmdsdk debug console font covers ASCII only, so non-ASCII text renders as garbage tiles.
inline constexpr bool consoleSupportsUtf8 = false;

// H40 draws 40 columns; NTSC shows 28 rows (V28), PAL 30 (V30), so the buffer keeps the maximum.
inline constexpr size_t consoleColumns  = 40;
inline constexpr size_t consoleNtscRows = 28;
inline constexpr size_t consolePalRows  = 28;

} // namespace toy::platform::detail

#ifdef TOYGINE_DEBUG

// GCC emits trap #7, and the CPU exception stops execution at the failing check.
#define TOYGINE_DEBUG_BREAK() __builtin_trap()

#endif // TOYGINE_DEBUG

/*!
  \brief Defines _EntryPoint() that builds a console application and runs it without command-line arguments.

  The application takes its version from APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_PATCH, and
  APP_VERSION_REVISION. After run() returns, the macro calls idleSystemConsole(), which never returns.
  The cartridge linker script sets the entry to _EntryPoint; no C runtime on this target calls main().

  \param appClassName Application class derived from \ref toy::application::ConsoleApplication and constructible from a
                      \ref toy::application::Version.
*/
#define IMPLEMENT_CONSOLE_APP(appClassName)                                                                            \
                                                                                                                       \
  void _EntryPoint() {                                                                                                 \
    appClassName app(toy::application::Version{.major    = APP_VERSION_MAJOR,                                          \
                                               .minor    = APP_VERSION_MINOR,                                          \
                                               .patch    = APP_VERSION_PATCH,                                          \
                                               .revision = APP_VERSION_REVISION});                                     \
                                                                                                                       \
    static_cast<void>(app.run(0, nullptr));                                                                            \
                                                                                                                       \
    toy::application::ConsoleApplication::idleSystemConsole();                                                         \
  }

#endif // INCLUDE_PLATFORMS_PLATFORM_CONFIG_MD_HPP_
