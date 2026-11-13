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
  \brief  Selects the platform values header for the active target.

  Leaves live under platforms/<slug>/platform_config.hpp; the target toolchain's predefined macro selects one.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_PLATFORMS_PLATFORM_CONFIG_HPP_
#define INCLUDE_PLATFORMS_PLATFORM_CONFIG_HPP_

#if defined(__GBA__)
#include "gba/platform_config.hpp"
#elif defined(__NDS__)
#include "nds/platform_config.hpp"
#elif defined(__3DS__)
#include "3ds/platform_config.hpp"
#elif defined(__SWITCH__)
#include "switch/platform_config.hpp"
#elif defined(__gamecube__)
#include "gamecube/platform_config.hpp"
#elif defined(__wii__)
#include "wii/platform_config.hpp"
#elif defined(__WIIU__)
#include "wii_u/platform_config.hpp"
#elif defined(__MEGA_DRIVE__)
#include "md/platform_config.hpp"
#elif defined(_WIN32)
#include "windows/platform_config.hpp"
#elif defined(__APPLE__)
#include "macos/platform_config.hpp"
#elif defined(__linux__)
#include "linux/platform_config.hpp"
#else
#error "no platform_config.hpp for this target platform"
#endif

#endif // INCLUDE_PLATFORMS_PLATFORM_CONFIG_HPP_
