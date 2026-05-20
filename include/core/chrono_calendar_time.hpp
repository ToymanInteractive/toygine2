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

  \ingroup Chrono

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

  /*!
    \brief Equality comparison is deleted.

    \ref toy::chrono::CalendarTime carries no time-zone metadata: identical field values from different hosts or after a
    DST transition do not refer to the same instant. Comparing two values would silently hide that ambiguity. Compare
    explicit fields directly when the intent is well-defined.
  */
  friend bool operator==(const CalendarTime & lhs, const CalendarTime & rhs) = delete;
};

/*!
  \struct CalendarTimeFormat

  \ingroup Chrono

  \brief Pairs a \ref toy::chrono::CalendarTime with a null-terminated display pattern.

  Passed to \c toy::OStringStream::operator<< to format the calendar date and time into into a human-readable
  representation. The pattern is scanned character-by-character; recognised tokens are replaced with the corresponding
  field of the \ref toy::chrono::CalendarTime; all other characters are emitted verbatim.

  \section features Key Features

  - **Flexible patterns**: any combination of \c y, \c M, \c d, \c h, \c m, \c s, \c z tokens with arbitrary literal
    separators.
  - **Literal passthrough**: non-token characters (\c '-', \c ':', \c '.', \c 'T', spaces, labels) are emitted
  unchanged.

  \section pattern_tokens Pattern Tokens

  | Token   | Component    | Behaviour                                  |
  |---------|--------------|--------------------------------------------|
  | \c y    | year         | no leading zero (e.g. \c 2026)             |
  | \c yy   | year         | last 2 digits, zero-padded (e.g. \c 26)    |
  | \c yyyy | year         | always 4 digits, zero-padded (e.g. \c 2026)|
  | \c M    | month        | no leading zero (e.g. \c 5)                |
  | \c MM   | month        | always 2 digits (e.g. \c 05)               |
  | \c d    | day          | no leading zero (e.g. \c 7)                |
  | \c dd   | day          | always 2 digits (e.g. \c 07)               |
  | \c h    | hour         | no leading zero (e.g. \c 9)                |
  | \c hh   | hour         | always 2 digits (e.g. \c 09)               |
  | \c m    | minute       | no leading zero (e.g. \c 3)                |
  | \c mm   | minute       | always 2 digits (e.g. \c 03)               |
  | \c s    | second       | no leading zero (e.g. \c 5)                |
  | \c ss   | second       | always 2 digits (e.g. \c 05)               |
  | \c z    | millisecond  | no leading zero (e.g. \c 42)               |
  | \c zzz  | millisecond  | always 3 digits (e.g. \c 042)              |

  \section usage Usage Example

  \code
  #include "core.hpp"

  const toy::chrono::CalendarTime ct = toy::chrono::SystemClock::now();
  toy::OStringStream<toy::FixedString<32>> stream;
  stream << toy::chrono::CalendarTimeFormat{"yyyy-MM-dd HH:mm:ss.zzz", ct}; // e.g. "2026-05-20 14:30:45.123"
  \endcode

  \section performance Performance Characteristics

  - **Construction**: O(1); no computation at construction time.
  - **Formatting**: O(n) where \a n is the length of \a pattern; one pass through the pattern string.
  - **Memory usage**: 12 bytes (\ref toy::CStringView pointer/length plus 10-byte \ref toy::chrono::CalendarTime); no
    heap allocation.

  \section safety Safety Guarantees

  - **Pattern lifetime**: \a pattern must remain valid for the duration of the format operation; string literals and
    static-storage pointers satisfy this. Dangling pointers cause undefined behaviour.
  - **Exception safety**: all operations are \c noexcept.

  \note Month token uses uppercase \c M to disambiguate from minutes; all other tokens are lowercase as in
        \ref toy::chrono::DurationFormat.
  \note \c yy emits the last two digits of the year (e.g. \c 2026 → \c 26).

  \sa \ref toy::chrono::CalendarTime
*/
struct CalendarTimeFormat {
  /// Format pattern.
  CStringView pattern;

  /// Calendar date and time to format.
  CalendarTime calendarTime;
};

} // namespace toy::chrono

#endif // INCLUDE_CORE_CHRONO_CALENDAR_TIME_HPP_
