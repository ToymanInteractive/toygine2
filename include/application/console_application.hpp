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
  \file   console_application.hpp
  \brief  Abstract base for console-mode application lifecycle.

  Defines \ref toy::application::ConsoleApplication: a \ref toy::application::CoreApplication subclass that registers
  interactive console assertion and stack-walk callbacks. Concrete platform subclasses inherit from this class and
  implement runInternal().

  \note Included by application.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_APPLICATION_CONSOLE_APPLICATION_HPP_
#define INCLUDE_APPLICATION_CONSOLE_APPLICATION_HPP_

#include "core_application.hpp"

namespace toy::application {

/*!
  \class ConsoleApplication

  \brief Abstract base for console-mode application lifecycle.

  ConsoleApplication extends \ref toy::application::CoreApplication with interactive console assertion and stack-walk
  handlers. On assertion failure the handler prints a formatted message to stdout and prompts the user to continue or
  break; the stack-walk handler prints each frame to stdout. Concrete platform subclasses must override runInternal()
  to provide the main loop.

  \section features Key Features

  - **Console assertion handler**: prints failure context and prompts continue/debug on assertion failure.
  - **Console stack-walk handler**: prints each stack frame to stdout.
  - **Abstract**: runInternal() remains pure virtual; platform subclasses provide the main loop.
  - **Exception safety**: All operations are noexcept.

  \section usage Usage Example

  \code
  #include "application.hpp"

  class MyApp : public toy::application::ConsoleApplication {
  protected:
    bool runInternal() noexcept override { return true; }
  };

  int main(int argc, char ** argv) {
    MyApp app;
    return app.run(argc, argv) ? 0 : 1;
  }
  \endcode

  \section performance Performance Characteristics

  - **Construction/destruction**: O(1); delegates to toy::initialize()/toy::deInitialize().
  - **Assertion handler**: interactive; not on any hot path.

  \section safety Safety Guarantees

  - **Exception safety**: All operations are noexcept.
  - **Singleton**: inherits singleton constraint from \ref toy::application::CoreApplication.

  \sa \ref toy::application::CoreApplication
*/
class ConsoleApplication : public CoreApplication {
public:
  /// Registers the console assertion and stack-walk callbacks via toy::initialize().
  ConsoleApplication() noexcept;

  /// Destroys the console application and shuts down the engine core module.
  ~ConsoleApplication() noexcept override;

  /// Copy construction is deleted to preserve the singleton invariant.
  ConsoleApplication(const ConsoleApplication &)             = delete;
  /// Copy assignment is deleted to preserve the singleton invariant.
  ConsoleApplication & operator=(const ConsoleApplication &) = delete;
  /// Move construction is deleted; instances are pinned to their declaration scope.
  ConsoleApplication(ConsoleApplication &&)                  = delete;
  /// Move assignment is deleted; instances are pinned to their declaration scope.
  ConsoleApplication & operator=(ConsoleApplication &&)      = delete;
};

} // namespace toy::application

#endif // INCLUDE_APPLICATION_CONSOLE_APPLICATION_HPP_
