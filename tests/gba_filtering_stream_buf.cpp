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
  \file   gba_filtering_stream_buf.cpp
  \brief  Definitions for \c GbaFilteringStreamBuf declared in \c gba_filtering_stream_buf.hpp.
*/

#include "gba_filtering_stream_buf.hpp"

namespace ansi_filter {

constexpr std::streambuf::int_type kEsc         = 0x1b;
constexpr std::streambuf::int_type kCsiFinalMin = 0x40;
constexpr std::streambuf::int_type kCsiFinalMax = 0x7E;

} // namespace ansi_filter

/// mGBA log level: fatal (0x100).
static constexpr uint16_t mGbaLogLevelFatal = 0x100;
/// mGBA log level: error (0x101).
static constexpr uint16_t mGbaLogLevelError = 0x101;
/// mGBA log level: warning (0x102).
static constexpr uint16_t mGbaLogLevelWarn  = 0x102;
/// mGBA log level: info (0x103).
static constexpr uint16_t mGbaLogLevelInfo  = 0x103;
/// mGBA log level: debug (0x104).
static constexpr uint16_t mGbaLogLevelDebug = 0x104;

static bool mGbaDetect() noexcept {
  // mGBA debug register
  *reinterpret_cast<volatile uint16_t *>(0x04FFF780) = 0xC0DE;

  return *reinterpret_cast<volatile uint16_t *>(0x04FFF780) == 0x1DEA;
}

GbaFilteringStreamBuf::GbaFilteringStreamBuf() noexcept {
#ifdef __GBA__
  _mGbaDetected = mGbaDetect();
#endif
}

std::streambuf::int_type GbaFilteringStreamBuf::overflow(int_type c) noexcept {
  if (traits_type::eq_int_type(c, traits_type::eof()) || !_shouldPass(c))
    return traits_type::not_eof(c);

  if (_lineBufferDataSize == kLineBufferSize)
    _flushLine();

  _lineBuffer[_lineBufferDataSize++] = traits_type::to_char_type(c);

  if (c == traits_type::to_int_type('\n'))
    _flushLine();

  if (!std::cout)
    return traits_type::eof();

  return traits_type::not_eof(c);
}

std::streamsize GbaFilteringStreamBuf::xsputn(const char * s, std::streamsize n) noexcept {
  std::streamsize processed = 0;

  for (std::streamsize i = 0; i < n; ++i) {
    if (const auto c = traits_type::to_int_type(s[i]); _shouldPass(c)) {
      if (_lineBufferDataSize == kLineBufferSize)
        _flushLine();

      _lineBuffer[_lineBufferDataSize++] = s[i];

      if (c == traits_type::to_int_type('\n'))
        _flushLine();

      if (!std::cout)
        return processed;
    }
    ++processed;
  }

  return processed;
}

int GbaFilteringStreamBuf::sync() noexcept {
  _flushLine();

  std::cout.flush();

  return std::cout ? 0 : -1;
}

bool GbaFilteringStreamBuf::_shouldPass(int_type c) noexcept {
  if (c == traits_type::eof())
    return false;

  switch (_state) {
    case State::Escape:
      if (c == '[') {
        _state = State::Csi;

        return false;
      }

      _state = State::Normal;

      return true;

    case State::Csi:
      if (c >= ansi_filter::kCsiFinalMin && c <= ansi_filter::kCsiFinalMax)
        _state = State::Normal;

      return false;

    case State::Normal:
      if (c == ansi_filter::kEsc) {
        _state = State::Escape;

        return false;
      }

      return true;

    default:
      return true;
  }
}

void GbaFilteringStreamBuf::_flushLine() noexcept {
  if (_lineBufferDataSize == 0)
    return;

  if (_mGbaDetected) {
    *reinterpret_cast<volatile uint16_t *>(0x04FFF700) = mGbaLogLevelInfo;

    char *       dst = reinterpret_cast<char *>(0x04FFF600); // MGBA Log String Register
    const char * src = _lineBuffer;
    while (*src) {
      *dst++ = *src++;
    }
    *dst = '\0';
  }

  if (_lineBufferDataSize <= 30) {
    std::cout.write(_lineBuffer, static_cast<std::streamsize>(_lineBufferDataSize));
  } else {
    std::cout.write(_lineBuffer, 27);
    std::cout << "...\n";
  }

  _lineBufferDataSize = 0;
}
