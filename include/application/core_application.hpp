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
  \file   core_application.hpp
  \brief  Abstract base class for platform application lifecycle.

  Defines \ref toy::application::CoreApplication: the abstract base managing engine initialization, command-line
  argument storage, version tracking, and the main run loop entry point. Platform-specific subclasses implement
  runInternal() and supply pid() / sleep() via native APIs.

  \note Included by application.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_APPLICATION_CORE_APPLICATION_HPP_
#define INCLUDE_APPLICATION_CORE_APPLICATION_HPP_

#include "version.hpp"

namespace toy::application {

/*!
  \class CoreApplication

  \brief Abstract base for platform application lifecycle management.

  CoreApplication owns engine initialization, command-line argument storage, version tracking, and the main run loop
  entry point. Platform-specific subclasses implement runInternal() and provide pid() and sleep() via native APIs.
  Exactly one CoreApplication instance may exist at a time; the constructor registers it as the singleton and the
  destructor clears it.

  \section features Key Features

  - **Singleton access**: instance() returns the single active application; only one instance is permitted.
  - **Lifecycle management**: Constructor invokes toy::initialize(); destructor invokes toy::deInitialize().
  - **Argument handling**: Stores argc/argv from run() and exposes typed accessors.
  - **Version tracking**: Holds a \ref toy::application::Version for the running application.
  - **Platform extension**: Subclasses override runInternal() and implement pid()/sleep() per platform.
  - **Exception safety**: All operations are noexcept.

  \section usage Usage Example

  \code
  #include "application.hpp"

  class MyApp : public toy::application::CoreApplication {
  public:
    MyApp() : CoreApplication(nullptr, nullptr) {}
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
  - **Argument access**: O(1) by index.
  - **Memory**: No heap allocation in CoreApplication itself.

  \section safety Safety Guarantees

  - **Contracts**: argument() asserts index < argumentsCount() in debug builds.
  - **Singleton**: Constructing a second instance while one is alive overwrites the singleton and causes undefined
    behavior.
  - **Exception safety**: All operations are noexcept.

  \warning Only one CoreApplication instance may exist at a time.

  \sa \ref toy::application::Version
*/
class CoreApplication {
public:
  /// Shuts down the engine core module and clears the singleton instance.
  virtual ~CoreApplication() noexcept;

  /*!
    \brief Returns the application version.

    \return Const reference to the current \ref toy::application::Version.
  */
  const Version & version() const noexcept;

  /*!
    \brief Sets the application version.

    \param version New version to assign.

    \post version() returns \a version.
  */
  void setVersion(Version version) noexcept;

  /*!
    \brief Returns the command-line argument at \a index.

    \param index Zero-based argument index.

    \return Pointer to the argument string, or \c nullptr if \a index is out of range or no arguments are set.

    \pre \a index < argumentsCount().
  */
  const char * argument(size_t index) const noexcept;

  /*!
    \brief Returns the raw argument array.

    \return Pointer to the argv array passed to setArguments(), or \c nullptr if not set.
  */
  char ** arguments() const noexcept;

  /*!
    \brief Returns the number of command-line arguments.

    \return Number of arguments set via setArguments() or run().
  */
  size_t argumentsCount() const noexcept;

  /*!
    \brief Stores command-line arguments for later access via argument() and arguments().

    \param argc Argument count; stored as a size_t if positive.
    \param argv Argument vector; stored as-is.

    \post argumentsCount() == \a argc (when \a argc > 0) and arguments() == \a argv.
  */
  void setArguments(int argc, char ** argv) noexcept;

  /*!
    \brief Stores arguments and delegates to the platform-specific runInternal().

    \param argc Argument count forwarded to setArguments().
    \param argv Argument vector forwarded to setArguments().

    \return \c true if the application completed successfully, \c false otherwise.

    \post argumentsCount() and arguments() reflect \a argc and \a argv.
  */
  bool run(int argc, char ** argv) noexcept;

  /*!
    \brief Returns the OS process identifier for the current process.

    \return PID of the running process.
  */
  uint32_t pid() const noexcept;

  /*!
    \brief Suspends the calling thread for at least \a milliseconds.

    \param milliseconds Duration to sleep in milliseconds.
  */
  void sleep(size_t milliseconds) const noexcept;

  /*!
    \brief Returns the active CoreApplication instance.

    \return Pointer to the singleton, or \c nullptr if no instance exists.
  */
  static CoreApplication * instance() noexcept;

protected:
  /*!
    \brief Initializes the engine core module and registers this object as the singleton.

    Calls toy::initialize() with the supplied callbacks, then registers this object as the active instance returned
    by instance().

    \param assertionCallback Callback invoked on assertion failure; may be \c nullptr to disable.
    \param stackWalkCallback Callback invoked per stack frame on assertion failure; may be \c nullptr to suppress.

    \post instance() == this and the assertion subsystem is active.

    \warning Only one CoreApplication may exist at a time. Constructing a second instance overwrites the singleton.

    \sa \ref toy::assertion::AssertionCallback, \ref toy::assertion::StackWalkCallback
  */
  CoreApplication(assertion::AssertionCallback assertionCallback,
                  assertion::StackWalkCallback stackWalkCallback) noexcept;

  /*!
    \brief Platform-specific run loop; called by run() after arguments are stored.

    \return \c true on success, \c false on failure.
  */
  virtual bool runInternal() noexcept = 0;

private:
  /// Arguments count
  size_t _argumentsCount{0};

  /// Argument vector.
  char ** _argumentsVector{nullptr};

  /// Application version.
  Version _version{};
};

} // namespace toy::application

#endif // INCLUDE_APPLICATION_CORE_APPLICATION_HPP_
