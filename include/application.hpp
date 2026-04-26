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

/*!
  \def IMPLEMENT_CONSOLE_APP(appClassName)

  \brief Defines the platform entry point for a console application.

  The macro expands to a \c main function that default-constructs \a appClassName, sets the application version from the
  build-system macros \c APP_VERSION_MAJOR, \c APP_VERSION_MINOR, \c APP_VERSION_PATCH, and \c APP_VERSION_REVISION,
  then delegates execution to toy::application::CoreApplication::run(). Place it once in the translation unit that
  defines the concrete application class. On unsupported target platforms the macro expands to a \c static_assert
  compile-time error.

  \param appClassName Concrete class derived from \ref toy::application::ConsoleApplication. Must be
                      default-constructible.

  \pre \a appClassName is a complete type that publicly derives from \ref toy::application::ConsoleApplication.
  \pre \a appClassName provides a public default constructor.
  \pre The build system defines \c APP_VERSION_MAJOR, \c APP_VERSION_MINOR, \c APP_VERSION_PATCH, and
       \c APP_VERSION_REVISION before this macro is expanded.
  \pre This macro is expanded at most once per executable.

  \sa \ref toy::application::ConsoleApplication, \ref toy::application::CoreApplication
*/
#if !defined(IMPLEMENT_CONSOLE_APP)

#define IMPLEMENT_CONSOLE_APP(appClassName) static_assert(false, "Build target os doesn't support console application");

#endif // !defined(IMPLEMENT_CONSOLE_APP)

#endif // INCLUDE_APPLICATION_HPP_
