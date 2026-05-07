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
  \file   chrono_countdown_timer.inl
  \brief  Inline implementations for \ref toy::chrono::CountdownTimer.

  \note Included by core.hpp only; do not include this file directly.
*/

namespace toy::chrono {

inline CountdownTimer::CountdownTimer(duration timeout) noexcept
  : _deadlineTicks{SteadyClock::nowTicks() + timeout.count()} {}

inline CountdownTimer::duration CountdownTimer::remaining() const noexcept {
  const auto now = SteadyClock::nowTicks();
  return now >= _deadlineTicks ? duration{0} : duration{_deadlineTicks - now};
}

inline bool CountdownTimer::expired() const noexcept {
  return SteadyClock::nowTicks() >= _deadlineTicks;
}

inline void CountdownTimer::restart(duration timeout) noexcept {
  _deadlineTicks = SteadyClock::nowTicks() + timeout.count();
}

} // namespace toy::chrono
