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
  \file   platform_config.hpp
  \brief  Values of the platform constants for the Nintendo Switch target.

  Supplies what \ref toy::platform reads on this target; the build adds this directory to the include path of the active
  platform only.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_PLATFORMS_SWITCH_PLATFORM_CONFIG_HPP_
#define INCLUDE_PLATFORMS_SWITCH_PLATFORM_CONFIG_HPP_

namespace toy::platform::detail {

// Assumed equal to desktop: GCC for the Cortex-A57 inlines copies up to 200 bytes, cost not measured.
inline constexpr size_t inlineCopyMaxBytes = 128;

// Skips the flag per call site to save ROM and RAM, so Action::IgnoreAlways acts as Action::Ignore.
inline constexpr bool assertIgnoreAlways = false;

// This target has no counter backend: the stub clock reports nominal nanoseconds.
inline constexpr int64_t steadyClockPeriodDenominator = 1'000'000'000;

// The devkitPro console font covers ASCII only, so non-ASCII text renders as garbage tiles.
inline constexpr bool consoleSupportsUtf8 = false;

} // namespace toy::platform::detail

#ifdef TOYGINE_DEBUG

// GCC emits brk, and the exception stops execution at the failing check.
#define TOYGINE_DEBUG_BREAK() __builtin_trap()

#endif // TOYGINE_DEBUG

#define IMPLEMENT_CONSOLE_APP(appClassName)                                                                            \
                                                                                                                       \
  int main(int argc, char * argv[]) {                                                                                  \
    appClassName app(toy::application::Version{.major    = APP_VERSION_MAJOR,                                          \
                                               .minor    = APP_VERSION_MINOR,                                          \
                                               .patch    = APP_VERSION_PATCH,                                          \
                                               .revision = APP_VERSION_REVISION});                                     \
                                                                                                                       \
    static_cast<void>(app.run(static_cast<size_t>(toy::max(0, argc)), argv));                                          \
                                                                                                                       \
    toy::application::ConsoleApplication::idleSystemConsole();                                                         \
  }

#endif // INCLUDE_PLATFORMS_SWITCH_PLATFORM_CONFIG_HPP_
