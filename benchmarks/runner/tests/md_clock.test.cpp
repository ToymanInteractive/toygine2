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
  \file   md_clock.test.cpp
  \brief  Unit tests for the Mega Drive scanline timestamps.
*/

#include <cstdint>

#include <doctest/doctest.h>

#include "md_clock.hpp"

namespace {

using toy::benchmark::detail::isPinned;
using toy::benchmark::detail::lineTimestamp;
using toy::benchmark::detail::needsSettling;
using toy::benchmark::detail::ntscLinesPerFrame;
using toy::benchmark::detail::palLinesPerFrame;
using toy::benchmark::detail::VdpSample;
using toy::benchmark::detail::verticalInterruptLine;

// Builds the sample of one line, with V counter values from Kabuto's hardware notes for 224-line mode.
[[nodiscard]] constexpr VdpSample sampleAt(std::uint32_t line, std::uint32_t linesPerFrame,
                                           std::uint32_t lastLineBeforeJump) noexcept {
  const std::uint32_t inFrame = line % linesPerFrame;

  // The interrupt on line 224 has run by the time the next sample is taken.
  const std::uint32_t frames = (line + linesPerFrame - verticalInterruptLine) / linesPerFrame;

  return {.frames        = frames,
          .vCounter      = static_cast<std::uint8_t>(inFrame <= lastLineBeforeJump ? inFrame : inFrame - linesPerFrame),
          .verticalBlank = inFrame >= verticalInterruptLine && inFrame != linesPerFrame - 1};
}

// Replays two frames line by line and checks that no timestamp is lower than the one before it.
[[nodiscard]] constexpr bool walkNeverStepsBack(std::uint32_t linesPerFrame,
                                                std::uint32_t lastLineBeforeJump) noexcept {
  std::uint32_t previous = 0;

  for (std::uint32_t line = 0; line < 2 * linesPerFrame; ++line) {
    const VdpSample sample = sampleAt(line, linesPerFrame, lastLineBeforeJump);

    if (needsSettling(sample))
      continue;

    const std::uint32_t timestamp = lineTimestamp(sample, linesPerFrame);

    if (timestamp < previous)
      return false;

    previous = timestamp;
  }

  return true;
}

// Replays two frames and checks each timestamp against the line it was taken on.
[[nodiscard]] constexpr bool walkStaysOnTime(std::uint32_t linesPerFrame, std::uint32_t lastLineBeforeJump) noexcept {
  // Line 0 of the walk follows a vertical blank, so its exact timestamp is the blank's length.
  const std::uint32_t blankLength = linesPerFrame - verticalInterruptLine;

  for (std::uint32_t line = 0; line < 2 * linesPerFrame; ++line) {
    const VdpSample sample = sampleAt(line, linesPerFrame, lastLineBeforeJump);

    if (needsSettling(sample))
      continue;

    const std::uint32_t exact     = line + blankLength;
    const std::uint32_t timestamp = lineTimestamp(sample, linesPerFrame);

    // An active line gets its exact timestamp; a pinned one lags, but by less than the whole blank.
    if (isPinned(sample) ? timestamp > exact || exact - timestamp >= blankLength : timestamp != exact)
      return false;
  }

  return true;
}

constexpr VdpSample c_activeLine{.frames = 3, .vCounter = 0x10, .verticalBlank = false};
constexpr VdpSample c_repeatedNtscValue{.frames = 3, .vCounter = 0xE5, .verticalBlank = true};
constexpr VdpSample c_repeatedPalValue{.frames = 2, .vCounter = 0x01, .verticalBlank = true};
constexpr VdpSample c_lastPalLine{.frames = 2, .vCounter = 0xFF, .verticalBlank = false};
constexpr VdpSample c_interruptLine{.frames = 0, .vCounter = 0xE0, .verticalBlank = true};
constexpr VdpSample c_lineAfterInterrupt{.frames = 1, .vCounter = 0xE1, .verticalBlank = true};

} // namespace

// An active line counts the vertical blank before it plus its own number.
TEST_CASE("benchmark::detail/line_timestamp/active_line_counts_from_the_interrupt") {
  const std::uint32_t expected = 3 * ntscLinesPerFrame + (ntscLinesPerFrame - verticalInterruptLine) + 0x10;

  CHECK(lineTimestamp(c_activeLine, ntscLinesPerFrame) == expected);

  static_assert(lineTimestamp(c_activeLine, ntscLinesPerFrame)
                  == 3 * ntscLinesPerFrame + (ntscLinesPerFrame - verticalInterruptLine) + 0x10,
                "an active line counts the frames, the vertical blank and its own number");
}

// In vertical blanking, and on the last line once the blank flag clears, the timestamp stays on the interrupt.
TEST_CASE("benchmark::detail/line_timestamp/vertical_blank_pins_to_the_interrupt") {
  CHECK(lineTimestamp(c_repeatedNtscValue, ntscLinesPerFrame) == 3 * ntscLinesPerFrame);
  CHECK(lineTimestamp(c_repeatedPalValue, palLinesPerFrame) == 2 * palLinesPerFrame);
  CHECK(lineTimestamp(c_lastPalLine, palLinesPerFrame) == 2 * palLinesPerFrame);

  static_assert(lineTimestamp(c_repeatedNtscValue, ntscLinesPerFrame) == 3 * ntscLinesPerFrame,
                "a value NTSC repeats inside vertical blanking is pinned to the interrupt");
  static_assert(lineTimestamp(c_repeatedPalValue, palLinesPerFrame) == 2 * palLinesPerFrame,
                "a low value PAL repeats inside vertical blanking is pinned to the interrupt");
  static_assert(lineTimestamp(c_lastPalLine, palLinesPerFrame) == 2 * palLinesPerFrame,
                "the last line, with the blank flag already clear, is pinned by its counter value");
}

// Across two frames of either region, the timestamp never steps back.
TEST_CASE("benchmark::detail/line_timestamp/never_steps_back_through_frames") {
  CHECK(walkNeverStepsBack(ntscLinesPerFrame, 0xEA));
  CHECK(walkNeverStepsBack(palLinesPerFrame, 0x102));

  static_assert(walkNeverStepsBack(ntscLinesPerFrame, 0xEA), "NTSC timestamps never step back across a frame");
  static_assert(walkNeverStepsBack(palLinesPerFrame, 0x102), "PAL timestamps never step back across a frame");
}

// Active lines get their exact timestamp; a pinned line lags by at most 37 lines on NTSC and 88 on PAL.
TEST_CASE("benchmark::detail/line_timestamp/stays_within_the_blank_of_the_true_line") {
  CHECK(walkStaysOnTime(ntscLinesPerFrame, 0xEA));
  CHECK(walkStaysOnTime(palLinesPerFrame, 0x102));

  static_assert(walkStaysOnTime(ntscLinesPerFrame, 0xEA), "NTSC timestamps are exact or lag within the blank");
  static_assert(walkStaysOnTime(palLinesPerFrame, 0x102), "PAL timestamps are exact or lag within the blank");
}

// Only the interrupt line asks for a retake.
TEST_CASE("benchmark::detail/needs_settling/only_on_the_interrupt_line") {
  CHECK(needsSettling(c_interruptLine));
  CHECK_FALSE(needsSettling(c_lineAfterInterrupt));
  CHECK_FALSE(needsSettling(c_activeLine));

  static_assert(needsSettling(c_interruptLine), "the interrupt line may precede its handler");
  static_assert(!needsSettling(c_lineAfterInterrupt), "the handler has run by the next line");
  static_assert(!needsSettling(c_activeLine), "an active line is never retaken");
}
