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
  \brief  additional linux platform header
*/

#ifndef SRC_PLATFORMS_LINUX_PLATFORM_CONFIG_HPP_
#define SRC_PLATFORMS_LINUX_PLATFORM_CONFIG_HPP_

#if defined(__linux__)

#define TARGET_OS OS_LINUX

#if defined(__aarch64__)
#define TARGET_CPU CPU_ARM_64
#elif defined(__x86_64__)
#define TARGET_CPU CPU_INTEL_x64
#else
#define TARGET_CPU CPU_INTEL_x86
#endif

#endif // defined(__linux__)

//----------------------------------------------------------------------------------------------------------------------

#undef assert

#ifdef _DEBUG

#define assert(x)                                                                                                      \
  if (!(x))                                                                                                            \
    toygine::assertion::assertion(#x, nullptr, __FILE__, __PRETTY_FUNCTION__, __LINE__);                               \
  else                                                                                                                 \
    ((void)0);

#define assert_message(x, message)                                                                                     \
  if (!(x))                                                                                                            \
    toygine::assertion::assertion(#x, message, __FILE__, __PRETTY_FUNCTION__, __LINE__);                               \
  else                                                                                                                 \
    ((void)0);

#else // _DEBUG

#define assert(x) ((void)0);
#define assert_message(x, message) ((void)0);

#endif // _DEBUG

//----------------------------------------------------------------------------------------------------------------------

#endif // SRC_PLATFORMS_LINUX_PLATFORM_CONFIG_HPP_
