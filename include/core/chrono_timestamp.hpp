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
  \file   chrono_timestamp.hpp
  \brief  Monotonic millisecond stamp for ordering events within a process.

  Defines \ref toy::chrono::Timestamp, the strong type the logging backend stamps each record with. The count starts
  at toy::chrono::initialize() and carries no calendar meaning.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_CHRONO_TIMESTAMP_HPP_
#define INCLUDE_CORE_CHRONO_TIMESTAMP_HPP_

namespace toy::chrono {

/*!
  \class Timestamp
  \brief Milliseconds elapsed since toy::chrono::initialize(), as an ordered value.

  now() reads \ref toy::chrono::SteadyClock against the start ticks recorded at initialization, so stamps never step
  back within a process. Not a calendar time: rendering a stamp for a person is the consumer's job, and the type
  carries no date fields.

  \section timestamp_features Key Features

  * **Monotonic ordering**: stamps taken later in one process compare greater or equal, on every target.
  * **Strong type**: a bare millisecond number does not pass where a stamp is required.
  * **Constexpr support**: milliseconds() and the comparisons evaluate in a constant expression.
  * **Value semantics**: trivially copyable; a copy carries the same count.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section timestamp_usage Usage Example

  \code
  #include "core.hpp"

  const toy::chrono::Timestamp earlier = toy::chrono::Timestamp::now();
  const toy::chrono::Timestamp later   = toy::chrono::Timestamp::now();
  const bool ordered = earlier <= later;
  \endcode

  \section timestamp_performance Performance Characteristics

  * **Reading**: O(1); one counter read and integer arithmetic in now().
  * **Access**: O(1); milliseconds() returns the stored count.
  * **Memory usage**: 8 bytes, the millisecond count alone.

  \section timestamp_safety Safety Guarantees

  * **Contracts**: toy::chrono::initialize() runs before now(); the platform startup order covers it.
  * **Thread safety**: now() only reads the counter and the start ticks written once at startup; any thread may call
    it.
  * **Determinism**: counts differ between runs and targets; never feed them into simulation state.
  * **Memory safety**: nothing allocates.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section timestamp_compatibility Compatibility

  * On the stub console targets now() returns the zero stamp until real backends land, so stamps there carry no
    ordering information.

  \note A default-constructed stamp reads \c 0, the count at the initialization moment.

  \sa \ref toy::chrono::SteadyClock
  \sa \ref toy::chrono::CalendarTime
*/
class Timestamp {
public:
  /*!
    \brief Stamps the current moment.

    \return Milliseconds between toy::chrono::initialize() and this call, read from \ref toy::chrono::SteadyClock.

    \pre toy::chrono::initialize() has run; the platform startup order guarantees it for application code.

    \note Allocates nothing and never blocks.

    \sa milliseconds()
  */
  [[nodiscard]] static Timestamp now() noexcept;

  /*!
    \brief Reports the stored millisecond count.

    \return Milliseconds since toy::chrono::initialize(); \c 0 for a default-constructed stamp.

    \note Works in a constant expression.
  */
  [[nodiscard]] constexpr int64_t milliseconds() const noexcept;

  /*!
    \brief Orders two stamps by their millisecond counts.

    \param lhs Stamp on the left of the operator.
    \param rhs Stamp on the right of the operator.

    \return \c strong_ordering::less, \c equal, or \c greater as the count of \a lhs orders before, with, or
            after the count of \a rhs; the compiler derives \c ==, \c !=, and the relational operators from it.

    \note Works in a constant expression.
  */
  [[nodiscard]] friend constexpr strong_ordering operator<=>(const Timestamp & lhs,
                                                             const Timestamp & rhs) noexcept = default;

private:
  /*!
    \brief Builds a stamp from a millisecond count.

    \param milliseconds Milliseconds since toy::chrono::initialize().

    \return Stamp carrying \a milliseconds.
  */
  [[nodiscard]] static constexpr Timestamp fromMilliseconds(int64_t milliseconds) noexcept;

  /// Milliseconds since toy::chrono::initialize(), zero at the start
  int64_t _milliseconds{0};
};

} // namespace toy::chrono

#endif // INCLUDE_CORE_CHRONO_TIMESTAMP_HPP_
