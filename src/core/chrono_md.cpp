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
  \file   chrono_md.cpp
  \brief  Implementations of \ref toy::chrono::SteadyClock, \ref toy::chrono::SystemClock, and
          \ref toy::chrono::Timestamp over the V-Int frame counter.

  \note Before the application enables the VDP vertical interrupt and runs initialize(), the frame counter stays at
        zero; now() and Timestamp::now() assert on that state, while nowTicks() and frequency() stay honest.
*/

#include "core.hpp"

namespace toy::chrono {

namespace {

/// V-Int frames counted since power-on, incremented by the interrupt handler
volatile uint32_t frameCount = 0;

/// Region of the console, fixed by initialize(); now() branches its scale on it, so both multiplies stay constant
bool isNtsc = true;

/// Set by initialize(); gates the reading-side asserts
bool initialized = false;

/// Raw frame count the Timestamp origin counts from, taken by initialize()
SteadyClock::rep timestampOrigin = 0;

/// Highest interrupt mask that still lets the level-6 V-Int through
constexpr unsigned int interruptMaskMax = 5;

/// Frames per second of an NTSC console
constexpr SteadyClock::rep ntscFramesPerSecond = 60;

/// Frames per second of a PAL console
constexpr SteadyClock::rep palFramesPerSecond = 50;

/// Steady clock ticks in one frame on NTSC
constexpr SteadyClock::rep ntscTicksPerFrame = 5;

/// Steady clock ticks in one frame on PAL
constexpr SteadyClock::rep palTicksPerFrame = 6;

/// V-Int handler body, extends the frame count the clock reads
void onVerticalInterrupt() noexcept {
  frameCount = frameCount + 1;
}

} // namespace

void initialize() noexcept {
  assert_message(ClownMDSDK::MainCPU::M68k::GetInterruptMask() <= interruptMaskMax,
                 "chrono::initialize: level 6 is masked, enable the VDP vertical interrupt and unmask it first");

  if (initialized) {
    // The counter keeps running: a re-initialization only re-fixes the timestamp origin.
    timestampOrigin = SteadyClock::nowTicks();
    return;
  }

  ClownMDSDK::MainCPU::M68k::DisableInterruptsTemporarily([]() noexcept {
    // A V-Int held off here lands after the restore as frame 0 or 1, so no carry can be pending.
    frameCount      = 0;
    timestampOrigin = 0;
    isNtsc          = !ClownMDSDK::MainCPU::Unsafe::IsPAL();
    initialized     = true;
  });
}

SteadyClock::time_point SteadyClock::now() noexcept {
  assert_message(initialized, "SteadyClock::now: initialize() has not run");

  const auto ticks = isNtsc ? nowTicks() * ntscTicksPerFrame : nowTicks() * palTicksPerFrame;

  return time_point{duration{ticks}};
}

SteadyClock::rep SteadyClock::nowTicks() noexcept {
  for (;;) {
    const auto frames = frameCount;

    // The handler may land between the two halves of a 32-bit read; equal consecutive reads are whole.
    if (frameCount == frames)
      return static_cast<SteadyClock::rep>(frames);
  }
}

SteadyClock::rep SteadyClock::frequency() noexcept {
  return ClownMDSDK::MainCPU::Unsafe::IsPAL() ? palFramesPerSecond : ntscFramesPerSecond;
}

CalendarTime SystemClock::now() noexcept {
  const auto uptime = duration_cast<Duration<int64_t, std::milli>>(SteadyClock::now().time_since_epoch());

  return CalendarTime::fromUptime(uptime);
}

Timestamp Timestamp::now() noexcept {
  assert_message(initialized, "Timestamp::now: initialize() has not run");

  const auto delta           = SteadyClock::nowTicks() - timestampOrigin;
  const auto framesPerSecond = SteadyClock::frequency();

  const int64_t milliseconds = delta / framesPerSecond * 1000 + delta % framesPerSecond * 1000 / framesPerSecond;

  return fromMilliseconds(milliseconds);
}

} // namespace toy::chrono

// Declared by clownmdsdk.h with C linkage and the interrupt attribute; the definition only counts frames.
void _VerticalInterruptHandler() {
  toy::chrono::onVerticalInterrupt();
}
