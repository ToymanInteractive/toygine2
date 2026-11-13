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

  Each leaf under platforms/ holds one target's values; the target toolchain's predefined macro selects the active leaf.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_PLATFORMS_PLATFORM_CONFIG_HPP_
#define INCLUDE_PLATFORMS_PLATFORM_CONFIG_HPP_

#if defined(__GBA__)
#include "platform_config_gba.hpp"
#elif defined(__NDS__)
#include "platform_config_nds.hpp"
#elif defined(__3DS__)
#include "platform_config_3ds.hpp"
#elif defined(__SWITCH__)
#include "platform_config_switch.hpp"
#elif defined(__gamecube__)
#include "platform_config_gamecube.hpp"
#elif defined(__wii__)
#include "platform_config_wii.hpp"
#elif defined(__WIIU__)
#include "platform_config_wii_u.hpp"
#elif defined(__MEGA_DRIVE__)
#include "platform_config_md.hpp"
#elif defined(_WIN32)
#include "platform_config_windows.hpp"
#elif defined(__APPLE__)
#include "platform_config_macos.hpp"
#elif defined(__linux__)
#include "platform_config_linux.hpp"
#else
#error "no platform_config.hpp for this target platform"
#endif

#endif // INCLUDE_PLATFORMS_PLATFORM_CONFIG_HPP_
