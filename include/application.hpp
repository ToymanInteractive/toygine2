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
  \file   application.hpp
  \brief  Umbrella header for the application module.

  Defines \ref toy::application::ConsoleApplication, \ref toy::application::CoreApplication, and
  \ref toy::application::Version: the console-mode intermediate base, the abstract application base class, and the
  version type.

  \note Include this header only; do not include internal headers directly.
*/

#ifndef INCLUDE_APPLICATION_HPP_
#define INCLUDE_APPLICATION_HPP_

#include "core.hpp"

/*!
  \namespace toy::application

  \brief Application lifecycle types: \ref toy::application::CoreApplication, \ref toy::application::ConsoleApplication,
         \ref toy::application::Version, and related utilities.
*/

#include "application/console_application.hpp"
#include "application/core_application.hpp"
#include "application/version.hpp"

//----------------------------------------------------------------------------------------------------------------------

#include "application/core_application.inl"
#include "application/version.inl"

//----------------------------------------------------------------------------------------------------------------------

#ifndef APP_VERSION_MAJOR
#define APP_VERSION_MAJOR 0
#endif

#ifndef APP_VERSION_MINOR
#define APP_VERSION_MINOR 0
#endif

#ifndef APP_VERSION_MAINTENANCE
#define APP_VERSION_MAINTENANCE 0
#endif

#ifndef APP_VERSION_REVISION
#define APP_VERSION_REVISION ""
#endif

#if !defined(IMPLEMENT_CONSOLE_APP)

#define IMPLEMENT_CONSOLE_APP(appClassName)                                                                            \
                                                                                                                       \
  void main() {                                                                                                        \
    static_assert(false, "Build target os doesn't support console application");                                       \
  }

#endif // !defined(IMPLEMENT_CONSOLE_APP)

#endif // INCLUDE_APPLICATION_HPP_
