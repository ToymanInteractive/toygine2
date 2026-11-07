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
  \file   chrono_stopwatch.hpp
  \brief  Elapsed-time measurement from a declared starting point.

  Defines \ref toy::chrono::Stopwatch, a value that starts counting at its declaration and reports the span since
  then. Used for benchmarks and one-off measurements over \ref toy::chrono::SteadyClock.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_CHRONO_STOPWATCH_HPP_
#define INCLUDE_CORE_CHRONO_STOPWATCH_HPP_

#include "chrono_steady_clock.hpp"

namespace toy::chrono {

/*!
  \class Stopwatch
  \brief Measures the span elapsed since its construction or last reset().

  A declaration starts the measurement: the default member initializer records the start ticks. Copying fixes the
  same starting point, so a copy reports the original span, not a fresh one.

  \section stopwatch_features Key Features

  * **Starts on declaration**: no start call; constructing the value records the starting counter reading.
  * **Restartable**: reset() moves the starting point to the current counter reading.
  * **Two readings**: elapsed() as a duration, elapsedTicks() as the raw tick difference.
  * **Value semantics**: trivially copyable; a copy shares the recorded starting point.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section stopwatch_usage Usage Example

  \code
  #include "core.hpp"

  const toy::chrono::Stopwatch watch;
  // ... measured work ...
  const auto elapsed = watch.elapsed();
  \endcode

  \section stopwatch_performance Performance Characteristics

  * **Reading**: O(1); one counter read, a subtraction, and a conversion where the counter does not tick in nanoseconds.
  * **Memory usage**: 8 bytes, the start tick alone.

  \section stopwatch_safety Safety Guarantees

  * **Contracts**: none; the platform startup order covers the counter (see toy::chrono::initialize()).
  * **Thread safety**: reading one instance from several threads is safe; reset() writes the start tick and needs
    external ordering against concurrent readers.
  * **Determinism**: readings differ between runs and targets; never feed them into simulation state.
  * **Memory safety**: nothing allocates.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section stopwatch_compatibility Compatibility

  * On the stub console targets the counter stays at zero, so every reading is zero until real backends land.

  \sa \ref toy::chrono::SteadyClock
*/
class Stopwatch {
public:
  /// Tick count type of \ref toy::chrono::SteadyClock
  using rep = SteadyClock::rep;

  /// Duration type of \ref toy::chrono::SteadyClock
  using duration = SteadyClock::duration;

  /*!
    \brief Reports the span since the starting point as a duration.

    \return Time elapsed since construction or the last reset(), in ticks of \ref toy::chrono::SteadyClock::period.

    \note Allocates nothing and never blocks.

    \sa elapsedTicks(), reset()
  */
  [[nodiscard]] duration elapsed() const noexcept;

  /*!
    \brief Reports the span since the starting point as a tick difference.

    \return Current reading of toy::chrono::SteadyClock::nowTicks() minus the recorded start, in that counter's unit.

    \sa elapsed(), reset()
  */
  [[nodiscard]] rep elapsedTicks() const noexcept;

  /*!
    \brief Moves the starting point to the current counter reading.

    \post elapsedTicks() counts from this call.
  */
  void reset() noexcept;

private:
  /// Counter reading the measurement counts from, taken at construction
  rep _startTicks{SteadyClock::nowTicks()};
};

} // namespace toy::chrono

#endif // INCLUDE_CORE_CHRONO_STOPWATCH_HPP_
