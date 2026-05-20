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
  \file   chrono_calendar_time.hpp
  \brief  Broken-down calendar date and time with millisecond resolution.

  Defines \ref toy::chrono::CalendarTime: a plain aggregate holding the Gregorian date and local time components
  returned by \ref toy::chrono::SystemClock::now().

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_CHRONO_CALENDAR_TIME_HPP_
#define INCLUDE_CORE_CHRONO_CALENDAR_TIME_HPP_

namespace toy::chrono {

/*!
  \struct CalendarTime
  \brief  Broken-down local calendar date and time with millisecond resolution.

  A plain aggregate populated by \ref toy::chrono::SystemClock::now(). All fields are zero on platforms without an RTC.

  \section features Key Features

  - **Millisecond resolution**: \c millisecond carries the sub-second component; sub-millisecond precision is truncated.
  - **Stub-safe**: all fields are zero-initialized on platforms without RTC.
  - **Aggregate type**: no user-declared constructors; supports designated initializers and \c constexpr use.

  \section usage Usage Example

  \code
  #include "core.hpp"

  const toy::chrono::CalendarTime ct = toy::chrono::SystemClock::now();
  // ct.year == 2026, ct.month == 5, ct.day == 19, ct.hour == 14, ...
  \endcode

  \section performance Performance Characteristics

  - **Size**: 10 bytes (no padding; alignment 2).
  - **Construction**: O(1); zero-initialization via \c CalendarTime{}.

  \section safety Safety Guarantees

  - **Type safety**: fixed-width integer fields prevent silent narrowing.
  - **Exception safety**: aggregate; all operations are \c noexcept.
  - **Memory safety**: no dynamic allocation; suitable for embedded and constrained targets.

  \sa \ref toy::chrono::SystemClock
*/
struct CalendarTime {
  /*!
    \brief Returns a sentinel used on platforms without RTC.

    \c year == \c 0 and \c month == \c 0 are not valid Gregorian values, so the result cannot be confused with a real
    timestamp.

    \return \ref toy::chrono::CalendarTime with all fields zero.
  */
  [[nodiscard]] static constexpr CalendarTime invalid() noexcept;

  /// Calendar year (e.g. 2026).
  int16_t  year;
  /// Month of year: 1–12.
  uint8_t  month;
  /// Day of month: 1–31.
  uint8_t  day;
  /// Day of week: 0 = Sunday, 1 = Monday, …, 6 = Saturday.
  uint8_t  dayOfWeek;
  /// Hour of day in 24-hour format: 0–23.
  uint8_t  hour;
  /// Minute within the hour: 0–59.
  uint8_t  minute;
  /// Second within the minute: 0–60 (60 occurs on a positive leap second).
  uint8_t  second;
  /// Millisecond within the current second: 0–999.
  uint16_t millisecond;
};

} // namespace toy::chrono

#endif // INCLUDE_CORE_CHRONO_CALENDAR_TIME_HPP_
