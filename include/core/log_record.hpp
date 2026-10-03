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
  \brief  Formatted text of one log call, kept together with the call it came from and the time it was made.

  Defines \ref toy::log::Record, which holds the message text in a buffer of its own and points at the
  \ref toy::log::Metadata of its call, and LOG_MESSAGE_CAPACITY, the size of that buffer.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_LOG_RECORD_HPP_
#define INCLUDE_CORE_LOG_RECORD_HPP_

#include "string.hpp"

#ifndef LOG_MESSAGE_CAPACITY
/*!
  \def   LOG_MESSAGE_CAPACITY
  \brief Size in bytes of the buffer a \ref toy::log::Record keeps its message in, the terminator included.

  A record holds at most this value minus one characters of message text. Define it ahead of core.hpp, on the compiler
  command line or in the target's CMake definitions, to trade message length for memory on a constrained target. It
  defaults to \c 256.

  \note Every record carries the whole buffer, so the value adds to the size of each record whatever its text.
  \note Must be greater than \c 0, checked by a static assertion in \ref toy::FixedStringStorage.
*/
#define LOG_MESSAGE_CAPACITY 256
#endif // LOG_MESSAGE_CAPACITY

namespace toy::log {

/// Size in bytes of the message buffer of every \ref toy::log::Record, the terminator included
inline constexpr size_t c_messageCapacity = LOG_MESSAGE_CAPACITY;

/*!
  \struct Record
  \brief  Formatted message of one log call, with a pointer to the call's metadata and a time stamp.

  Aggregate the producer fills field by field. The message text is copied into the record, while the format string,
  source location, and severity stay in the \ref toy::log::Metadata the record points at. Every field has a default, so
  a record built without an initializer holds no metadata, a time stamp of \c 0, and an empty message.

  \section record_features Key Features

  * **Aggregate initialization**: no constructor, so designated initializers name the fields at the call site.
  * **Owned message**: the text lives in the record, so it stays readable after the arguments it was formatted from
    are gone.
  * **Fixed size**: the message buffer takes LOG_MESSAGE_CAPACITY bytes whatever the text, and nothing allocates.
  * **Constexpr support**: a record builds and reads in a constant expression.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section record_usage Usage Example

  \code
  #include "core.hpp"

  static constexpr toy::log::Metadata metadata{"asset {} loaded", __FILE__, __LINE__, toy::log::Level::Info};

  toy::log::Record record{.meta = &metadata, .timestamp = 0, .message = "asset player loaded"};
  \endcode

  \section record_performance Performance Characteristics

  * **Construction**: O(n) in the length of the message, which is copied into the buffer; the metadata is stored as a
    pointer.
  * **Copy**: the message copies as \ref toy::FixedStringStorage does, and the other fields as bytes.
  * **Memory usage**: one pointer, a 32-bit time stamp, and the message buffer with its length; at the default
    capacity, 280 bytes where a pointer is 8 bytes and 268 where it is 4.

  \section record_safety Safety Guarantees

  * **Lifetime**: \ref toy::log::Record::meta points at metadata the record does not own, so the metadata has to
    outlive every copy of the record. A \c static \c constexpr \ref toy::log::Metadata covers that on its own.
  * **Contracts**: none on the record itself; the metadata pointer is not checked, a null one included.
  * **Memory safety**: nothing allocates, and the message buffer lives inside the record.
  * **Thread safety**: nothing synchronizes a write. A record no one writes after building it reads from any thread.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section record_compatibility Compatibility

  * Nothing allocates or calls into the platform, so the type suits embedded and retro targets; a lower
    LOG_MESSAGE_CAPACITY shrinks every record where memory is short.

  \note The message holds at most LOG_MESSAGE_CAPACITY minus one characters; a longer text is rejected as
        \ref toy::String rejects an edit that does not fit.
  \note A copy is a byte copy only while the message storage fits \ref toy::platform::c_inlineCopyMaxBytes, and the
        default capacity does not.

  \sa \ref toy::log::Metadata
  \sa \ref toy::FixedString
*/
struct Record {
  const Metadata * meta{nullptr};         ///< Metadata of the call the message came from, \c nullptr until set

  uint32_t timestamp{0};                  ///< Time of the call in the unit of whoever fills the record, \c 0 until set

  FixedString<c_messageCapacity> message; ///< Formatted text of the message
};

} // namespace toy::log

#endif // INCLUDE_CORE_LOG_RECORD_HPP_
