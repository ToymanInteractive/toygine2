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
  \file   chrono_clock_source_stub.cpp
  \brief  Implementations for \ref toy::chrono::ClockSource and \ref toy::chrono::SteadyClock, stub backend.

  Delegates all tick reads to \c std::chrono::steady_clock and normalises the result to nanoseconds. Used on platforms
  without a dedicated hardware counter backend (NDS, 3DS, Switch).

  \warning Performance is determined by the \c std::chrono::steady_clock implementation of the host OS; no syscall-free
           guarantee is provided.
*/

#include <chrono>

#include "core.hpp"

namespace toy::chrono {

static ClockSource * g_active = nullptr;

inline int64_t readNs() noexcept {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch())
    .count();
}

ClockSource::ClockSource() noexcept {
  assert_message(g_active == nullptr, "A ClockSource is already active; only one instance per process is allowed");

  _frequency = 1'000'000'000;

  g_active = this;
}

ClockSource::~ClockSource() {
  g_active = nullptr;
}

int64_t ClockSource::nowTicks() const noexcept {
  return readNs();
}

SteadyClock::rep SteadyClock::nowTicks() noexcept {
  return readNs();
}

SteadyClock::rep SteadyClock::frequency() noexcept {
  return g_active->frequency();
}

SteadyClock::time_point SteadyClock::now() noexcept {
  return time_point{duration{readNs()}};
}

} // namespace toy::chrono
