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
  \file   chrono_system_clock.hpp
  \brief  Wall-clock type anchored to the Unix epoch.

  Defines \ref toy::chrono::SystemClock: a stateless clock that returns the current calendar time as nanoseconds since
  Jan 1, 1970 (UTC). Does not depend on \ref toy::chrono::ClockSource; delegates directly to the platform time API on
  each call. Not steady — the returned value may decrease if the system clock is adjusted (NTP, DST, manual set).

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_CHRONO_SYSTEM_CLOCK_HPP_
#define INCLUDE_CORE_CHRONO_SYSTEM_CLOCK_HPP_

#include "chrono_duration.hpp"

namespace toy::chrono {

/*!
  \class SystemClock

  \ingroup Chrono

  \brief Stateless wall clock anchored to the Unix epoch (Jan 1, 1970, UTC).

  Returns the current calendar time as a \ref toy::chrono::TimePoint with nanosecond resolution. Does not require an
  active \ref toy::chrono::ClockSource — each call to now() reads the platform time API directly. The clock is not
  steady: the returned value may decrease or jump when the system clock is adjusted by NTP, DST changes, or a manual
  time set. Use \ref toy::chrono::SteadyClock for elapsed-time measurement and \ref toy::chrono::Stopwatch or
  \ref toy::chrono::CountdownTimer for interval logic.

  \section features Key Features

  - **Unix epoch**: \c time_point is measured from Jan 1, 1970, 00:00:00 UTC; compatible with POSIX and OS calendar
    APIs.
  - **No ClockSource dependency**: stateless; no RAII owner required before calling now().
  - **Nanosecond period**: \c period is \c std::ratio<1,1000000000> on all platforms; precision is bounded by the
    underlying platform API.
  - **Not steady**: \c is_steady is \c false — the clock may be adjusted at any time by the operating system.
  - **Stub support**: on platforms without RTC (e.g. GBA) now() returns \c time_point{} (epoch).

  \section usage Usage Example

  \code
  #include "core.hpp"

  const auto now = toy::chrono::SystemClock::now();
  const auto secondsSinceEpoch =
    toy::chrono::duration_cast<toy::chrono::seconds>(now.time_since_epoch());
  \endcode

  \section performance Performance Characteristics

  - **now()**: O(1); one platform API call (\c clock_gettime on POSIX, \c GetSystemTimePreciseAsFileTime on Windows).
  - **Memory usage**: 0 bytes (stateless type).

  \section safety Safety Guarantees

  - **No preconditions**: now() may be called without an active \ref toy::chrono::ClockSource.
  - **Non-negative on RTC platforms**: now().time_since_epoch().count() is non-negative on all platforms with a
    functioning RTC initialised to a date after Jan 1, 1970.
  - **Stub behaviour**: on platforms without RTC, now() returns \c time_point{} (epoch, count == 0).
  - **Exception safety**: all operations are \c noexcept.

  \note \c is_steady is \c false: the clock may jump backward or forward when the system time is adjusted.

  \warning Do not use for elapsed-time measurement or interval logic; use \ref toy::chrono::SteadyClock,
           \ref toy::chrono::Stopwatch, or \ref toy::chrono::CountdownTimer instead.

  \sa \ref toy::chrono::SteadyClock, \ref toy::chrono::Stopwatch, \ref toy::chrono::CountdownTimer
*/
class SystemClock {
public:
  /// Arithmetic type used to count nanoseconds since the Unix epoch.
  using rep        = int64_t;
  /// Tick period: one nanosecond, fixed on all platforms.
  using period     = std::ratio<1, 1'000'000'000>;
  /// Duration type produced by now(); nanoseconds as a \ref toy::chrono::Duration.
  using duration   = Duration<rep, period>;
  /// Time-point type returned by now(); anchored to the Unix epoch via this clock.
  using time_point = TimePoint<SystemClock>;

  /// Not steady: the underlying platform clock may be adjusted after construction.
  static constexpr bool is_steady = false;

  /*!
    \brief Current wall-clock time as nanoseconds since the Unix epoch (Jan 1, 1970, UTC).

    Reads the platform calendar-time API directly on each call. The returned value advances with real time but may
    decrease or jump if the system clock is adjusted (NTP sync, DST change, manual set).

    \return \c time_point whose \c time_since_epoch() is the number of nanoseconds elapsed since Jan 1, 1970, 00:00:00
            UTC, or \c time_point{} on platforms without RTC.

    \note On platforms without an RTC (e.g. GBA) this function always returns \c time_point{} (epoch).

    \sa \ref toy::chrono::SteadyClock::now()
  */
  [[nodiscard]] static time_point now() noexcept;
};

} // namespace toy::chrono

#endif // INCLUDE_CORE_CHRONO_SYSTEM_CLOCK_HPP_
