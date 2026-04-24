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
  \file   chrono_clock_source_x64.cpp
  \brief  Implementations for \ref toy::chrono::ClockSource and \ref toy::chrono::SteadyClock on x86-64.

  Uses the invariant TSC via \c __rdtsc(). Frequency is calibrated once at construction against
  \c std::chrono::steady_clock using a burn-in round followed by five 20 ms measurement windows; the highest measured
  value is kept (minimum wall-clock overshoot gives the most accurate ratio).
*/

#include <chrono>
#include <thread>

#if defined(_MSC_VER)
#include <intrin.h>
#else
#include <cpuid.h>
#include <x86intrin.h>
#endif

#include "core.hpp"

namespace toy::chrono {

static ClockSource * g_active{nullptr};
static int64_t       g_tscBase{0};
static double        g_nsPerTick{0.0};

inline int64_t readTsc() noexcept {
  return static_cast<int64_t>(__rdtsc());
}

bool hasInvariantTsc() noexcept {
  uint32_t eax{0};
  uint32_t ebx{0};
  uint32_t ecx{0};
  uint32_t edx{0};

#if defined(_MSC_VER)
  int info[4];
  __cpuid(info, static_cast<int>(0x80000007u));
  edx = static_cast<uint32_t>(info[3]);
#else
  __get_cpuid(0x80000007u, &eax, &ebx, &ecx, &edx);
#endif

  (void)eax;
  (void)ebx;
  (void)ecx;

  return (edx & (1u << 8u)) != 0u;
}

int64_t calibrateTscHz() noexcept {
  constexpr int  c_rounds = 5;
  constexpr auto c_window = std::chrono::milliseconds(20);

  // Burn-in: one discarded round to let the CPU reach a stable frequency.
  readTsc();
  std::this_thread::sleep_for(c_window);
  readTsc();

  int64_t bestHz = 0;
  for (int i = 0; i < c_rounds; ++i) {
    const auto    wallStart = std::chrono::steady_clock::now();
    const int64_t tscStart  = readTsc();
    std::this_thread::sleep_for(c_window);
    const int64_t tscEnd  = readTsc();
    const auto    wallEnd = std::chrono::steady_clock::now();
    const int64_t wallNs  = std::chrono::duration_cast<std::chrono::nanoseconds>(wallEnd - wallStart).count();
    if (wallNs > 0) {
      const int64_t hz = (tscEnd - tscStart) * 1'000'000'000LL / wallNs;
      if (hz > bestHz)
        bestHz = hz;
    }
  }
  return bestHz;
}

ClockSource::ClockSource() {
  assert_message(hasInvariantTsc(), "invariant TSC required; this CPU is not supported by the x64 chrono backend");
  assert_message(g_active == nullptr, "a ClockSource is already active; only one instance per process is allowed");

  _frequency = calibrateTscHz();

  g_nsPerTick = 1e9 / static_cast<double>(_frequency);
  g_tscBase   = readTsc();
  g_active    = this;
}

ClockSource::~ClockSource() {
  g_active = nullptr;
}

int64_t ClockSource::nowTicks() const noexcept {
  return readTsc() - g_tscBase;
}

SteadyClock::rep SteadyClock::nowTicks() noexcept {
  return readTsc() - g_tscBase;
}

SteadyClock::rep SteadyClock::frequency() noexcept {
  return g_active->frequency();
}

SteadyClock::time_point SteadyClock::now() noexcept {
  const auto ns = static_cast<rep>(static_cast<double>(readTsc() - g_tscBase) * g_nsPerTick);

  return time_point{duration{ns}};
}

} // namespace toy::chrono
