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
  \file   hashes.benchmark.cpp
  \brief  Implementation of picobench benchmarks for toy::crc8(), toy::crc16(), and toy::crc32().
*/

#include "core.hpp"
#include "picobench/picobench.hpp"

namespace toy {

static void crc8ShortString(picobench::state & state) noexcept {
  static constexpr char s[] = "Hello";

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i)
    result += crc8(s, sizeof(s) - 1);

  state.set_result(result);
}

static void crc8MediumString(picobench::state & state) noexcept {
  static constexpr char s[] = "Toygine2 - Free 2D/3D game engine.";

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i)
    result += crc8(s, sizeof(s) - 1);

  state.set_result(result);
}

static void crc16MediumString(picobench::state & state) noexcept {
  static constexpr char s[] = "Toygine2 - Free 2D/3D game engine.";

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i)
    result += crc16(s, sizeof(s) - 1);

  state.set_result(result);
}

static void crc32MediumString(picobench::state & state) noexcept {
  static constexpr char s[] = "Toygine2 - Free 2D/3D game engine.";

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i)
    result += crc32(s, sizeof(s) - 1);

  state.set_result(result);
}

static void crc32Buffer512(picobench::state & state) noexcept {
  static constexpr auto payload = []() consteval noexcept {
    array<unsigned char, 512> arr{};

    for (size_t i = 0; i < 512; ++i)
      arr[i] = static_cast<unsigned char>(i ^ 0xA5U);

    return arr;
  }();

  size_t           result{0};
  picobench::scope scope(state);
  for (int i = 0; i < state.iterations(); ++i)
    result += crc32(payload.data(), payload.size());

  state.set_result(result);
}

PICOBENCH_SUITE("toy::hashes");
PICOBENCH(crc8ShortString);
PICOBENCH(crc8MediumString);
PICOBENCH(crc16MediumString);
PICOBENCH(crc32MediumString);
PICOBENCH(crc32Buffer512);

} // namespace toy
