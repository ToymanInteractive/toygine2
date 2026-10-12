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
  \file   md_clock.hpp
  \brief  Scanline timestamps from the Mega Drive frame counter and VDP V counter.

  Defines \ref toy::benchmark::detail::VdpSample and the arithmetic that turns one into a timestamp in scanlines. The
  arithmetic lives outside the platform file so the desktop unit tests can compile it.

  \note Included by the Mega Drive platform file and by the runner's unit test.
*/

#ifndef INCLUDE_BENCHMARKS_RUNNER_MD_CLOCK_HPP_
#define INCLUDE_BENCHMARKS_RUNNER_MD_CLOCK_HPP_

#include <cstdint>

namespace toy::benchmark::detail {

/// Line on which the VDP raises the vertical interrupt in 224-line mode; the V counter reads the same value there.
inline constexpr std::uint8_t c_verticalInterruptLine = 0xE0;

/// Scanlines in an NTSC frame.
inline constexpr std::uint32_t c_ntscLinesPerFrame = 262;

/// Scanlines in a PAL frame.
inline constexpr std::uint32_t c_palLinesPerFrame = 313;

/// Length of an NTSC scanline in picoseconds: 3420 cycles of the 53.693175 MHz master clock.
inline constexpr std::uint64_t c_ntscPicosecondsPerLine = 63'695'246;

/// Length of a PAL scanline in picoseconds: 3420 cycles of the 53.203424 MHz master clock.
inline constexpr std::uint64_t c_palPicosecondsPerLine = 64'281'577;

/// One reading of the frame counter and the VDP, taken together for a timestamp.
struct VdpSample final {
  std::uint32_t frames;        ///< Vertical interrupts handled so far.
  std::uint8_t  vCounter;      ///< Low byte of the VDP V counter.
  bool          verticalBlank; ///< Vertical blanking flag of the VDP status.
};

/*!
  \brief Tells whether a sample's timestamp is pinned to the vertical interrupt.

  \param sample  Reading to classify.

  \return \c true inside vertical blanking or at a V counter of \c 0xE0 and above, where the 8-bit counter repeats
          values within a frame.

  \note SGDK pins its sub-tick inside vertical blanking for the same reason.

  \sa needsSettling()
*/
[[nodiscard]] constexpr bool isPinned(const VdpSample & sample) noexcept;

/*!
  \brief Tells whether a sample must be retaken once its line is over.

  \param sample  Reading to classify.

  \return \c true on the interrupt line, where the handler may not have run yet and \a sample.frames may be one short.
*/
[[nodiscard]] constexpr bool needsSettling(const VdpSample & sample) noexcept;

/*!
  \brief Converts a sample to scanlines since power-on.

  \param sample         Reading during which the frame counter did not change.
  \param linesPerFrame  \ref toy::benchmark::detail::c_ntscLinesPerFrame or
                        \ref toy::benchmark::detail::c_palLinesPerFrame.

  \return Frames times lines per frame, plus the lines since the last vertical interrupt, which a pinned sample counts
          as \c 0. Wraps modulo 2^32.

  \pre needsSettling() is \c false for \a sample.

  \note The timestamp of a pinned sample lags the true time by up to 37 lines on NTSC and 88 on PAL.
*/
[[nodiscard]] constexpr std::uint32_t lineTimestamp(const VdpSample & sample, std::uint32_t linesPerFrame) noexcept;

} // namespace toy::benchmark::detail

#include "md_clock.inl"

#endif // INCLUDE_BENCHMARKS_RUNNER_MD_CLOCK_HPP_
