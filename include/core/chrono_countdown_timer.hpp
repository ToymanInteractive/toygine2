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
  \file   chrono_countdown_timer.hpp
  \brief  Deadline-based countdown timer over the active \ref toy::chrono::SteadyClock.

  Defines \ref toy::chrono::CountdownTimer: an 8-byte, trivially copyable timer that stores a deadline tick count and
  reports remaining time and expiry state relative to \ref toy::chrono::SteadyClock.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_CHRONO_COUNTDOWN_TIMER_HPP_
#define INCLUDE_CORE_CHRONO_COUNTDOWN_TIMER_HPP_

#include "chrono_steady_clock.hpp"

namespace toy::chrono {

/*!
  \class CountdownTimer

  \ingroup Chrono

  \brief Deadline-based countdown timer anchored to the active \ref toy::chrono::SteadyClock.

  Computes a deadline tick count on construction as the current tick count plus the given timeout, then reports
  remaining time and expiry state on demand. Trivially copyable: copying a \c CountdownTimer produces an independent
  timer with the same deadline.

  \section features Key Features

  - **Trivially copyable**: safe to copy with \c memcpy; no destructor or heap involvement.
  - **8-byte footprint**: single \c int64_t field; suitable for stack, struct-of-arrays, and embedded targets.
  - **Clamped remaining()**: remaining() returns \c duration{0} once the deadline has passed; never negative.
  - **Restartable**: restart() resets the deadline from the current moment without constructing a new object.

  \section usage Usage Example

  \code
  #include "core.hpp"

  toy::chrono::ClockSource clock;

  toy::chrono::CountdownTimer timer{std::chrono::milliseconds{500}};
  if (timer.expired()) {
    // handle timeout
  }
  const auto left = toy::chrono::duration_cast<std::chrono::milliseconds>(timer.remaining());
  \endcode

  \section performance Performance Characteristics

  - **Construction**: O(1); one counter read via \ref toy::chrono::SteadyClock::nowTicks().
  - **remaining()**: O(1); one counter read and one comparison with conditional subtraction.
  - **expired()**: O(1); one counter read and one comparison.
  - **restart()**: O(1); one counter read and one addition.
  - **Memory usage**: 8 bytes.

  \section safety Safety Guarantees

  - **Precondition**: an active \ref toy::chrono::ClockSource must exist for the duration of any call; behaviour is
    undefined if no source is active.
  - **Non-negative remaining()**: remaining() is clamped to \c duration{0} after expiry; the result is always
    non-negative.
  - **Exception safety**: all operations are \c noexcept.

  \sa \ref toy::chrono::Stopwatch, \ref toy::chrono::SteadyClock
*/
class CountdownTimer {
public:
  /// Arithmetic type used to represent a raw tick count.
  using rep      = SteadyClock::rep;
  /// Duration type used by remaining() and restart(); parameterized on \ref toy::chrono::SteadyClock::period.
  using duration = SteadyClock::duration;

  /*!
    \brief Constructs the timer with a deadline at the current time plus \a timeout.

    \param timeout Duration from now until the deadline.

    \pre An active \ref toy::chrono::ClockSource exists in the current process.
    \pre \a timeout must be non-negative; a negative value produces an immediately expired timer.

    \post expired() returns \c false for \a timeout ticks from construction; remaining() returns a value ≤ \a timeout.

    \sa restart(), expired(), remaining()
  */
  explicit CountdownTimer(duration timeout) noexcept;

  /*!
    \brief Time remaining until the deadline.

    \return Duration until the deadline, or \c duration{0} if the deadline has already passed.

    \pre An active \ref toy::chrono::ClockSource exists in the current process.

    \sa expired(), restart()
  */
  [[nodiscard]] duration remaining() const noexcept;

  /*!
    \brief Whether the deadline has passed.

    \return \c true if the current tick count has reached or exceeded the deadline; \c false otherwise.

    \pre An active \ref toy::chrono::ClockSource exists in the current process.

    \sa remaining(), restart()
  */
  [[nodiscard]] bool expired() const noexcept;

  /*!
    \brief Resets the deadline to the current time plus \a timeout.

    \param timeout Duration from now until the new deadline.

    \pre An active \ref toy::chrono::ClockSource exists in the current process.
    \pre \a timeout must be non-negative; a negative value produces an immediately expired timer.

    \post expired() returns \c false for \a timeout ticks from the call; remaining() returns a value ≤ \a timeout.

    \sa expired(), remaining()
  */
  void restart(duration timeout) noexcept;

private:
  /// Deadline expressed as an absolute tick count from \ref toy::chrono::SteadyClock.
  rep _deadlineTicks;
};

} // namespace toy::chrono

#endif // INCLUDE_CORE_CHRONO_COUNTDOWN_TIMER_HPP_
