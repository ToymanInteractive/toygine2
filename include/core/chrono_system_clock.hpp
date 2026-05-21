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
  \brief  Wall-clock type that returns the current local date and time as a broken-down \ref toy::chrono::CalendarTime.

  Defines \ref toy::chrono::SystemClock: a stateless clock that returns a \ref toy::chrono::CalendarTime. Not steady —
  the returned value may change non-monotonically if the system clock is adjusted (NTP, DST, manual set).

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_CHRONO_SYSTEM_CLOCK_HPP_
#define INCLUDE_CORE_CHRONO_SYSTEM_CLOCK_HPP_

namespace toy::chrono {

/*!
  \class SystemClock

  \ingroup Chrono

  \brief Stateless wall clock that returns the current local date and time as \ref toy::chrono::CalendarTime.

  Each call to now() reads the platform calendar-time API directly and returns broken-down local time as a
  \ref toy::chrono::CalendarTime aggregate. Does not require an active \ref toy::chrono::ClockSource. The clock is not
  steady: the returned value may change non-monotonically when the system clock is adjusted by NTP, DST transitions,
  time-zone changes, or a manual time set. Use \ref toy::chrono::SteadyClock for elapsed-time measurement and
  \ref toy::chrono::Stopwatch or \ref toy::chrono::CountdownTimer for interval logic.

  \section features Key Features

  - **Broken-down local time**: now() returns year, month, day, weekday, hour, minute, second, and millisecond directly
    — no epoch arithmetic required at the call site.
  - **No ClockSource dependency**: stateless; no RAII owner required before calling now().
  - **Millisecond resolution**: sub-millisecond precision from the underlying platform API is truncated.
  - **Not steady**: \c is_steady is \c false — the clock may be adjusted at any time by the operating system.
  - **Stub support**: on platforms without RTC now() returns \ref toy::chrono::CalendarTime::invalid().

  \section usage Usage Example

  \code
  #include "core.hpp"

  const toy::chrono::CalendarTime ct = toy::chrono::SystemClock::now();
  // ct.year == 2026, ct.month == 5, ct.day == 20, ct.hour == 14, ...
  \endcode

  \section performance Performance Characteristics

  - **now()**: O(1); one platform API call (\c clock_gettime + \c localtime_r on POSIX, \c GetLocalTime on Windows).
  - **Memory usage**: 0 bytes (stateless type).

  \section safety Safety Guarantees

  - **No preconditions**: now() may be called without an active \ref toy::chrono::ClockSource.
  - **Stub behaviour**: on platforms without RTC, now() returns \ref toy::chrono::CalendarTime::invalid().
  - **Exception safety**: all operations are \c noexcept.

  \note \c is_steady is \c false: the clock may jump backward or forward when the system time is adjusted.

  \warning Do not use for elapsed-time measurement or interval logic; use \ref toy::chrono::SteadyClock,
           \ref toy::chrono::Stopwatch, or \ref toy::chrono::CountdownTimer instead.

  \sa \ref toy::chrono::CalendarTime, \ref toy::chrono::SteadyClock, \ref toy::chrono::Stopwatch,
      \ref toy::chrono::CountdownTimer
*/
class SystemClock {
public:
  /// Not steady: the underlying platform clock may be adjusted at any time by the operating system.
  static constexpr bool is_steady = false;

  /*!
    \brief Current local wall-clock time as a broken-down \ref toy::chrono::CalendarTime.

    Reads the platform calendar-time API directly on each call. The returned value advances with real time but may
    change non-monotonically if the system clock is adjusted (NTP sync, DST transition, time-zone change, manual set).

    \return \ref toy::chrono::CalendarTime with all fields filled from the current local time.

    \note On platforms without an RTC this function always returns \ref toy::chrono::CalendarTime::invalid().
  */
  [[nodiscard]] static CalendarTime now() noexcept;
};

} // namespace toy::chrono

#endif // INCLUDE_CORE_CHRONO_SYSTEM_CLOCK_HPP_
