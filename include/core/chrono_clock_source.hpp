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
  \file   chrono_clock_source.hpp
  \brief  RAII owner of the platform hardware time source.

  Defines \ref toy::chrono::ClockSource: the process-wide owner of the hardware counter that backs every chrono utility
  in \ref toy::chrono. Exactly one instance must be alive before any \ref toy::chrono::SteadyClock,
  \ref toy::chrono::Stopwatch, or \ref toy::chrono::CountdownTimer call, and it must outlive all of them.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_CHRONO_CLOCK_SOURCE_HPP_
#define INCLUDE_CORE_CHRONO_CLOCK_SOURCE_HPP_

namespace toy::chrono {

/*!
  \class ClockSource

  \ingroup Chrono

  \brief RAII owner of the platform hardware time source.

  Acquires the hardware counter on construction and releases it on destruction. A process-wide singleton: at most one
  \c ClockSource may exist at any given time; a debug assertion fires if a second instance is constructed while one is
  already active. Typically created as a local variable in \c main() or as a member of an engine root object.

  \section features Key Features

  - **RAII lifetime**: hardware resources are acquired on construction and released on destruction.
  - **Singleton invariant**: enforced by a debug assertion; undefined behaviour if violated in release builds.
  - **Zero allocation**: 8-byte value type, no heap use.
  - **Noexcept API**: all operations are \c noexcept; no exceptions are thrown.
  - **Backend abstraction**: the active hardware counter is selected at compile time; call sites are platform-agnostic.

  \section usage Usage Example

  \code
  #include "core.hpp"

  int main() {
    toy::chrono::ClockSource clock; // hardware acquired
    toy::chrono::Stopwatch sw;
    // ...
  } // clock destroyed, hardware released
  \endcode

  \section performance Performance Characteristics

  - **Construction**: O(1) on most backends; some backends may block briefly to calibrate the counter frequency.
  - **Destruction**: O(1).
  - **nowTicks()**: O(1), typically a single hardware counter read.
  - **frequency()**: O(1), returns a cached field.
  - **Memory usage**: 8 bytes (\c int64_t frequency field).

  \section safety Safety Guarantees

  - **Singleton invariant**: debug assertion prevents construction of a second active instance; undefined behaviour in
    release builds if violated.
  - **Lifetime contract**: must outlive every dependent \ref toy::chrono::SteadyClock, \ref toy::chrono::Stopwatch, and
    \ref toy::chrono::CountdownTimer instance.
  - **Exception safety**: all operations are \c noexcept; no exceptions are thrown.
  - **Thread safety**: not thread-safe; construct and destroy on the owning thread only. Concurrent reads via nowTicks()
    and frequency() from worker threads are permitted once construction has completed and a happens-before edge is
    established.

  \note Move and copy operations are deleted; instances are pinned to their declaration scope.

  \warning Must outlive all \ref toy::chrono::Stopwatch and \ref toy::chrono::CountdownTimer instances that depend on
           it.

  \sa \ref toy::chrono::SteadyClock, \ref toy::chrono::Stopwatch, \ref toy::chrono::CountdownTimer
*/
class ClockSource {
public:
  /*!
    \brief Initialises the hardware counter and registers this instance as the active source.

    \pre No other \ref toy::chrono::ClockSource instance is currently alive in the process (checked by a debug
         assertion; undefined behaviour in release builds if violated).

    \post The hardware counter is running, frequency() returns a positive value, and nowTicks() returns monotonically
          non-decreasing values.

    \note May block briefly on backends that need to calibrate the counter frequency at startup.

    \sa ~ClockSource(), nowTicks(), frequency()
  */
  ClockSource();

  /*!
    \brief Releases the hardware counter and deregisters this instance.

    \pre All \ref toy::chrono::SteadyClock, \ref toy::chrono::Stopwatch, and \ref toy::chrono::CountdownTimer instances
         that depend on this source have already been destroyed.

    \post No active source is registered; subsequent calls to nowTicks() from any surviving dependent are undefined
          behaviour.

    \sa ClockSource()
  */
  ~ClockSource();

  /// Copy construction is deleted to preserve the singleton invariant.
  ClockSource(const ClockSource &)             = delete;
  /// Copy assignment is deleted to preserve the singleton invariant.
  ClockSource & operator=(const ClockSource &) = delete;
  /// Move construction is deleted; instances are pinned to their declaration scope.
  ClockSource(ClockSource &&)                  = delete;
  /// Move assignment is deleted; instances are pinned to their declaration scope.
  ClockSource & operator=(ClockSource &&)      = delete;

  /*!
    \brief Tick frequency of the hardware counter in Hz.

    Cached during construction; constant for the lifetime of the object. Combine with nowTicks() to convert raw tick
    deltas into seconds or any \ref toy::chrono::Duration period.

    \return Ticks per second; always strictly greater than \c 0 after construction completes.

    \note Inline accessor; reduces to a single field load.

    \sa nowTicks()
  */
  [[nodiscard]] int64_t frequency() const noexcept;

  /*!
    \brief Current hardware counter value.

    \return Raw tick count relative to an unspecified epoch; monotonically non-decreasing within a single thread.

    \note The epoch is implementation-defined and may differ between processes and across reboots; only differences
          between two values produced by the same \ref toy::chrono::ClockSource instance are meaningful.

    \sa frequency()
  */
  [[nodiscard]] int64_t nowTicks() const noexcept;

private:
  /// Cached hardware counter frequency in Hz; set once during construction.
  int64_t _frequency{0};
};

} // namespace toy::chrono

#endif // INCLUDE_CORE_CHRONO_CLOCK_SOURCE_HPP_
