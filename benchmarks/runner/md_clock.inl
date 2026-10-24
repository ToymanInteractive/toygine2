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
  \file   md_clock.inl
  \brief  Inline implementations for the Mega Drive scanline timestamps.

  \note Included by md_clock.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_BENCHMARKS_RUNNER_MD_CLOCK_INL_
#define INCLUDE_BENCHMARKS_RUNNER_MD_CLOCK_INL_

namespace toy::benchmark::detail {

constexpr bool isPinned(const VdpSample & sample) noexcept {
  return sample.verticalBlank || sample.vCounter >= verticalInterruptLine;
}

constexpr bool needsSettling(const VdpSample & sample) noexcept {
  return isPinned(sample) && sample.vCounter == verticalInterruptLine;
}

constexpr std::uint32_t lineTimestamp(const VdpSample & sample, std::uint32_t linesPerFrame) noexcept {
  // An active line follows the whole vertical blank that began with the last interrupt.
  const std::uint32_t sinceInterrupt = isPinned(sample) ? 0 : linesPerFrame - verticalInterruptLine + sample.vCounter;

  return sample.frames * linesPerFrame + sinceInterrupt;
}

} // namespace toy::benchmark::detail

#endif // INCLUDE_BENCHMARKS_RUNNER_MD_CLOCK_INL_
