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
  \file   platform.hpp
  \brief  Constants whose values depend on the target platform.

  Defines the \ref toy::platform constants over the values the active platform supplies in its platform_config.hpp, so
  engine code reads one documented name on every target.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_PLATFORM_HPP_
#define INCLUDE_CORE_PLATFORM_HPP_

namespace toy::platform {

/*!
  \brief Largest object, in bytes, that copies faster whole than through a call to memcpy().

  Lets a type choose between copying all of its bytes and copying only the part in use. On the retro targets the value
  is the size up to which GCC copies an object with inline loads and stores; on desktop it comes from a benchmark,
  because clang inlines copies well past the size where they still pay off.

  \note Each target sets the value in the platform_config.hpp of its directory under src/platforms, with a comment on
        where the number comes from.
  \note Values not measured on the target are estimates, and the comment beside each one says so.
*/
inline constexpr size_t inlineCopyMaxBytes = detail::inlineCopyMaxBytes;

/*!
  \brief Whether assert_message keeps a flag per call site for \ref toy::assertion::Action::IgnoreAlways.

  \c true on the desktop targets. The consoles set \c false to save the flag and the code that reads it at every call
  site, so \ref toy::assertion::Action::IgnoreAlways acts as \ref toy::assertion::Action::Ignore there.

  \note Each target sets the value in the platform_config.hpp of its directory under src/platforms.
*/
inline constexpr bool assertIgnoreAlways = detail::assertIgnoreAlways;

} // namespace toy::platform

#endif // INCLUDE_CORE_PLATFORM_HPP_
