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

  Defines \ref toy::chrono::Duration and \ref toy::chrono::TimePoint as thin aliases over \c std::chrono, brings
  \c std::chrono::duration_cast into \ref toy::chrono for unqualified use, and provides \ref toy::chrono::DurationFormat
  for pattern-based duration formatting.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_CHRONO_DURATION_HPP_
#define INCLUDE_CORE_CHRONO_DURATION_HPP_

namespace toy::chrono {

/*!
  \brief Generic duration alias over \c std::chrono::duration.

  \ingroup Chrono

  \tparam Rep    Arithmetic type representing the tick count.
  \tparam Period \c std::ratio specifying the tick period relative to one second.

  \sa \ref toy::chrono::TimePoint, duration_cast()
*/
template <typename Rep, typename Period>
using Duration = std::chrono::duration<Rep, Period>;

/*!
  \brief Generic time-point alias over \c std::chrono::time_point.

  \ingroup Chrono

  \tparam Clock The clock type that defines the epoch.
  \tparam Dur   Duration type used to store the offset from the epoch (defaults to \c Clock::duration).

  \sa \ref toy::chrono::Duration
*/
template <typename Clock, typename Dur = typename Clock::duration>
using TimePoint = std::chrono::time_point<Clock, Dur>;

/// Brings \c std::chrono::duration_cast into \ref toy::chrono for unqualified use at call sites.
using std::chrono::duration_cast;

/*!
  \struct DurationFormat

  \brief Pairs a \ref toy::chrono::Duration with a null-terminated display pattern.

  \ingroup Chrono

  Passed to \c toy::OStringStream::operator<< to format the duration according to the pattern. The pattern is scanned
  character-by-character; recognised tokens are substituted with the corresponding time component; all other characters
  are emitted as literals.

  \section pattern_tokens Pattern Tokens

  | Token | Component    | Behaviour                     |
  |-------|--------------|-------------------------------|
  | \c h  | hours        | no leading zero (e.g. \c 9)   |
  | \c hh | hours        | always 2 digits (e.g. \c 09)  |
  | \c m  | minutes      | no leading zero (e.g. \c 3)   |
  | \c mm | minutes      | always 2 digits (e.g. \c 03)  |
  | \c s  | seconds      | no leading zero (e.g. \c 5)   |
  | \c ss | seconds      | always 2 digits (e.g. \c 05)  |
  | \c z  | milliseconds | no leading zero (e.g. \c 42)  |
  | \c zzz| milliseconds | always 3 digits (e.g. \c 042) |

  Any other character in \a pattern is emitted verbatim. Negative durations are prefixed with \c '-' followed by the
  formatted absolute value.

  \tparam Rep    Arithmetic representation type of the duration tick count.
  \tparam Period \c std::ratio specifying the tick period relative to one second.

  \section usage Usage Example

  \code
  #include "core.hpp"

  toy::chrono::ClockSource clock;
  toy::chrono::Stopwatch   sw;
  // ... work ...
  const auto fmt = toy::chrono::DurationFormat{sw.elapsed(), "hh:mm:ss.zzz"};
  toy::OStringStream<toy::FixedString<32>> stream;
  stream << fmt; // e.g. "00:00:01.042"
  \endcode

  \warning The string referenced by \c pattern must outlive the \c DurationFormat value. Passing a pointer to a
           temporary or stack-allocated buffer beyond that buffer's lifetime causes undefined behavior. Use a string
           literal or storage with static lifetime.

  \sa \ref toy::chrono::Duration, \ref toy::chrono::Stopwatch
*/
template <typename Rep, typename Period>
struct DurationFormat {
  /// Duration value to format.
  Duration<Rep, Period> value;

  /// Pattern string.
  CStringView pattern;
};

} // namespace toy::chrono

#endif // INCLUDE_CORE_CHRONO_DURATION_HPP_
