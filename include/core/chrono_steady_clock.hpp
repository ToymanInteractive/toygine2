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
  \file   chrono_steady_clock.hpp
  \brief  Monotonic tick counter of the active platform.

  Defines \ref toy::chrono::SteadyClock, the highest-resolution steady counter the platform offers, and
  toy::chrono::initialize(), which fixes the origin \ref toy::chrono::Timestamp counts from.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_CHRONO_STEADY_CLOCK_HPP_
#define INCLUDE_CORE_CHRONO_STEADY_CLOCK_HPP_

#include "chrono_duration.hpp"

namespace toy::chrono {

/*!
  \brief Ticks per second of the \ref toy::chrono::SteadyClock duration, fixed by the active target.

  The value lives in each target's platform_config header under include/platforms, selected by the dispatcher in
  platforms/platform_config.hpp.

  \sa \ref toy::chrono::SteadyClock
*/
inline constexpr int64_t steadyClockPeriodDenominator = platform::detail::steadyClockPeriodDenominator;

/*!
  \brief Prepares the clock backend and fixes the origin toy::chrono::Timestamp::now() counts from.

  The engine calls it once at startup, before the application body runs; a program with its own entry point calls it
  there instead, after the platform's interrupt path is ready where the backend needs one: a dispatcher, an interrupt
  source, and the mask.

  \post toy::chrono::Timestamp::now() counts from this call.

  \note Calling it again restarts the timestamp origin and breaks the ordering of stamps taken across the call.
*/
void initialize() noexcept;

/*!
  \class SteadyClock
  \brief Monotonic clock over the highest-resolution steady counter of the active platform.

  Stateless facade: every method is static and reads the platform counter directly. The epoch is unspecified, so only
  differences of readings taken within one process carry meaning.

  \section steady_clock_features Key Features

  * **Monotonic**: readings never step back within a process, whatever the system does to wall-clock time.
  * **Fixed unit**: \ref toy::chrono::SteadyClock::period is fixed by \ref toy::chrono::steadyClockPeriodDenominator.
  * **Tick access**: nowTicks() and frequency() expose the raw counter for arithmetic without a duration conversion.
  * **No allocation**: a reading is a platform counter read plus integer arithmetic.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section steady_clock_usage Usage Example

  \code
  #include "core.hpp"

  const auto frameStart = toy::chrono::SteadyClock::now();
  // ... frame work ...
  const auto frameDelta = toy::chrono::SteadyClock::now() - frameStart;
  \endcode

  \section steady_clock_performance Performance Characteristics

  * **Reading**: O(1); one platform counter read, plus integer arithmetic where the counter unit differs from the
    period.
  * **Memory usage**: no state; the class is never instantiated.

  \section steady_clock_safety Safety Guarantees

  * **Contracts**: none on the reading side; initialize() runs before the application starts.
  * **Thread safety**: every method reads the counter and state written once at startup, so any thread may call them.
  * **Determinism**: readings differ between runs and targets; replay and rollback record inputs, not re-read ticks.
  * **Memory safety**: nothing allocates.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \sa \ref toy::chrono::Stopwatch
  \sa \ref toy::chrono::SystemClock
*/
class SteadyClock {
public:
  /// Signed 64-bit tick count; the range outlasts any session
  using rep = int64_t;

  /// Seconds per tick of \ref toy::chrono::SteadyClock::duration, from \ref toy::chrono::steadyClockPeriodDenominator
  using period = std::ratio<1, static_cast<std::intmax_t>(steadyClockPeriodDenominator)>;

  /// Time span counted in ticks of \ref toy::chrono::SteadyClock::period
  using duration = Duration<rep, period>;

  /// Instant on this clock's unspecified epoch
  using time_point = TimePoint<SteadyClock>;

  /// Readings never step back within a process
  static constexpr bool is_steady = true;

  /*!
    \brief Reads the counter as a point on this clock's timeline.

    \return Current reading; differences of two readings measure the span between them.

    \note Allocates nothing and never blocks.

    \sa nowTicks()
  */
  [[nodiscard]] static time_point now() noexcept;

  /*!
    \brief Reads the raw platform counter.

    \return Current tick count in the counter's own unit.

    \note The unit is the counter's, not period's; frequency() converts between them.

    \sa now(), frequency()
  */
  [[nodiscard]] static rep nowTicks() noexcept;

  /*!
    \brief Reports the tick rate of the counter nowTicks() reads.

    \return Ticks per second of the counter nowTicks() reads.

    \sa nowTicks(), toy::chrono::initialize()
  */
  [[nodiscard]] static rep frequency() noexcept;
};

} // namespace toy::chrono

#endif // INCLUDE_CORE_CHRONO_STEADY_CLOCK_HPP_
