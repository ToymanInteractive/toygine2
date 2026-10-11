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
  \brief  Application object that reports the version of its build.

  Defines \ref toy::application::BaseApplication, built once from a \ref toy::application::Version and read back where
  the application names its build, such as a startup log line.

  \note Included by application.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_APPLICATION_BASE_APPLICATION_HPP_
#define INCLUDE_APPLICATION_BASE_APPLICATION_HPP_

#include <core.hpp>

#include "version.hpp"

namespace toy::application {

/*!
  \class BaseApplication
  \brief Application object that holds the version of its build.

  Takes a \ref toy::application::Version at construction and keeps a copy for its whole lifetime; no member replaces
  it. The copy shares the revision characters with the caller, so the revision should come from a string literal or
  other static storage.

  \section base_application_features Key Features

  * **Constexpr support**: construction and version() evaluate in a constant expression, so the application can be a
    \c constexpr object.
  * **Explicit construction**: a \ref toy::application::Version never converts to an application implicitly.
  * **Fixed version**: version() returns a \c const reference, and nothing changes the version after construction.
  * **No allocation**: the object holds one \ref toy::application::Version and nothing else.

  \section base_application_usage Usage Example

  \code
  #include "application.hpp"

  constexpr toy::application::BaseApplication app{
    toy::application::Version{26, 20, 0, "f7e57d4"},
  };

  const uint32_t major = app.version().major;
  \endcode

  \section base_application_performance Performance Characteristics

  * **Construction**: O(1); copies the version, not the revision characters.
  * **Access**: O(1); version() returns a reference to the stored copy.
  * **Memory usage**: the size of one \ref toy::application::Version.

  \section base_application_safety Safety Guarantees

  * **Lifetime**: the stored revision views characters the application does not own, so they have to outlive it.
  * **Contracts**: none; any version is accepted, an empty revision included.
  * **Memory safety**: nothing allocates; the version lives in the object itself.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \sa \ref toy::application::Version
*/
class BaseApplication {
public:
  /*!
    \brief Creates an application that reports \a version.

    \param version Version of the build; copied, while its revision characters stay with the caller.

    \post version() compares equal to \a version.

    \sa version()
  */
  explicit constexpr BaseApplication(const Version & version) noexcept;

  /*!
    \brief Returns the version passed at construction.

    \return Reference to the stored version, valid as long as the application exists.
  */
  constexpr const Version & version() const noexcept;

private:
  Version _version;
};

} // namespace toy::application

#endif // INCLUDE_APPLICATION_BASE_APPLICATION_HPP_
