//
// Copyright (c) 2025 Toyman Interactive
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
  \brief  additional nintendo 3ds platform header
*/

#ifndef SRC_PLATFORMS_3DS_PLATFORM_CONFIG_HPP_
#define SRC_PLATFORMS_3DS_PLATFORM_CONFIG_HPP_

#if defined(__DEVKITPRO__)

inline constexpr auto currentPlatform = Platform::N3DS;

#define TARGET_CPU CPU_ARM_32

#endif // defined(__DEVKITPRO__)

#include "../common/assertion_macro_gcc_clang.hpp"

#endif // SRC_PLATFORMS_3DS_PLATFORM_CONFIG_HPP_
