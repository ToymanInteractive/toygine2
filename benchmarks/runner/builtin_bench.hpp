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
  \file   builtin_bench.hpp
  \brief  Benchmark rows of the console runner.

  Defines \ref toy::benchmark::Clock and \ref toy::benchmark::Bench: the timer a console platform supplies, and the
  object a \c BENCHMARK_CASE body writes to where nanobench is unavailable.

  \note Reached through bench.hpp; included directly only by the runner's own headers and by its unit test.
*/

#ifndef INCLUDE_BENCHMARKS_RUNNER_BUILTIN_BENCH_HPP_
#define INCLUDE_BENCHMARKS_RUNNER_BUILTIN_BENCH_HPP_

#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>

#include "line_buffer.hpp"

namespace toy::benchmark {

/*!
  \brief Function reading the platform timer.

  \param clockData  Pointer stored beside the function in \ref toy::benchmark::Clock.

  \return Timer value in ticks; wraps modulo 2^32, so only differences carry meaning.
*/
using now_function_type = std::uint32_t (*)(const void * clockData) noexcept;

/// Timer of a console platform and the limits every row is measured under.
struct Clock final {
  now_function_type now;                ///< Reads the timer; called only at epoch boundaries.
  const void *      clockData;          ///< Passed to \a now unchanged; owned by the platform.
  std::uint64_t     picosecondsPerTick; ///< Length of one tick, the \c ps_per_tick field of a row.
  std::uint32_t     minEpochTicks;      ///< Shortest epoch calibration accepts, in ticks.
  std::uint32_t     maxIterations;      ///< Calls per epoch, at most 2^31, at which calibration gives up.
};

namespace detail {

/// Epochs timed per row: seven pairs and the names fit the 256 bytes an mGBA log message carries.
inline constexpr std::size_t c_epochCount = 7;

} // namespace detail

/*!
  \brief Keeps the compiler from folding away a measured result.

  \tparam T      Type of the result.
  \param  value  Result the barrier consumes.

  \note GCC and Clang take \a value as the input operand of an empty asm statement, so the computation behind it stays.
        The branch is chosen by \c __clang__ and \c __GNUC__, because clang-cl defines \c _MSC_VER while understanding
        the assembly.
  \note MSVC has no inline assembly on x64 or Arm64, so every byte of \a value is read through a \c volatile pointer
        instead. Reading one byte would leave the compiler free to fold whatever affects only the others.
*/
template <typename T>
void doNotOptimizeAway(T && value) noexcept;

/*!
  \class Bench
  \brief One benchmark of the console runner: a CSV row per measured implementation.

  Calibrates the calls per epoch against \ref toy::benchmark::Clock, times seven epochs and prints them as integers:
  the names once, then the calls and the tick count of every epoch. The host script
  \c tools/builder/bmf_from_csv.py divides, takes the median and reports the spread, so a CPU without a floating-point
  unit does none of it. Offers the part of nanobench's table that benchmark bodies call, so one body builds against
  either.

  \section features Key Features

  * **Source-compatible**: unit() and run() match the calls a body makes on nanobench
  * **Allocation-free**: no container, no heap, no \c std::function
  * **Integers only**: ticks and call counts, no division and no fixed point on the device
  * **Timer outside the loop**: the clock is read only at epoch boundaries
  * **Failed calibration reported**: a row that never reaches \c minEpochTicks prints a comment with both limits

  \section usage Usage Example

  \code
  #include "toy_benchmark.hpp"

  BENCHMARK_CASE("core/string_view/find") {
    bench.unit("search");
    bench.run("toy::StringView", [&] { toy::benchmark::doNotOptimizeAway(view.find(needle)); });
  }
  \endcode

  \section performance Performance Characteristics

  * **run()**: calibration doubles the calls until an epoch lasts \c minEpochTicks, then seven epochs of that count
  * **Memory usage**: the clock, the output, three words of state and seven pairs of epoch counters

  \section safety Safety Guarantees

  * **Contracts**: none checked; a timer wrapping within one epoch yields a wrong row, not a failure
  * **Exception safety**: No operation throws; exceptions are off in the build

  \note Output happens after the epochs, never inside them.

  \sa \ref toy::benchmark::Clock
*/
class Bench final {
public:
  /*!
    \brief Builds the benchmark named \a title that measures with \a clock and prints to \a output.

    \param clock   Platform timer and calibration limits.
    \param output  Destination of the report lines.
    \param title   Benchmark name, the \c case field of every row; must outlive the object.

    \post Nothing is printed yet and the unit is \c op.
  */
  Bench(const Clock & clock, const Output & output, const char * title) noexcept;

  ~Bench() noexcept                = default;
  Bench(const Bench &)             = delete;
  Bench & operator=(const Bench &) = delete;
  Bench(Bench &&)                  = delete;
  Bench & operator=(Bench &&)      = delete;

  /*!
    \brief Names the operation one call measures.

    \param unitName  Operation name, such as \c search; must outlive the object.

    \return This benchmark.

    \post Following rows carry \a unitName in their \c unit field.
  */
  Bench & unit(const char * unitName) noexcept;

  /*!
    \brief Measures \a operation and prints its row.

    \tparam Callable   Type of the operation; invocable as an lvalue without arguments.
    \param  name       Row name, the implementation measured; must outlive the call.
    \param  operation  Operation called once per iteration inside every epoch.

    \return This benchmark.

    \post One line was written: the row for \a name, or a comment naming the epoch length and the call limit that
          calibration could not reach.
  */
  template <typename Callable>
    requires std::invocable<Callable &>
  Bench & run(const char * name, Callable && operation);

private:
  /// Returns the calls an epoch needs to reach \c minEpochTicks, or \c 0 when \c maxIterations is reached first.
  template <typename Epoch>
    requires std::invocable<Epoch &, std::uint32_t>
  [[nodiscard]] std::uint32_t calibrate(Epoch & timeEpoch) const;

  void writeRow(const char * name, std::uint32_t iterations,
                const std::array<std::uint32_t, detail::c_epochCount> & epochs) noexcept;
  void writeUncalibrated(const char * name) noexcept;

  Clock        _clock;
  Output       _output;
  const char * _title;
  const char * _unit{"op"};
};

} // namespace toy::benchmark

#include "builtin_bench.inl"

#endif // INCLUDE_BENCHMARKS_RUNNER_BUILTIN_BENCH_HPP_
