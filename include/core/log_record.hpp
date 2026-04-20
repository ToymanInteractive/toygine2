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
  \file   log_record.hpp
  \brief  In-flight log record stored in the transport ring buffer.

  Defines \ref toy::log::Record and the \c LOG_MESSAGE_CAPACITY capacity constant that controls the per-record
  formatted-message buffer size.

  \note Included by core.hpp; do not include this file directly.
*/

#ifndef INCLUDE_CORE_LOG_RECORD_HPP_
#define INCLUDE_CORE_LOG_RECORD_HPP_

/*!
  \def LOG_MESSAGE_CAPACITY

  \brief Per-record formatted-message buffer size in bytes (default \c 256, including null terminator).

  Define this macro before including the header (or via compiler flags) to override the default size.
*/
#ifndef LOG_MESSAGE_CAPACITY
#define LOG_MESSAGE_CAPACITY 256
#endif // LOG_MESSAGE_CAPACITY

namespace toy::log {

/// Maximum byte length of a formatted log message including the null terminator.
inline constexpr size_t c_messageCapacity = LOG_MESSAGE_CAPACITY;

/*!
  \struct Record

  \brief  Single log entry in the transport ring buffer.

  Carries the pre-formatted message along with its metadata pointer and timestamp. The message is formatted at the call
  site before being pushed into the transport, so the backend thread (or manual flush) only needs to forward the string
  to the active \ref toy::log::ISink.

  \section features Key Features

  - **No allocation**: Fixed-size struct lives entirely inside the ring buffer.
  - **Pre-formatted**: Message is formatted at push time; backend is format-free.
  - **Metadata pointer**: File, line, and level available for structured sink output.
  - **Timestamp**: Captured at push time via \ref toy::TimestampPolicy.

  \section performance Performance Characteristics

  - **Size**: \c sizeof(Record) = 24 + \c LOG_MESSAGE_CAPACITY bytes.
  - **Copy cost**: One struct assignment per push and per pop.

  \section safety Safety Guarantees

  - **Lifetime**: \a meta must point to a \c static \c constexpr \ref toy::log::Metadata.
  - **Exception safety**: No exceptions; all fields are value types.

  \sa \ref toy::log::Metadata, \ref toy::log::ISink, \ref toy::TimestampPolicy
*/
struct Record {
  /// Pointer to the static call-site metadata (format pattern, file, line, level).
  const Metadata * meta = nullptr;

  /// Platform timestamp captured at push time.
  uint32_t timestamp = 0;

  /// Formatted message ready for sink output.
  FixedString<c_messageCapacity> message;
};

} // namespace toy::log

#endif // INCLUDE_CORE_LOG_RECORD_HPP_
