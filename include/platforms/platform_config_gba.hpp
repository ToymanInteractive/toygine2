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
  \file   platform_config_gba.hpp
  \brief  Values of the platform constants for the Game Boy Advance target.

  Supplies what \ref toy::platform reads on this target; the dispatcher platforms/platform_config.hpp selects this
  file by the target toolchain's macro.

  \note Included by platforms/platform_config.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_PLATFORMS_PLATFORM_CONFIG_GBA_HPP_
#define INCLUDE_PLATFORMS_PLATFORM_CONFIG_GBA_HPP_

namespace toy::platform::detail {

// GCC for Thumb on the ARM7TDMI copies objects up to 44 bytes inline and calls memcpy() above.
inline constexpr size_t inlineCopyMaxBytes = 44;

// Skips the flag per call site to save ROM and RAM, so Action::IgnoreAlways acts as Action::Ignore.
inline constexpr bool assertIgnoreAlways = false;

// The Timer 2+3 cascade ticks at the full 16.777216 MHz, so one steady clock tick is one cascade tick.
inline constexpr int64_t steadyClockPeriodDenominator = 16'777'216;

// The devkitPro console font covers ASCII only, so non-ASCII text renders as garbage tiles.
inline constexpr bool consoleSupportsUtf8 = false;

} // namespace toy::platform::detail

#ifdef TOYGINE_DEBUG

// GCC emits an undefined instruction, and the BIOS exception vector stops execution at the failing check.
#define TOYGINE_DEBUG_BREAK() __builtin_trap()

#endif // TOYGINE_DEBUG

#define IMPLEMENT_CONSOLE_APP(appClassName)                                                                            \
                                                                                                                       \
  int main() {                                                                                                         \
    appClassName app(toy::application::Version{.major    = APP_VERSION_MAJOR,                                          \
                                               .minor    = APP_VERSION_MINOR,                                          \
                                               .patch    = APP_VERSION_PATCH,                                          \
                                               .revision = APP_VERSION_REVISION});                                     \
                                                                                                                       \
    static_cast<void>(app.run(0, nullptr));                                                                            \
                                                                                                                       \
    toy::application::ConsoleApplication::idleSystemConsole();                                                         \
  }

#endif // INCLUDE_PLATFORMS_PLATFORM_CONFIG_GBA_HPP_
