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
  \file   chrono_system_clock.hpp
  \brief  Wall-clock reading of the active platform.

  Defines \ref toy::chrono::SystemClock, the stateless entry point that returns the local time as
  \ref toy::chrono::CalendarTime. Used for time display in games and for log output on the desktop targets.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_CHRONO_SYSTEM_CLOCK_HPP_
#define INCLUDE_CORE_CHRONO_SYSTEM_CLOCK_HPP_

#include "chrono_calendar_time.hpp"

namespace toy::chrono {

/*!
  \class SystemClock
  \brief Wall clock returning the local date and time as broken-down fields.

  Reads the platform's real-time source where one exists and synthesizes a value from the process uptime elsewhere.
  Every target returns a usable \ref toy::chrono::CalendarTime; no reading is a sentinel. Adjustments of the system
  time move the readings, so spans measured across them belong to \ref toy::chrono::SteadyClock.

  \section system_clock_features Key Features

  * **Local reading**: fields carry local time and the offset from UTC, ready for display.
  * **Always usable**: targets without an RTC return the value toy::chrono::CalendarTime::fromUptime() synthesizes
    instead of an invalid marker.
  * **Stateless**: the single method is static; the class is never instantiated.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section system_clock_usage Usage Example

  \code
  #include "core.hpp"

  const toy::chrono::CalendarTime calendar = toy::chrono::SystemClock::now();
  \endcode

  \section system_clock_performance Performance Characteristics

  * **Reading**: O(1); platform time calls, no loops over data.
  * **Memory usage**: no state; the result is 12 bytes.

  \section system_clock_safety Safety Guarantees

  * **Contracts**: none; the platform startup order covers the synthesis (see toy::chrono::initialize()).
  * **Thread safety**: every call reads the platform time source independently.
  * **Determinism**: readings differ between runs and targets; never feed them into simulation state.
  * **Memory safety**: nothing allocates.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section system_clock_compatibility Compatibility

  * macOS and Linux read clock_gettime(CLOCK_REALTIME) with localtime_r; Windows reads GetLocalTime with
    GetTimeZoneInformation.
  * The console targets synthesize from the uptime through toy::chrono::CalendarTime::fromUptime(); on the stub
    backends the uptime is zero, so the reading is the all-zero base.

  \sa \ref toy::chrono::CalendarTime
  \sa \ref toy::chrono::SteadyClock
*/
class SystemClock {
public:
  /// System time adjustments move the readings, so the clock is not steady
  static constexpr bool is_steady = false;

  /*!
    \brief Reads the local wall-clock time.

    \return Local date and time with the offset from UTC; on targets without an RTC, the value
            toy::chrono::CalendarTime::fromUptime() synthesizes from the uptime.

    \note Allocates nothing; the platform time calls never block on I/O.

    \sa \ref toy::chrono::CalendarTime
  */
  [[nodiscard]] static CalendarTime now() noexcept;
};

} // namespace toy::chrono

#endif // INCLUDE_CORE_CHRONO_SYSTEM_CLOCK_HPP_
