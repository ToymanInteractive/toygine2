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
  \file   log_sink.hpp
  \brief  Abstract sink interface for the logger.

  Defines \ref toy::log::ISink: the polymorphic output destination invoked by \ref toy::LogBackend on every flushed
  \ref toy::log::Record. Concrete sinks (console, mGBA, custom) implement this interface.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_LOG_SINK_HPP_
#define INCLUDE_CORE_LOG_SINK_HPP_

namespace toy::log {

/*!
  \class ISink

  \brief Abstract output destination for formatted log messages.

  Implemented by each concrete sink. \ref toy::LogBackend holds a pointer to the active sink and calls write() for every
  flushed \ref toy::log::Record. The interface is intentionally minimal: a single \c noexcept virtual method with no
  buffering or formatting contract.

  \section features Key Features

  - **noexcept write**: Sinks must not throw; the log path is entirely exception-free.
  - **Pointer-based**: Backend holds \c ISink* so sinks can be swapped at runtime.
  - **Minimal interface**: One virtual method; no buffering contract imposed on implementers.
  - **Virtual dispatch**: Called from the backend flush path, not the hot encoding path.

  \section usage Usage Example

  \code
  #include "core.hpp"

  struct StderrSink : toy::log::ISink {
    void write(const char * message, size_t length) noexcept override {
      std::fwrite(message, 1, length, stderr);
    }
  };
  \endcode

  \section performance Performance Characteristics

  - **write**: One indirect call; cost dominated by the concrete sink's I/O path.
  - **Memory usage**: One vtable pointer per instance.

  \section safety Safety Guarantees

  - **Exception safety**: write() is \c noexcept; destructor defaults to \c noexcept.
  - **Lifetime**: The sink must outlive any \ref toy::LogBackend that references it.
  - **Thread safety**: Not enforced; concrete sinks must document their own thread guarantees.

  \sa \ref toy::ConsoleSink, \ref toy::MgbaSink, \ref toy::LogBackend
*/
class ISink {
public:
  /// Default constructor.
  ISink() noexcept = default;

  /// Virtual destructor; defined out-of-line to anchor the vtable in a single translation unit.
  virtual ~ISink() noexcept;

  /// Copy construction is deleted to prevent slicing of derived sinks.
  ISink(const ISink &) = delete;

  /// Move construction is deleted; sinks are referenced by stable pointer in the backend.
  ISink(ISink &&) = delete;

  /// Copy assignment is deleted to prevent slicing of derived sinks.
  ISink & operator=(const ISink &) = delete;

  /// Move assignment is deleted; sinks are referenced by stable pointer in the backend.
  ISink & operator=(ISink &&) = delete;

  /*!
    \brief Writes \a length bytes from \a message to the output destination.

    \param message Pointer to the formatted log message.
    \param length  Byte length of \a message, excluding the null terminator.

    \pre \a message must not be null when \a length \c > \c 0.
  */
  virtual void write(const char * message, size_t length) noexcept = 0;
};

} // namespace toy::log

#endif // INCLUDE_CORE_LOG_SINK_HPP_
