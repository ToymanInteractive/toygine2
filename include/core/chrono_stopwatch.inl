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
  \file   chrono_stopwatch.inl
  \brief  Inline implementations for \ref toy::chrono::Stopwatch.

  \note Included by core.hpp only; do not include this file directly.
*/

namespace toy::chrono {

inline Stopwatch::duration Stopwatch::elapsed() const noexcept {
  return duration{SteadyClock::nowTicks() - _startTicks};
}

inline Stopwatch::rep Stopwatch::elapsedTicks() const noexcept {
  return SteadyClock::nowTicks() - _startTicks;
}

inline void Stopwatch::reset() noexcept {
  _startTicks = SteadyClock::nowTicks();
}

} // namespace toy::chrono
