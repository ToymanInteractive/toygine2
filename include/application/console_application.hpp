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
  \file   console_application.hpp
  \brief  Application object that writes text to the output and error streams of the system console.

  Defines \ref toy::application::ConsoleApplication, the application type for a program that reports to the user
  through console text. Adds static write calls for the output and error streams to
  \ref toy::application::BaseApplication.

  \note Included by application.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_APPLICATION_CONSOLE_APPLICATION_HPP_
#define INCLUDE_APPLICATION_CONSOLE_APPLICATION_HPP_

#include <core.hpp>

#include "version.hpp"

namespace toy::application {

/*!
  \class ConsoleApplication
  \brief Application object for a program that reports through the system console.

  Extends \ref toy::application::BaseApplication with two pairs of write calls: one pair for the output stream, one for
  the error stream. In each pair, one call writes the text as is and the other ends the line after it.

  \section console_application_features Key Features

  * **Constexpr support**: construction evaluates in a constant expression, so the application can be a \c constexpr
    object.
  * **Explicit construction**: a \ref toy::application::Version never converts to an application implicitly.
  * **Separate streams**: writeOutput() and writeError() keep regular output apart from diagnostics.
  * **Line variants**: writeOutputLine() and writeErrorLine() end the line after the text.
  * **Text input**: each write call takes any \ref toy::StringLike text, \ref toy::String and \ref toy::StringView
    included.
  * **Static output**: no write call needs an instance, so code without access to the application object can write.
  * **Base services**: version(), pid(), sleep(), and yield() come from \ref toy::application::BaseApplication.

  \section console_application_usage Usage Example

  \code
  #include "application.hpp"

  constexpr toy::application::ConsoleApplication app{
    toy::application::Version{26, 20, 0, "f7e57d4"},
  };

  toy::application::ConsoleApplication::writeOutputLine(toy::StringView{"Hello world from Console sample app"});
  toy::application::ConsoleApplication::writeError(toy::StringView{"Missing config: "});
  toy::application::ConsoleApplication::writeErrorLine(toy::StringView{"settings.cfg"});
  \endcode

  \section console_application_performance Performance Characteristics

  * **Construction**: O(1); copies the version, as \ref toy::application::BaseApplication does.
  * **Output**: O(1) to wrap the text in a \ref toy::StringView; the platform console does the rest.
  * **Memory usage**: the size of \ref toy::application::BaseApplication; the class adds no data members.

  \section console_application_safety Safety Guarantees

  * **Contracts**: the header checks none; the constructor accepts any version.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \sa \ref toy::application::BaseApplication, \ref toy::StringLike
*/
class ConsoleApplication final : public BaseApplication {
public:
  /*!
    \brief Creates a console application that reports \a version.

    \param version Version of the build to report.

    \post version() compares equal to \a version.

    \sa version()
  */
  explicit constexpr ConsoleApplication(const Version & version) noexcept;

  /*!
    \brief Writes \a text to the console output stream without ending the line.

    \param text Text to write, of any \ref toy::StringLike type.

    \sa writeOutputLine(), writeError()
  */
  static void writeOutput(const StringLike auto & text) noexcept;

  /*!
    \brief Writes \a text to the console output stream and ends the line.

    \param text Text to write, of any \ref toy::StringLike type.

    \sa writeOutput(), writeErrorLine()
  */
  static void writeOutputLine(const StringLike auto & text) noexcept;

  /*!
    \brief Writes \a text to the console error stream without ending the line.

    \param text Text to write, of any \ref toy::StringLike type.

    \sa writeErrorLine(), writeOutput()
  */
  static void writeError(const StringLike auto & text) noexcept;

  /*!
    \brief Writes \a text to the console error stream and ends the line.

    \param text Text to write, of any \ref toy::StringLike type.

    \sa writeError(), writeOutputLine()
  */
  static void writeErrorLine(const StringLike auto & text) noexcept;

private:
  /*!
    \brief Passes \a text to the platform console; all four public write calls forward here.

    \param text    Text to write.
    \param isError \c true for the error stream, \c false for the output stream.
    \param newLine \c true to end the line after the text.
  */
  static void writeToSystemConsole(StringView text, bool isError, bool newLine) noexcept;
};

} // namespace toy::application

#endif // INCLUDE_APPLICATION_CONSOLE_APPLICATION_HPP_
