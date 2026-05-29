//
// Copyright (c) 2025-2026 Toyman Interactive
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
  \brief  Compile-time platform and CPU bindings for Nintendo GBA.

  Defines \ref toy::Platform and \ref toy::CpuArchitecture values (\c currentPlatform, \c currentCpuArchitecture) for
  GBA toolchains.

  \note Included by GBA platform translation units; not a public module header.
*/

#ifndef SRC_PLATFORMS_GBA_PLATFORM_CONFIG_HPP_
#define SRC_PLATFORMS_GBA_PLATFORM_CONFIG_HPP_

#include "../../../include/core/platform.hpp"

#if defined(__GBA__)

#include "../common/assertion_macro_gcc_clang.hpp"

//----------------------------------------------------------------------------------------------------------------------

namespace toy {

inline constexpr auto currentPlatform = Platform::GBA;

inline constexpr auto currentCpuArchitecture = CpuArchitecture::Arm32;

namespace chrono {

/// Denominator of the \ref toy::chrono::SteadyClock tick period (GBA system clock: 16.78 MHz).
inline constexpr int64_t c_steadyClockPeriodDenominator = 16'777'216;

} // namespace chrono

} // namespace toy

//----------------------------------------------------------------------------------------------------------------------

#define IMPLEMENT_CONSOLE_APP(appClassName)                                                                            \
                                                                                                                       \
  int main() {                                                                                                         \
    appClassName app;                                                                                                  \
                                                                                                                       \
    app.setVersion({.major    = APP_VERSION_MAJOR,                                                                     \
                    .minor    = APP_VERSION_MINOR,                                                                     \
                    .patch    = APP_VERSION_PATCH,                                                                     \
                    .revision = APP_VERSION_REVISION});                                                                \
                                                                                                                       \
    return app.run(0, nullptr) ? EXIT_SUCCESS : EXIT_FAILURE;                                                          \
  }

//----------------------------------------------------------------------------------------------------------------------

#endif // defined(__GBA__)

#endif // SRC_PLATFORMS_GBA_PLATFORM_CONFIG_HPP_
