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
  \file   chrono_calendar_time.hpp
  \brief  Broken-down calendar reading with a UTC offset.

  Defines \ref toy::chrono::CalendarTime, the date and time fields \ref toy::chrono::SystemClock returns and
  fromUptime() synthesizes on targets without an RTC.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_CHRONO_CALENDAR_TIME_HPP_
#define INCLUDE_CORE_CHRONO_CALENDAR_TIME_HPP_

#include "chrono_duration.hpp"

namespace toy::chrono {

/*!
  \struct CalendarTime
  \brief  Local date and time split into calendar fields, with the offset from UTC.

  What toy::chrono::SystemClock::now() returns: the platform's local reading on the desktop targets, a value
  synthesized by fromUptime() where no RTC exists. Field ranges follow the platform reading; the synthesized value
  uses the conditional calendar fromUptime() documents.

  \section calendar_time_features Key Features

  * **Aggregate initialization**: no constructor; designated initializers name the fields, and an omitted one is zero.
  * **Constexpr support**: comparison and fromUptime() evaluate in a constant expression.
  * **Trivially copyable**: a copy is a byte copy of the twelve bytes.
  * **Complete reading**: the offset from UTC travels with the fields, so a consumer can reconstruct the instant.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section calendar_time_usage Usage Example

  \code
  #include "core.hpp"

  const toy::chrono::CalendarTime calendar = toy::chrono::SystemClock::now();
  const auto year = calendar.year;
  \endcode

  \section calendar_time_performance Performance Characteristics

  * **Construction**: O(1); fields are stored as given.
  * **Access**: O(1); every field is read directly.
  * **Memory usage**: 12 bytes, alignment 2.

  \section calendar_time_safety Safety Guarantees

  * **Contracts**: none on the fields; fromUptime() checks its input with assert_message in debug builds.
  * **Type safety**: fixed-width fields carry unit-bearing values, not a formatted string.
  * **Memory safety**: nothing allocates, and the value owns nothing outside its fields.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \note A default-constructed value is all zeros, which is the base of the synthesis, not a valid calendar date.

  \sa \ref toy::chrono::SystemClock
*/
struct CalendarTime {
  /*!
    \brief Splits an uptime into calendar fields over a conditional 360-day calendar.

    Counts from an all-zero base: months hold 30 days and years hold 12 such months, so a reading never looks like a
    valid current date. \ref toy::chrono::CalendarTime::dayOfWeek and \ref toy::chrono::CalendarTime::tzOffsetMinutes
    stay \c 0. \ref toy::chrono::SystemClock calls it on targets without an RTC.

    \param uptime Milliseconds elapsed since the process started.

    \return Fields of \a uptime laid over the zero base.

    \pre \a uptime is non-negative, checked by assert_message in debug builds.
    \pre The year computed from \a uptime fits \c int16_t, checked by assert_message in debug builds.

    \note Works in a constant expression.
  */
  [[nodiscard]] static constexpr CalendarTime fromUptime(Duration<int64_t, std::milli> uptime) noexcept;

  int16_t year{0};            ///< Full year on an RTC reading; whole 360-day years since the zero base when synthesized

  uint8_t month{0};           ///< Month 1-12 on an RTC reading; 0-11 when synthesized

  uint8_t day{0};             ///< Day of the month 1-31 on an RTC reading; 0-29 when synthesized

  uint8_t dayOfWeek{0};       ///< Day of the week, \c 0 = Sunday through \c 6 = Saturday; \c 0 when synthesized

  uint8_t hour{0};            ///< Hour 0-23

  uint8_t minute{0};          ///< Minute 0-59

  uint8_t second{0};          ///< Second 0-59; the platform APIs report no leap second

  uint16_t millisecond{0};    ///< Millisecond within the second, 0-999

  int16_t tzOffsetMinutes{0}; ///< Local offset from UTC in minutes, east positive (UTC+3: \c 180); \c 0 synthesized

  /*!
    \brief Reports whether every field of \a lhs equals the matching field of \a rhs.

    \param lhs Reading on the left of the operator.
    \param rhs Reading on the right of the operator.

    \return \c true when all nine fields match; the compiler derives \c != from this operator.

    \note Works in a constant expression.
  */
  [[nodiscard]] friend constexpr bool operator==(const CalendarTime & lhs, const CalendarTime & rhs) noexcept = default;
};

} // namespace toy::chrono

#endif // INCLUDE_CORE_CHRONO_CALENDAR_TIME_HPP_
