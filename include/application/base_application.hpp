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
  \file   base_application.hpp
  \brief  Abstract application object that runs the program body and holds its version and command-line arguments.

  Defines \ref toy::application::BaseApplication, the base class every application type derives from.

  \note Included by application.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_APPLICATION_BASE_APPLICATION_HPP_
#define INCLUDE_APPLICATION_BASE_APPLICATION_HPP_

#include <core.hpp>

#include "version.hpp"

namespace toy::application {

/*!
  \class BaseApplication
  \brief Abstract application object that runs the program body and holds the version of its build.

  A program derives from it and implements runInternal(). The entry point constructs the derived application from a
  \ref toy::application::Version, then calls run() with the command-line arguments. No setter changes the stored
  version; assigning another application replaces the whole object, version included.

  \section base_application_features Key Features

  * **Derived-only construction**: the constructor is protected and runInternal() is pure virtual, so only a derived
    application can exist.
  * **Constexpr support**: construction and version() evaluate in a constant expression, so a derived application can
    be a \c constexpr object.
  * **Read-only version**: version() returns a \c const reference, and the class has no setter.
  * **Command-line arguments**: run() stores them, and argsCount(), args(), and arg() read them back.
  * **Process ID**: pid() reports the ID of the running process and needs no instance.
  * **Thread suspension**: sleep() and yield() hand the calling thread back to the scheduler and need no instance.

  \section base_application_usage Usage Example

  \code
  #include "application.hpp"

  class ToolApplication final : public toy::application::BaseApplication {
  public:
    explicit constexpr ToolApplication(const toy::application::Version & version) noexcept
      : BaseApplication(version) {}

  private:
    bool runInternal() noexcept override {
      return argsCount() > 1;
    }
  };

  int main(int argc, char * argv[]) {
    ToolApplication app{toy::application::Version{26, 20, 0, "f7e57d4"}};

    return app.run(static_cast<size_t>(std::max(0, argc)), argv) ? EXIT_SUCCESS : EXIT_FAILURE;
  }
  \endcode

  \section base_application_performance Performance Characteristics

  * **Construction**: O(1); copies the version, not the revision characters.
  * **Access**: O(1); version(), argsCount(), args(), and arg() read stored values.
  * **Run**: one virtual call to runInternal() per run() call.
  * **Memory usage**: one \ref toy::application::Version, the argument count and pointer, and the virtual table
    pointer.

  \section base_application_safety Safety Guarantees

  * **Lifetime**: the stored revision and arguments point to memory the application does not own. That memory has to
    outlive the application, as a string literal and the arguments main() receives do.
  * **Contracts**: arg() checks its index against argsCount() with assert_message in debug builds; a shipping build
    returns \c nullptr instead.
  * **Memory safety**: nothing allocates; run() stores the argument pointer without copying the arguments.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \sa \ref toy::application::Version, \ref toy::application::ConsoleApplication
*/
class BaseApplication {
public:
  /*!
    \brief Destroys the application.

    Destroying a derived application through a pointer to the base also runs the derived destructor.
  */
  virtual ~BaseApplication() = default;

  /*!
    \brief Returns the version the application reports.

    \return Reference to the stored version, valid as long as the application exists.

    \note Assigning another application changes the referenced value; the reference itself stays valid.
  */
  [[nodiscard]] constexpr const Version & version() const noexcept;

  /*!
    \brief Stores the command-line arguments and runs the application body.

    Calls runInternal() once per call, after the arguments become readable through argsCount(), args(), and arg().

    \param argc Number of entries in \a argv.
    \param argv Command-line arguments, as main() receives them.

    \return Result of runInternal(): \c true when the application finished successfully, \c false otherwise.

    \pre \a argv holds at least \a argc entries and outlives the application.

    \post argsCount() returns \a argc, and args() returns \a argv.

    \sa runInternal()
  */
  [[nodiscard]] bool run(size_t argc, char ** argv) noexcept;

  /*!
    \brief Returns the number of command-line arguments.

    \return Argument count the last run() call received, or \c 0 before the first call.

    \sa args(), arg()
  */
  [[nodiscard]] size_t argsCount() const noexcept;

  /*!
    \brief Returns the command-line arguments.

    \return Argument array the last run() call received, or \c nullptr before the first call.

    \sa argsCount(), arg()
  */
  [[nodiscard]] char ** args() noexcept;

  /*!
    \brief Returns one command-line argument.

    \param index Zero-based position in the argument array.

    \return Argument at \a index, or \c nullptr when \a index is out of range or run() has not been called.

    \pre \a index is less than argsCount(); checked by assert_message in debug builds.

    \sa argsCount(), args()
  */
  [[nodiscard]] const char * arg(size_t index) const noexcept;

  /*!
    \brief Returns the ID the operating system assigned to the running process.

    Each call queries the platform; read the ID once at startup and keep it.

    \return Process ID, or \c 0 when the target has no implementation or the system call fails.

    \note Returns \c 0 on Sega MD, Nintendo GBA, Nintendo DS, Nintendo 64, Nintendo GameCube, Nintendo Wii, and Nintendo
          Wii U, where the build links the stub implementation.
    \note On Nintendo Switch, the result holds only the low 32 bits of the 64-bit process ID.
  */
  [[nodiscard]] static uint32_t pid() noexcept;

  /*!
    \brief Blocks the calling thread for the given number of milliseconds.

    The actual duration depends on the scheduler and the platform timer resolution; treat the argument as approximate.

    \param milliseconds Time to block, in milliseconds.

    \note Returns at once on targets without a thread implementation.
    \warning Never call it from the frame loop, a job body, or the mixer callback.

    \sa yield()
  */
  static void sleep(size_t milliseconds) noexcept;

  /*!
    \brief Gives the rest of the calling thread's time slice to another thread that is ready to run.

    Returns without waiting when no other thread is ready. Which threads qualify depends on the platform scheduler.

    \sa sleep()
  */
  static void yield() noexcept;

protected:
  /*!
    \brief Creates an application that reports \a version.

    \param version Version of the build to report.

    \post version() compares equal to \a version.

    \sa version()
  */
  explicit constexpr BaseApplication(const Version & version) noexcept;

  /*!
    \brief Runs the application body; the derived application implements it.

    run() calls it after storing the command-line arguments, so argsCount(), args(), and arg() already return them.

    \return \c true when the application finished successfully, \c false otherwise.

    \sa run()
  */
  virtual bool runInternal() noexcept = 0;

private:
  /// Version the application reports
  Version _version;

  /// Argument count the last run() call received
  size_t _argC = 0;

  /// Argument array the last run() call received
  char ** _argV = nullptr;
};

} // namespace toy::application

#endif // INCLUDE_APPLICATION_BASE_APPLICATION_HPP_
