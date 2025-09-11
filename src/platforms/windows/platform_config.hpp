//
// Copyright (c) 2025 by Toyman Interactive
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
  \brief  additional windows platform header
*/

#ifndef SRC_PLATFORMS_WINDOWS_PLATFORM_CONFIG_HPP_
#define SRC_PLATFORMS_WINDOWS_PLATFORM_CONFIG_HPP_

#if defined(_WIN32)

#define TARGET_OS OS_WINDOWS

#if defined(_M_IX86) || defined(__i386__)
#define TARGET_CPU CPU_INTEL_x86
#elif defined(_M_X64) || defined(__x86_64__)
#define TARGET_CPU CPU_INTEL_x64
#endif

#endif // defined(_WIN32)

//----------------------------------------------------------------------------------------------------------------------

#undef assert

#ifdef _DEBUG

#if defined(_MSC_VER)
#define __FUNC_SIGNATURE__ __FUNCSIG__
#elif defined(__GNUC__) || defined(__clang__)
#define __FUNC_SIGNATURE__ __PRETTY_FUNCTION__
#else
#define __FUNC_SIGNATURE__ __func__
#endif

#define assert(condition)                                                                                              \
  if (!(condition)) [[unlikely]]                                                                                       \
    toygine::assertion::assertion(#condition, nullptr, __FILE__, __FUNC_SIGNATURE__, __LINE__);                        \
  else                                                                                                                 \
    ((void)0);

#define assert_message(condition, message)                                                                             \
  if (!(condition)) [[unlikely]]                                                                                       \
    toygine::assertion::assertion(#condition, message, __FILE__, __FUNC_SIGNATURE__, __LINE__);                        \
  else                                                                                                                 \
    ((void)0);

#else // _DEBUG

#define assert(condition) ((void)0);
#define assert_message(condition, message) ((void)0);

#endif // _DEBUG

//----------------------------------------------------------------------------------------------------------------------

#endif // SRC_PLATFORMS_WINDOWS_PLATFORM_CONFIG_HPP_
