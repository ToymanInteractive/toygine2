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
  \file   log_level.hpp
  \brief  Severity levels for the logger.

  Defines \ref toy::log::Level used to filter log output at compile time via \c LOG_MIN_LEVEL and at runtime via
  \ref toy::LogBackend.

  \note Included by core.hpp; do not include this file directly.
*/

#ifndef INCLUDE_CORE_LOG_LEVEL_HPP_
#define INCLUDE_CORE_LOG_LEVEL_HPP_

namespace toy::log {

/*!
  \enum Level

  \brief Severity level of a log message.

  Numeric ordering enables compile-time filtering: a message is compiled away when its level is less than
  \c LOG_MIN_LEVEL.

  \sa LogMetadata, LOG_MIN_LEVEL
*/
enum class Level : uint8_t {
  /// Fine-grained diagnostic information.
  Trace = 0,

  /// Coarser diagnostic information useful during development.
  Debug = 1,

  /// Normal operational events.
  Info = 2,

  /// Potentially harmful situations that do not prevent execution.
  Warn = 3,

  /// Error events that may allow continued execution.
  Error = 4,

  /// Disables all logging; no messages are emitted.
  Off = 5,
};

} // namespace toy::log

#endif // INCLUDE_CORE_LOG_LEVEL_HPP_
