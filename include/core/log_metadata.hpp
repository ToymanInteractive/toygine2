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
  \file   log_metadata.hpp
  \brief  Compile-time metadata associated with each log call site.

  Defines \ref toy::log::Metadata: a POD aggregate created once per \c LOG_* macro invocation as a \c static
  \c constexpr local variable. Only its address is passed at runtime.

  \note Included by core.hpp; do not include this file directly.
*/

#ifndef INCLUDE_CORE_LOG_METADATA_HPP_
#define INCLUDE_CORE_LOG_METADATA_HPP_

namespace toy::log {

/*!
  \struct Metadata

  \brief  Immutable per-call-site descriptor stored as a static constexpr local.

  Created once at compile time by each \c LOG_* macro expansion. Only a pointer to the instance is passed at runtime,
  keeping the hot path to a single pointer copy.

  \section features Key Features

  - **Zero runtime cost**: Entire struct lives in read-only data; no construction at call time.
  - **No allocation**: Pointer-sized token on the hot path.
  - **Source location**: Carries file, line, and severity for structured log output.
  - **Constexpr**: All fields are string/integer literals; initialization is \c constexpr.

  \section usage Usage Example

  \code
  #include "core.hpp"

  static constexpr toy::log::Metadata meta{"player pos=({}, {})", __FILE__, __LINE__, toy::log::Level::Info};
  toy::LogBackend::instance().push(&meta, x, y);
  \endcode

  \section performance Performance Characteristics

  - **Runtime overhead**: One pointer copy per log call.
  - **Memory usage**: 24 bytes (two pointers + uint32_t + uint8_t + padding).

  \section safety Safety Guarantees

  - **Lifetime**: Must have static storage duration; \c LOG_* macros guarantee this.
  - **Exception safety**: POD aggregate; no exceptions possible.
  - **Thread safety**: Read-only after construction; safe to access from any thread.

  \sa toy::log::Level, toy::LogBackend
*/
struct Metadata {
  /// \c {} / \c {N} format pattern passed to toy::vformatTo() on the backend.
  const char * format;

  /// Source file path, typically \c __FILE__.
  const char * file;

  /// Source line number, typically \c __LINE__.
  size_t line;

  /// Severity level of the message.
  Level level;
};

} // namespace toy::log

#endif // INCLUDE_CORE_LOG_METADATA_HPP_
