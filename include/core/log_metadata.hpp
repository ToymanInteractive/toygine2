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
  \file   log_metadata.hpp
  \brief  Constant part of a log call, fixed where the call is written.

  Defines \ref toy::log::Metadata, which holds the format string of a message, the source file and line the message
  comes from, and the \ref toy::log::Level it carries.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_LOG_METADATA_HPP_
#define INCLUDE_CORE_LOG_METADATA_HPP_

namespace toy::log {

/*!
  \struct Metadata
  \brief  Format string, source location, and severity of one log call.

  \ingroup Logger

  Aggregate the caller fills field by field. No member carries a default, so a field the initializer leaves out is
  value-initialized instead: a null pointer, a zero line, or \ref toy::log::Level::Off.

  \section metadata_features Key Features

  * **Aggregate initialization**: no constructor, so designated initializers name the fields at the call site.
  * **Constexpr support**: a record built from literals and constants evaluates in a constant expression.
  * **No allocation**: two pointers, a line number, and a level; neither string belongs to the record.
  * **Trivially copyable**: a copy is a byte copy of the object, and both copies point at the same characters.
  * **Type safety**: the severity is a \ref toy::log::Level, so a bare number does not compile in its place.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section metadata_usage Usage Example

  \code
  #include "core.hpp"

  constexpr toy::log::Metadata metadata{"asset {} loaded in {} ms", __FILE__, __LINE__, toy::log::Level::Info};
  \endcode

  \section metadata_performance Performance Characteristics

  * **Construction**: O(1); every field is stored as given, and neither string is scanned or copied.
  * **Access**: O(1); every field is read directly.
  * **Memory usage**: two pointers, a 32-bit number, and a byte; 24 bytes where a pointer is 8 bytes, 16 where it is 4.

  \section metadata_safety Safety Guarantees

  * **Lifetime**: \a format and \a file point at characters the record does not own, so both have to outlive every copy
    of it. A string literal covers that on its own; anything else lasts only as long as whoever built it.
  * **Contracts**: none; neither pointer is checked, a null one included.
  * **Memory safety**: nothing allocates, and the record owns nothing outside its own fields.
  * **Thread safety**: nothing synchronizes a write. A record no one writes after building it reads from any thread, a
    job body or the mixer callback included.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \note Holds no message arguments: only the format string of a message reaches the record.

  \sa \ref toy::log::Level
*/
struct Metadata {
  const char * format; ///< Format string of the message

  const char * file;   ///< Source file the call is written in, typically \c __FILE__

  uint32_t line;       ///< Line of the call within \a file, typically \c __LINE__

  Level level;         ///< Severity the call carries
};

} // namespace toy::log

#endif // INCLUDE_CORE_LOG_METADATA_HPP_
