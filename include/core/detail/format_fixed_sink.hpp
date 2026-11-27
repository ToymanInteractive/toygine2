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
  \file   format_fixed_sink.hpp
  \brief  Byte sink that limits writes to a fixed capacity and records overflow.

  Defines \ref toy::detail::FormatFixedSink, which the format entry points construct over a fixed-size buffer and pass
  to the walk. Internal to the format implementation.
*/

#ifndef INCLUDE_CORE_DETAIL_FORMAT_FIXED_SINK_HPP_
#define INCLUDE_CORE_DETAIL_FORMAT_FIXED_SINK_HPP_

namespace toy::detail {

/*!
  \class FormatFixedSink
  \brief Byte sink over caller-owned storage that limits writes to its capacity and records overflow.

  The format entry points construct one over a fixed-size buffer and pass it to the walk. Writes copy only the bytes
  that fit and leave the overflow flag set, so the entry point can detect truncation.
*/
class FormatFixedSink {
public:
  /*!
    \brief Writes a view, clamped at the remaining capacity.

    \param data Characters to write; may be empty.
  */
  constexpr void write(span<const char> data) noexcept;

  /*!
    \brief Writes one character through write().

    \param character Byte to write.
  */
  constexpr void put(char character) noexcept;

  /// Destination bytes of the fixed-capacity result.
  char * out;

  /// Destination capacity in bytes.
  size_t capacity;

  /// Bytes written so far.
  size_t size;

  /// Set when a write exceeds the remaining capacity.
  bool overflow;
};

} // namespace toy::detail

#include "format_fixed_sink.inl"

#endif // INCLUDE_CORE_DETAIL_FORMAT_FIXED_SINK_HPP_
