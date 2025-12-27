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
  \file   platform.hpp
  \brief  Platform enumeration and platform detection utilities.
*/

#ifndef INCLUDE_CORE_PLATFORM_HPP_
#define INCLUDE_CORE_PLATFORM_HPP_

/*!
  \enum Platform
  \brief Enumeration of target operating systems and platforms.

  Identifies the target platform for compile-time and runtime platform detection. Values use hexadecimal identifiers
  starting from \c 0x1000, with each platform family assigned a distinct range. This design enables compile-time
  platform checks and potential combination with CPU architecture identifiers.

  \section features Key Features

  - 🔧 **Constexpr Support**: All values are usable in constexpr contexts
  - 🎯 **Compile-Time Detection**: Enables compile-time conditional compilation via \c if \c constexpr
  - 📐 **Type Safety**: Strongly-typed enum class prevents implicit conversions
  - 🔗 **Architecture Integration**: Hex values designed to combine with CPU architecture identifiers

  \section usage Usage Example

  \code
  #include "core.hpp"

  // Compile-time platform detection
  constexpr Platform currentPlatform = Platform::MacOS;

  if constexpr (currentPlatform == Platform::MacOS) {
    // macOS-specific code (evaluated at compile time)
  } else if constexpr (currentPlatform == Platform::Windows) {
    // Windows-specific code (evaluated at compile time)
  } else if constexpr (currentPlatform == Platform::Linux) {
    // Linux-specific code (evaluated at compile time)
  }

  // Runtime platform checking
  Platform runtimePlatform = getCurrentPlatform();
  if (runtimePlatform == Platform::Linux) {
    // Linux-specific logic
  }
  \endcode

  \note Platform families use distinct hex ranges: desktop (\c 0x1000-\c 0x3000), mobile (\c 0x3100-\c 0x4000),
        consoles (\c 0x5000-\c 0x8000). iOS (\c 0x3100) shares the Apple family range with macOS (\c 0x3000).
*/
enum class Platform : unsigned int {
  /// Microsoft Windows Desktop operating system (Windows XP, Vista, 7, 8, 8.1, 10, 11, etc.)
  Windows = 0x1000,

  /// GNU/Linux operating system and Linux-based distributions
  Linux = 0x2000,

  /// Apple macOS desktop operating system
  MacOS = 0x3000,

  /// Apple iOS mobile operating system
  iOS = 0x3100,

  /// Google Android mobile operating system
  Android = 0x4000,

  /// Nintendo Game Boy Advance handheld gaming console
  GBA = 0x5000,

  /// Nintendo DS dual-screen handheld gaming console
  NDS = 0x6000,

  /// Nintendo 3DS handheld gaming console with stereoscopic 3D display
  N3DS = 0x7000,

  /// Nintendo Switch hybrid gaming console
  Switch = 0x8000,
};

#endif // INCLUDE_CORE_PLATFORM_HPP_
