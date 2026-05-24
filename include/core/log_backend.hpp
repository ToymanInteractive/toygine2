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
  \file   log_backend.hpp
  \brief  Singleton transport that owns the active sink and dispatches log records.

  Defines \ref toy::log::Backend: the process-wide entry point for \c LOG_* macros. Holds a pointer to the active
  \ref toy::log::ISink and a timestamp policy callback; \c push() formats every \ref toy::log::Record on the caller's
  stack and forwards it synchronously to the installed sink.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_LOG_BACKEND_HPP_
#define INCLUDE_CORE_LOG_BACKEND_HPP_

#include "log_level.hpp"
#include "log_metadata.hpp"
#include "log_record.hpp"
#include "log_sink.hpp"

namespace toy::log {

/*!
  \class Backend

  \ingroup Logger

  \brief Process-wide singleton transport for log records.

  Owns the active \ref toy::log::ISink and the timestamp policy used to stamp each pushed record. Constructed once per
  process (typically as a member of \ref toy::CoreApplication); a debug assertion fires if a second instance is created
  while one is already active. \c LOG_* macros call \c Backend::instance().push() to format the message on the caller's
  stack and dispatch the resulting record synchronously to the installed sink.

  \section features Key Features

  - **RAII singleton**: lifetime tied to a stack or member object; debug assertion enforces uniqueness.
  - **Pluggable sink**: setSink() swaps the destination at runtime; null sink silently drops records.
  - **Pluggable timestamp**: setTimestampPolicy() installs a callback; defaults to a zero-returning stub.
  - **Synchronous dispatch**: push() formats and writes inline; no ring buffer or background thread in v1.
  - **No allocation**: \ref toy::log::Record is constructed on the caller's stack.
  - **Noexcept**: every method is \c noexcept; the log path is exception-free.

  \section usage Usage Example

  \code
  #include "core.hpp"

  toy::log::Backend backend;                       // RAII; registers as active
  toy::ConsoleSink sink;
  backend.setSink(&sink);

  static constexpr toy::log::Metadata meta{"player at ({}, {})", __FILE__, __LINE__, toy::log::Level::Info};
  toy::log::Backend::instance().push(&meta, 10, 20);
  \endcode

  \section performance Performance Characteristics

  - **push()**: O(n) where n is the formatted message length; bounded by \c LOG_MESSAGE_CAPACITY.
  - **setSink() / setTimestampPolicy()**: O(1) pointer store.
  - **instance()**: O(1) static load.
  - **Memory usage**: one sink pointer plus one function pointer.

  \section safety Safety Guarantees

  - **Singleton invariant**: debug assertion fires on a second instance; undefined behavior in release builds if
    violated.
  - **Lifetime contract**: the installed sink and any state captured by the timestamp policy must outlive the backend.
  - **Exception safety**: all operations are \c noexcept.
  - **Thread safety**: not thread-safe in v1; serialize push() externally until the ring-buffer transport lands.

  \note Move and copy operations are deleted; instances are pinned to their declaration scope.

  \warning The default timestamp policy returns \c 0. Install a real policy via setTimestampPolicy() before relying on
           \ref toy::log::Record::timestamp.

  \sa \ref toy::log::ISink, \ref toy::log::Record, \ref toy::log::Metadata, \ref toy::ConsoleSink
*/
class Backend {
public:
  /// Signature for the timestamp callback installed via setTimestampPolicy().
  using timestamp_fn_type = uint32_t (*)() noexcept;

  /*!
    \brief Registers this instance as the process-wide active backend.

    \pre No other \ref toy::log::Backend instance is currently alive in the process (checked by a debug assertion;
         undefined behavior in release builds if violated).

    \post instance() returns a reference to this object, sink() is \c nullptr, and timestampPolicy() returns
          \c &defaultTimestamp.

    \sa ~Backend(), instance()
  */
  Backend() noexcept;

  /*!
    \brief Deregisters this instance as the active backend.

    \pre No further calls to push() are made after this destructor runs.

    \post instance() asserts in debug; behavior is undefined in release builds.

    \sa Backend()
  */
  ~Backend() noexcept;

  /// Copy and move are deleted; at most one active backend may be registered at a time.
  TOYGINE_NO_COPY_MOVE(Backend);

  /*!
    \brief Returns the currently registered backend.

    \return Reference to the live \ref toy::log::Backend instance.

    \pre A \ref toy::log::Backend is alive (checked by a debug assertion; undefined behavior in release builds if
         violated).

    \sa Backend()
  */
  [[nodiscard]] static Backend & instance() noexcept;

  /*!
    \brief Installs \a sink as the dispatch target for subsequent push() calls.

    \param sink Sink to receive records, or \c nullptr to drop records silently.

    \post sink() returns \a sink.

    \note The previously installed sink, if any, is not destroyed; ownership remains with the caller.

    \sa sink(), push()
  */
  void setSink(ISink * sink) noexcept;

  /*!
    \brief Currently installed sink.

    \return Pointer to the installed sink, or \c nullptr if none.

    \sa setSink()
  */
  [[nodiscard]] ISink * sink() const noexcept;

  /*!
    \brief Installs \a fn as the timestamp policy used to stamp each pushed record.

    \param fn Callback returning the timestamp for the next record, or \c nullptr to restore the default
              zero-returning stub.

    \post timestampPolicy() returns the installed callback (never \c nullptr).

    \sa timestampPolicy(), defaultTimestamp()
  */
  void setTimestampPolicy(timestamp_fn_type fn) noexcept;

  /*!
    \brief Currently installed timestamp policy.

    \return Function pointer used to stamp records; never \c nullptr.

    \sa setTimestampPolicy()
  */
  [[nodiscard]] timestamp_fn_type timestampPolicy() const noexcept;

  /*!
    \brief Default timestamp policy.

    Installed automatically by the constructor and restored when setTimestampPolicy() is called with \c nullptr. Always
    returns \c 0, so \ref toy::log::Record::timestamp is meaningless until a real policy is installed.

    \return Always \c 0.

    \sa setTimestampPolicy()
  */
  [[nodiscard]] static uint32_t defaultTimestamp() noexcept;

  /*!
    \brief Formats \a args into a \ref toy::log::Record and dispatches it to the installed sink.

    Stamps the record with timestampPolicy()(), then forwards the call-site format pattern through toy::vformatTo()
    into the record's message buffer. Synchronously invokes \ref toy::log::ISink::write() on the installed sink.
    Returns early if no sink is installed.

    \tparam Args Argument pack forwarded to toy::vformatTo().
    \param  meta Pointer to a \c static \c constexpr \ref toy::log::Metadata describing the call site.
    \param  args Format arguments matching the placeholders in \c meta->format.

    \pre \a meta must be non-null and reference a \c static \c constexpr \ref toy::log::Metadata.

    \note Synchronous in v1: write() runs on the caller's thread before push() returns.

    \sa setSink(), toy::vformatTo()
  */
  template <typename... Args>
  void push(const Metadata * meta, const Args &... args) noexcept;

private:
  /// Active sink; \c nullptr drops every pushed record. Owned by the caller; the backend never destroys it.
  ISink * _sink{nullptr};

  /// Active timestamp policy; never \c nullptr (defaults to defaultTimestamp, restored on setTimestampPolicy(nullptr)).
  timestamp_fn_type _timestampFn;
};

} // namespace toy::log

#endif // INCLUDE_CORE_LOG_BACKEND_HPP_
