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
  \file   chrono_steady_clock.hpp
  \brief  Stateless monotonic clock facade over the active \ref toy::chrono::ClockSource.

  Defines \ref toy::chrono::SteadyClock: a \c TrivialClock-compatible, stateless clock whose tick period is derived from
  the compile-time constant \c toy::chrono::c_steadyClockPeriodDenominator.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_CHRONO_STEADY_CLOCK_HPP_
#define INCLUDE_CORE_CHRONO_STEADY_CLOCK_HPP_

namespace toy::chrono {

/*!
  \class SteadyClock

  \ingroup Chrono

  \brief Stateless monotonic clock that satisfies the \c TrivialClock concept.

  A stateless facade over the active \ref toy::chrono::ClockSource. All methods delegate to the source registered by the
  most recently constructed, still-living \ref toy::chrono::ClockSource. The tick period is fixed at compile time via
  \c toy::chrono::c_steadyClockPeriodDenominator.

  \section features Key Features

  - **TrivialClock-compatible**: provides \c rep, \c period, \c duration, \c time_point, \c is_steady, and \c now() as
    required by the \c std::chrono::TrivialClock named requirements.
  - **Stateless**: no instance data; all methods are \c static. Suitable for use as a clock type parameter in
    \c std::chrono templates.
  - **Compile-time period**: \c period is derived from \c toy::chrono::c_steadyClockPeriodDenominator and resolved at
    compile time.
  - **Thin wrapper**: delegates directly to the active source; no additional state and no exceptions.

  \section usage Usage Example

  \code
  #include "core.hpp"

  int main() {
    toy::chrono::ClockSource clock;

    const auto t0 = toy::chrono::SteadyClock::now();
    // ... work ...
    const auto t1 = toy::chrono::SteadyClock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
  }
  \endcode

  \section performance Performance Characteristics

  - **now()**: O(1); one counter read plus a constant unit conversion when the source frequency differs from
    \c period::den.
  - **nowTicks()**: O(1); one counter read with no unit conversion.
  - **frequency()**: O(1); one field load from the active \ref toy::chrono::ClockSource.
  - **Memory usage**: 0 bytes (stateless type).

  \section safety Safety Guarantees

  - **Precondition**: a \ref toy::chrono::ClockSource must be alive for the duration of any call; behaviour is undefined
    if no source is active.
  - **Monotonicity**: \c now() is monotonically non-decreasing on a single thread; not guaranteed across threads without
    synchronisation.
  - **Exception safety**: all operations are \c noexcept.

  \note \c is_steady is \c true: the underlying counter is never adjusted after the source is constructed.

  \sa \ref toy::chrono::ClockSource, \ref toy::chrono::Stopwatch, \ref toy::chrono::CountdownTimer
*/
class SteadyClock {
public:
  /// Arithmetic type used to count ticks; signed 64-bit to keep duration arithmetic free of overflow on long sessions.
  using rep        = int64_t;
  /// Tick period as a \c std::ratio; resolved at compile time from \c toy::chrono::c_steadyClockPeriodDenominator.
  using period     = std::ratio<1, static_cast<std::intmax_t>(c_steadyClockPeriodDenominator)>;
  /// Duration type produced by now(); a \ref toy::chrono::Duration parameterized on \c rep and \c period.
  using duration   = Duration<rep, period>;
  /// Time-point type returned by now(); a \ref toy::chrono::TimePoint anchored to this clock.
  using time_point = TimePoint<SteadyClock>;

  /// Satisfies the \c TrivialClock named requirement; the underlying counter is never adjusted after construction.
  static constexpr bool is_steady = true;

  /*!
    \brief Current time as a typed \c time_point.

    \return Monotonically non-decreasing \c time_point measured from an unspecified epoch.

    \pre An active \ref toy::chrono::ClockSource exists in the current process.

    \sa nowTicks(), frequency()
  */
  [[nodiscard]] static time_point now() noexcept;

  /*!
    \brief Current raw counter value.

    Bypasses unit conversion; use for micro-benchmarks or when the tick count itself is needed.

    \return Raw tick count; same epoch and monotonicity guarantees as now().

    \pre An active \ref toy::chrono::ClockSource exists in the current process.

    \sa now(), frequency()
  */
  [[nodiscard]] static rep nowTicks() noexcept;

  /*!
    \brief Tick frequency of the active counter in Hz.

    \return Ticks per second as reported by the active \ref toy::chrono::ClockSource.

    \pre An active \ref toy::chrono::ClockSource exists in the current process.

    \sa now(), nowTicks()
  */
  [[nodiscard]] static rep frequency() noexcept;
};

} // namespace toy::chrono

#endif // INCLUDE_CORE_CHRONO_STEADY_CLOCK_HPP_
