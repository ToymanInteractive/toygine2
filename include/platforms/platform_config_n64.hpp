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
  \file   platform_config_n64.hpp
  \brief  Values of the platform constants for the Nintendo 64 target.

  Supplies what \ref toy::platform reads on this target; the dispatcher platforms/platform_config.hpp selects this
  file by the target toolchain's macro.

  \note Included by platforms/platform_config.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_PLATFORMS_PLATFORM_CONFIG_N64_HPP_
#define INCLUDE_PLATFORMS_PLATFORM_CONFIG_N64_HPP_

namespace toy::platform::detail {

// GCC for the VR4300 inlines constant-size copies as 8-byte load/store pairs; no memcpy() call up to 1024 bytes.
inline constexpr size_t inlineCopyMaxBytes = 128;

// Skips the flag per call site to save ROM and RAM, so Action::IgnoreAlways acts as Action::Ignore.
inline constexpr bool assertIgnoreAlways = false;

// This target has no counter backend: the stub clock reports nominal nanoseconds.
inline constexpr int64_t steadyClockPeriodDenominator = 1'000'000'000;

// The libdragon console font covers ASCII only, so non-ASCII text renders as garbage tiles.
inline constexpr bool consoleSupportsUtf8 = false;

} // namespace toy::platform::detail

#ifdef TOYGINE_DEBUG

// GCC emits a trap instruction, and the BIOS exception vector stops execution at the failing check.
#define TOYGINE_DEBUG_BREAK() __builtin_trap()

#endif // TOYGINE_DEBUG

#endif // INCLUDE_PLATFORMS_PLATFORM_CONFIG_N64_HPP_
