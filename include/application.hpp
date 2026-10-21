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
  \file   application.hpp
  \brief  Umbrella header for the engine application module.

  Single public entry point for the application module: aggregates its public headers into namespace
  \ref toy::application.

  \note Include this header only; do not include internal headers directly.
*/

#ifndef INCLUDE_APPLICATION_HPP_
#define INCLUDE_APPLICATION_HPP_

#include "core.hpp"

/*!
  \namespace toy::application

  \brief Application-level types, among them the version a build reports about itself.
*/

#include "application/base_application.hpp"
#include "application/console_application.hpp"
#include "application/version.hpp"

//----------------------------------------------------------------------------------------------------------------------

#include "application/base_application.inl"
#include "application/console_application.inl"
#include "application/version.inl"

//----------------------------------------------------------------------------------------------------------------------

#ifndef APP_VERSION_MAJOR
/// Major version number IMPLEMENT_CONSOLE_APP passes to the application; \c 0 unless the build defines it.
#define APP_VERSION_MAJOR 0
#endif // APP_VERSION_MAJOR

#ifndef APP_VERSION_MINOR
/// Minor version number IMPLEMENT_CONSOLE_APP passes to the application; \c 0 unless the build defines it.
#define APP_VERSION_MINOR 0
#endif // APP_VERSION_MINOR

#ifndef APP_VERSION_PATCH
/// Patch number IMPLEMENT_CONSOLE_APP passes to the application; \c 0 unless the build defines it.
#define APP_VERSION_PATCH 0
#endif // APP_VERSION_PATCH

#ifndef APP_VERSION_REVISION
/// Source revision IMPLEMENT_CONSOLE_APP passes to the application; empty unless the build defines it.
#define APP_VERSION_REVISION ""
#endif // APP_VERSION_REVISION

#if !defined(IMPLEMENT_CONSOLE_APP)

/*!
  \brief Fails the build on a target that has no console application entry point.

  Takes effect only where the platform_config.hpp of the active target leaves the macro undefined.

  \param appClassName Application class derived from \ref toy::application::ConsoleApplication.
*/
#define IMPLEMENT_CONSOLE_APP(appClassName)                                                                            \
                                                                                                                       \
  int main() {                                                                                                         \
    static_assert(false, "Target platform doesn't support console application");                                       \
  }

#endif
#endif // INCLUDE_APPLICATION_HPP_
