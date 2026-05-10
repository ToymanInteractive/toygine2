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

  Defines \ref toy::chrono::Duration and \ref toy::chrono::TimePoint as thin aliases over \c std::chrono, and provides
  \ref toy::chrono::DurationFormat for pattern-based duration formatting.

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

  \sa \ref toy::chrono::TimePoint
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

/*!
  \brief Pairs a \ref toy::chrono::Duration with a null-terminated display pattern.

  \ingroup Chrono

  Passed to \c toy::OStringStream::operator<< to format the duration into a human-readable clock representation. The
  pattern is scanned character-by-character; recognised tokens are replaced with the corresponding time component
  derived from a millisecond decomposition; all other characters are emitted verbatim.

  \tparam Rep    Arithmetic representation type of the duration tick count.
  \tparam Period \c std::ratio specifying the tick period relative to one second.

  \section features Key Features

  - **Flexible patterns**: any combination of \c h, \c m, \c s, \c z tokens with arbitrary literal separators.
  - **Millisecond precision**: decomposes duration down to milliseconds; sub-millisecond ticks are truncated.
  - **Negative durations**: prefixes the formatted output with \c '-'; absolute value is formatted normally.
  - **Literal passthrough**: non-token characters (\c ':', \c '.', spaces, labels) are emitted unchanged.

  \section pattern_tokens Pattern Tokens

  | Token  | Component    | Behaviour                     |
  |--------|--------------|-------------------------------|
  | \c h   | hours        | no leading zero (e.g. \c 9)   |
  | \c hh  | hours        | always 2 digits (e.g. \c 09)  |
  | \c m   | minutes      | no leading zero (e.g. \c 3)   |
  | \c mm  | minutes      | always 2 digits (e.g. \c 03)  |
  | \c s   | seconds      | no leading zero (e.g. \c 5)   |
  | \c ss  | seconds      | always 2 digits (e.g. \c 05)  |
  | \c z   | milliseconds | no leading zero (e.g. \c 42)  |
  | \c zzz | milliseconds | always 3 digits (e.g. \c 042) |

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

  \section performance Performance Characteristics

  - **Construction**: O(1); no computation at construction time.
  - **Formatting**: O(n) where \a n is the length of \a pattern; one \c duration_cast to milliseconds then a
    single pass through the pattern string.
  - **Memory usage**: 8–16 bytes depending on \a Rep and pointer size; no heap allocation.

  \section safety Safety Guarantees

  - **Pattern lifetime**: \a pattern must remain valid for the duration of the format operation; string literals
    and static-storage pointers satisfy this. Dangling pointers cause undefined behaviour.
  - **Exception safety**: all operations are \c noexcept.

  \sa \ref toy::chrono::Duration
*/
template <typename Rep, typename Period>
struct DurationFormat {
  /// Format pattern.
  CStringView pattern;

  /// Duration value to format.
  Duration<Rep, Period> duration;
};

/*!
  \brief Deduction guide: enables \c DurationFormat{"pattern", duration} without explicit template arguments when the
         second argument is a \ref toy::chrono::Duration.

  \tparam Rep    Arithmetic representation type deduced from the \ref toy::chrono::Duration argument.
  \tparam Period \c std::ratio tick period deduced from the \ref toy::chrono::Duration argument.
*/
template <typename Rep, typename Period>
DurationFormat(const char *, Duration<Rep, Period>) -> DurationFormat<Rep, Period>;

} // namespace toy::chrono

#endif // INCLUDE_CORE_CHRONO_DURATION_HPP_
