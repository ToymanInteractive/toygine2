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
  \file   chrono_duration.hpp
  \brief  Duration and time-point aliases over \c std::chrono.

  Defines \ref toy::chrono::Duration and \ref toy::chrono::TimePoint as thin aliases over \c std::chrono, and brings
  \c std::chrono::duration_cast into \ref toy::chrono so call sites can use it unqualified.

  \note Included by core.hpp; do not include this file directly.
*/

#ifndef INCLUDE_CORE_CHRONO_DURATION_HPP_
#define INCLUDE_CORE_CHRONO_DURATION_HPP_

namespace toy::chrono {

/*!
  \brief Generic duration alias over \c std::chrono::duration.

  \ingroup Chrono

  \tparam RepresentationType Arithmetic type representing the tick count.
  \tparam PeriodType         \c std::ratio specifying the tick period relative to one second.

  \sa \ref toy::chrono::TimePoint, duration_cast()
*/
template <typename RepresentationType, typename PeriodType>
using Duration = std::chrono::duration<RepresentationType, PeriodType>;

/*!
  \brief Generic time-point alias over \c std::chrono::time_point.

  \ingroup Chrono

  \tparam ClockType    The clock type that defines the epoch.
  \tparam DurationType Duration type used to store the offset from the epoch (defaults to \c Clock::duration).

  \sa \ref toy::chrono::Duration
*/
template <typename ClockType, typename DurationType = typename Clock::duration>
using TimePoint = std::chrono::time_point<ClockType, DurationType>;

/// Brings \c std::chrono::duration_cast into \ref toy::chrono for unqualified use at call sites.
using std::chrono::duration_cast;

} // namespace toy::chrono

#endif // INCLUDE_CORE_CHRONO_DURATION_HPP_
