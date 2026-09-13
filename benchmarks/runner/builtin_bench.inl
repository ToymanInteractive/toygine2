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
  \file   builtin_bench.inl
  \brief  Inline implementations for \ref toy::benchmark::Bench and toy::benchmark::doNotOptimizeAway().

  \note Included by builtin_bench.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_BENCHMARKS_RUNNER_BUILTIN_BENCH_INL_
#define INCLUDE_BENCHMARKS_RUNNER_BUILTIN_BENCH_INL_

namespace toy::benchmark {

template <typename T>
void doNotOptimizeAway(T && value) noexcept {
#if defined(__GNUC__)
  // The value is an input of an empty instruction, so the computation behind it has to happen and reach a register or
  // memory. The test is __GNUC__ rather than _MSC_VER: clang-cl defines the latter but understands the assembly.
  asm volatile("" : : "r,m"(value) : "memory");
#else
  // MSVC has no inline assembly on x64 or Arm64. A volatile read of the first byte is an observable use of the value;
  // taking its address alone is not.
  static_cast<void>(*reinterpret_cast<const volatile char *>(&value));
#endif
}

inline Bench::Bench(const Clock & clock, const Output & output, const char * title) noexcept
  : _clock{clock}
  , _output{output}
  , _title{title} {}

inline Bench & Bench::unit(const char * unitName) noexcept {
  _unit = unitName;

  return *this;
}

template <typename Callable>
  requires std::invocable<Callable &>
Bench & Bench::run(const char * name, Callable && operation) {
  // The clock is read at the epoch edges only, so the loop between the two reads holds the operation alone.
  const auto timeEpoch = [this, &operation](std::uint32_t iterations) {
    const std::uint32_t start = _clock.now(_clock.clockData);

    for (std::uint32_t call = 0; call < iterations; ++call)
      operation();

    return static_cast<std::uint32_t>(_clock.now(_clock.clockData) - start);
  };

  const auto iterations = calibrate(timeEpoch);
  if (iterations == 0) {
    writeUncalibrated(name);

    return *this;
  }

  std::array<std::uint32_t, detail::c_epochCount> epochs{};

  for (std::uint32_t & epoch : epochs)
    epoch = timeEpoch(iterations);

  writeRow(name, iterations, epochs);

  return *this;
}

template <typename Epoch>
  requires std::invocable<Epoch &, std::uint32_t>
std::uint32_t Bench::calibrate(Epoch & timeEpoch) const {
  // The doubling stops at the limit instead of stepping over it, so no epoch runs more calls than the platform allows.
  for (std::uint32_t iterations = 1; iterations <= _clock.maxIterations;) {
    if (timeEpoch(iterations) >= _clock.minEpochTicks)
      return iterations;

    if (iterations == _clock.maxIterations)
      break;

    iterations = iterations > _clock.maxIterations / 2 ? _clock.maxIterations : iterations * 2;
  }

  return 0;
}

inline void Bench::writeRow(const char * name, std::uint32_t iterations,
                            const std::array<std::uint32_t, detail::c_epochCount> & epochs) noexcept {
  detail::LineBuffer line{_output};

  line.addText("#csv ");
  line.addText(_title);
  line.addText(",");
  line.addText(name);
  line.addText(",");
  line.addText(_unit);
  line.addText(",");
  line.addDecimal(_clock.picosecondsPerTick);
  line.addText(",");
  line.addDecimal(detail::c_epochCount);

  // The calls repeat per epoch because the host reads a pair at a time and never assumes they match.
  for (const std::uint32_t epoch : epochs) {
    line.addText(",");
    line.addDecimal(iterations);
    line.addText(",");
    line.addDecimal(epoch);
  }

  line.flush();
}

inline void Bench::writeUncalibrated(const char * name) noexcept {
  detail::LineBuffer line{_output};

  // The line carries both numbers: calibration ends the same way for a standing timer and for a limit too small.
  line.addText("# ");
  line.addText(name);
  line.addText(": no epoch reached ");
  line.addDecimal(_clock.minEpochTicks);
  line.addText(" ticks within ");
  line.addDecimal(_clock.maxIterations);
  line.addText(" calls");
  line.flush();
}

} // namespace toy::benchmark

#endif // INCLUDE_BENCHMARKS_RUNNER_BUILTIN_BENCH_INL_
