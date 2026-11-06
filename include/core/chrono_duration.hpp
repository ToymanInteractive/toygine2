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
  \file   chrono_duration.hpp
  \brief  Duration and time-point vocabulary of the chrono module.

  Defines \ref toy::chrono::Duration and \ref toy::chrono::TimePoint, transparent aliases over the standard-library
  templates. The clocks and \ref toy::chrono::Stopwatch build their member types from them.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_CHRONO_DURATION_HPP_
#define INCLUDE_CORE_CHRONO_DURATION_HPP_

namespace toy::chrono {

/*!
  \brief Time span of \a Rep ticks, each \a Period of a second long.

  Alias of std::chrono::duration with no runtime behavior of its own; every operation of the standard template applies.

  \tparam Rep    Arithmetic type of the tick count, or a class emulating one.
  \tparam Period std::ratio giving the seconds per tick.

  \sa \ref toy::chrono::TimePoint
*/
template <typename Rep, typename Period>
using Duration = std::chrono::duration<Rep, Period>;

/*!
  \brief Instant on \a Clock's timeline, stored as a \a Dur since the clock's epoch.

  Alias of std::chrono::time_point. Subtracting two points of one clock yields a \ref toy::chrono::Duration; the epoch
  itself is whatever the clock defines.

  \tparam Clock Clock type whose epoch the point counts from; satisfies the CppClock requirements.
  \tparam Dur   \ref toy::chrono::Duration measuring the offset from the epoch (default: the clock's own duration).

  \sa \ref toy::chrono::Duration
  \sa \ref toy::chrono::SteadyClock
*/
template <typename Clock, typename Dur = typename Clock::duration>
using TimePoint = std::chrono::time_point<Clock, Dur>;

//--------------------------------------------------------------------------------------------------------------------

/// Converts a duration to another period, truncating toward zero: https://cppreference.com/cpp/chrono/duration_cast
using std::chrono::duration_cast;

/// Duration counting whole hours
using std::chrono::hours;

/// Duration counting whole minutes
using std::chrono::minutes;

/// Duration counting whole seconds
using std::chrono::seconds;

/// Duration counting whole milliseconds
using std::chrono::milliseconds;

/// Duration counting whole microseconds
using std::chrono::microseconds;

/// Duration counting whole nanoseconds
using std::chrono::nanoseconds;

} // namespace toy::chrono

#endif // INCLUDE_CORE_CHRONO_DURATION_HPP_
