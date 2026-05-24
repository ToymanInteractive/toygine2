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
  \file   utils.benchmark.cpp
  \brief  Implementation of picobench benchmarks for utility functions in the core module.
*/

#include "core.hpp"
#include "picobench/picobench.hpp"

namespace toy {

static void itoaInt8(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    char buf[32];
    itoa(buf, size(buf), int8_t{42});
    result += static_cast<unsigned char>(buf[0]);
  }
  state.set_result(result);
}

static void itoaInt16(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    char buf[32];
    itoa(buf, size(buf), int16_t{-1234});
    result += static_cast<unsigned char>(buf[0]);
  }
  state.set_result(result);
}

static void itoaInt32(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    char buf[32];
    itoa(buf, size(buf), int32_t{12'345'678});
    result += static_cast<unsigned char>(buf[0]);
  }
  state.set_result(result);
}

static void itoaInt64(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    char buf[32];
    itoa(buf, size(buf), int64_t{-9'223'372'036'854'775'807LL});
    result += static_cast<unsigned char>(buf[0]);
  }
  state.set_result(result);
}

static void utoaUint8Base10(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    char buf[32];
    utoa(buf, size(buf), uint8_t{200}, 10U);
    result += static_cast<unsigned char>(buf[0]);
  }
  state.set_result(result);
}

static void utoaUint16Base10(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    char buf[32];
    utoa(buf, size(buf), uint16_t{65'535}, 10U);
    result += static_cast<unsigned char>(buf[0]);
  }
  state.set_result(result);
}

static void utoaUint32Base10(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    char buf[32];
    utoa(buf, size(buf), uint32_t{4'000'000'000U}, 10U);
    result += static_cast<unsigned char>(buf[0]);
  }
  state.set_result(result);
}

static void utoaUint64Base10(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    char buf[32];
    utoa(buf, size(buf), uint64_t{18'446'744'073'709'551'615ULL}, 10U);
    result += static_cast<unsigned char>(buf[0]);
  }
  state.set_result(result);
}

static void utoaUint32Base16(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    char buf[32];
    utoa(buf, size(buf), uint32_t{0xDEADBEEFU}, 16U);
    result += static_cast<unsigned char>(buf[0]);
  }
  state.set_result(result);
}

static void utoaUint64Base16(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    char buf[32];
    utoa(buf, size(buf), uint64_t{0xFFFFFFFFFFFFFFFFULL}, 16U);
    result += static_cast<unsigned char>(buf[0]);
  }
  state.set_result(result);
}

static void ftoaFloatDefaultPrecision(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    char buf[64];
    ftoa(buf, size(buf), 3.14159265F);
    result += static_cast<unsigned char>(buf[0]);
  }
  state.set_result(result);
}

static void ftoaFloatCustomPrecision(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    char buf[64];
    ftoa(buf, size(buf), -123.456F, 4);
    result += static_cast<unsigned char>(buf[0]);
  }
  state.set_result(result);
}

static void ftoaDoubleDefaultPrecision(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    char buf[64];
    ftoa(buf, size(buf), 3.14159265358979);
    result += static_cast<unsigned char>(buf[0]);
  }
  state.set_result(result);
}

static void ftoaDoubleCustomPrecision(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    char buf[64];
    ftoa(buf, size(buf), -12345.6789012345, 8);
    result += static_cast<unsigned char>(buf[0]);
  }
  state.set_result(result);
}

static void formatNumberStringLongComma(picobench::state & state) noexcept {
  static constexpr auto c_initialBuf = []() consteval noexcept {
    const char * src = "123456789012345";

    std::array<char, 32> arr{};
    size_t               i = 0;
    while (src[i] != '\0') {
      arr[i] = src[i];
      ++i;
    }

    return arr;
  }();

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i) {
    auto buf = c_initialBuf;
    formatNumberString(buf.data(), buf.size(), ",");
    result += static_cast<unsigned char>(buf[0]);
  }
  state.set_result(result);
}

static void highestBitHighBits(picobench::state & state) noexcept {
  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i)
    result += highestBit(0x8000000000000000ULL);

  state.set_result(result);
}

PICOBENCH_SUITE("toy::utils");
PICOBENCH(itoaInt8);
PICOBENCH(itoaInt16);
PICOBENCH(itoaInt32);
PICOBENCH(itoaInt64);
PICOBENCH(utoaUint8Base10);
PICOBENCH(utoaUint16Base10);
PICOBENCH(utoaUint32Base10);
PICOBENCH(utoaUint64Base10);
PICOBENCH(utoaUint32Base16);
PICOBENCH(utoaUint64Base16);
PICOBENCH(ftoaFloatDefaultPrecision);
PICOBENCH(ftoaFloatCustomPrecision);
PICOBENCH(ftoaDoubleDefaultPrecision);
PICOBENCH(ftoaDoubleCustomPrecision);
PICOBENCH(formatNumberStringLongComma);
PICOBENCH(highestBitHighBits);

} // namespace toy
