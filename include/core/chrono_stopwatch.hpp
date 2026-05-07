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
  \file   chrono_stopwatch.hpp
  \brief  Elapsed-time measurement type over the active \ref toy::chrono::SteadyClock.

  Defines \ref toy::chrono::Stopwatch: an 8-byte, trivially copyable elapsed-time counter. Captures the current tick
  count on construction and computes elapsed time on demand by querying \ref toy::chrono::SteadyClock.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_CHRONO_STOPWATCH_HPP_
#define INCLUDE_CORE_CHRONO_STOPWATCH_HPP_

namespace toy::chrono {

/*!
  \class Stopwatch

  \ingroup Chrono

  \brief Elapsed-time counter anchored to the active \ref toy::chrono::SteadyClock.

  Captures the tick count at construction and computes elapsed time on every call to elapsed() or elapsedTicks() by
  reading the current tick count from \ref toy::chrono::SteadyClock. Trivially copyable: copying a \c Stopwatch produces
  an independent counter with the same start point.

  \section features Key Features

  - **Trivially copyable**: safe to copy with \c memcpy; no destructor or heap involvement.
  - **8-byte footprint**: single \c int64_t field; suitable for stack, struct-of-arrays, and embedded targets.
  - **Zero-overhead elapsed**: elapsed() subtracts two tick values with no dynamic allocation or system call overhead
    beyond the counter read in \ref toy::chrono::SteadyClock::nowTicks().
  - **Resettable**: reset() moves the anchor to the current tick without constructing a new object.

  \section usage Usage Example

  \code
  #include "core.hpp"

  toy::chrono::ClockSource clock;

  toy::chrono::Stopwatch sw;
  // ... work ...
  const auto ms = toy::chrono::duration_cast<std::chrono::milliseconds>(sw.elapsed());
  \endcode

  \section performance Performance Characteristics

  - **Construction**: O(1); one counter read via \ref toy::chrono::SteadyClock::nowTicks().
  - **elapsed()**: O(1); one counter read and one subtraction.
  - **elapsedTicks()**: O(1); one counter read and one subtraction.
  - **reset()**: O(1); one counter read.
  - **Memory usage**: 8 bytes.

  \section safety Safety Guarantees

  - **Precondition**: an active \ref toy::chrono::ClockSource must exist for the duration of any call; behaviour is
    undefined if no source is active.
  - **Non-negative elapsed**: elapsed() and elapsedTicks() return non-negative values as long as the underlying counter
    is monotonically non-decreasing, which is guaranteed by \ref toy::chrono::SteadyClock.
  - **Exception safety**: all operations are \c noexcept.

  \sa \ref toy::chrono::CountdownTimer, \ref toy::chrono::SteadyClock
*/
class Stopwatch {
public:
  /// Arithmetic type used to represent a raw tick count.
  using rep      = SteadyClock::rep;
  /// Duration type returned by elapsed(); parameterized on \ref toy::chrono::SteadyClock::period.
  using duration = SteadyClock::duration;

  /*!
    \brief Constructs and starts the stopwatch at the current tick count.

    \pre An active \ref toy::chrono::ClockSource exists in the current process.

    \post elapsedTicks() returns a non-negative value for all subsequent calls.

    \sa reset(), elapsed()
  */
  Stopwatch() noexcept;

  /*!
    \brief Elapsed time since construction or the last reset().

    \return Duration from the captured start tick to the current tick.

    \pre An active \ref toy::chrono::ClockSource exists in the current process.

    \sa elapsedTicks(), reset()
  */
  [[nodiscard]] duration elapsed() const noexcept;

  /*!
    \brief Raw tick count elapsed since construction or the last reset().

    Bypasses unit conversion; use for micro-benchmarks or when raw ticks are needed.

    \return Ticks elapsed from the captured start tick to the current tick.

    \pre An active \ref toy::chrono::ClockSource exists in the current process.

    \sa elapsed(), reset()
  */
  [[nodiscard]] rep elapsedTicks() const noexcept;

  /*!
    \brief Resets the start point to the current tick count.

    \pre An active \ref toy::chrono::ClockSource exists in the current process.

    \post elapsed() and elapsedTicks() measure from the moment reset() was called.

    \sa elapsed(), elapsedTicks()
  */
  void reset() noexcept;

private:
  /// Tick count captured at construction or the last reset().
  rep _startTicks;
};

} // namespace toy::chrono

#endif // INCLUDE_CORE_CHRONO_STOPWATCH_HPP_
