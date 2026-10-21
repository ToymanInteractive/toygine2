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

#ifndef SAMPLES_TETRIS_TERMINAL_SCREEN_HPP_
#define SAMPLES_TETRIS_TERMINAL_SCREEN_HPP_

#include <cstdint>

namespace terminal {

struct Size {
  int32_t columns;
  int32_t rows;
};

enum class EnterResult : uint8_t {
  ok,
  notATerminal,
  tooSmall,
  setupFailed,
};

inline constexpr int32_t c_maxColumns = 160;
inline constexpr int32_t c_maxRows    = 60;
inline constexpr int32_t c_minColumns = 48;
inline constexpr int32_t c_minRows    = 24;

Size        querySize() noexcept;
EnterResult enter() noexcept;
void        leave() noexcept;

} // namespace terminal

#endif // SAMPLES_TETRIS_TERMINAL_SCREEN_HPP_
