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
  \file   gba_filtering_stream_buf.hpp
  \brief  ANSI escape sequence filter streambuf for Nintendo GBA.

  Defines GbaFilteringStreamBuf, a \c std::streambuf that strips CSI (Control Sequence Introducer) and SGR (Select
  Graphic Rendition) codes so that only plain text is forwarded to the underlying stream. Intended for test output on
  GBA hardware, where ANSI escapes are not supported.
*/

#ifndef TEST_GBA_FILTERING_STREAM_BUF_HPP_
#define TEST_GBA_FILTERING_STREAM_BUF_HPP_

#include <cstddef>
#include <iostream>
#include <streambuf>

/*!
  \brief Filters ANSI escape sequences (CSI, including SGR color codes) so only plain text is forwarded to the
         underlying stream.

  Filters \c ESC [... and SGR parameter sequences. Output is line-buffered: filtered characters are accumulated in a
  fixed buffer and written to the underlying stream on newline, when the buffer is full, or on sync().
  Used for test output on GBA where ANSI is not supported.
*/
class GbaFilteringStreamBuf final : public std::streambuf {
public:
  GbaFilteringStreamBuf() noexcept;

protected:
  /// Accumulates character in line buffer; flushes on newline or buffer full.
  int_type overflow(int_type c) noexcept override;

  /// Filters and accumulates buffer; flushes on newline or buffer full.
  std::streamsize xsputn(const char * s, std::streamsize n) noexcept override;

  /// Flushes the line buffer to the underlying stream, then flushes the stream.
  int sync() noexcept override;

private:
  static constexpr std::size_t kLineBufferSize = 1024;

  /// FSM states for parsing ESC and CSI sequences.
  enum class State { Normal, Escape, Csi };

  State _state{State::Normal};

  char _lineBuffer[kLineBufferSize];
  std::size_t _lineBufferDataSize{0};

  [[nodiscard]] bool _shouldPass(int_type c) noexcept;

  void _flushLine() noexcept;

  bool _mGbaDetected = false;
};

#endif // TEST_GBA_FILTERING_STREAM_BUF_HPP_
