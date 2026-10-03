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
  \file   log_isink.hpp
  \brief  Interface a log destination implements to receive formatted records.

  Defines \ref toy::log::ISink, the abstract base a sink derives from: a caller hands it one \ref toy::log::Record at a
  time, and the derived class decides where the text goes.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_LOG_SINK_HPP_
#define INCLUDE_CORE_LOG_SINK_HPP_

#include "log_record.hpp"

namespace toy::log {

/*!
  \class ISink
  \brief Abstract destination of formatted log records.

  Interface with one operation: write() hands the sink a \ref toy::log::Record, and the derived class decides what
  reaches its destination and whether it keeps the text. The interface holds no state, so the cost of a call and every
  guarantee beyond it belong to the derived sink.

  \section sink_features Key Features

  * **Runtime polymorphism**: a caller writes through a \ref toy::log::ISink reference, so the destination changes
    without recompiling the caller.
  * **Fixed address**: copy and move are deleted, so a sink stays where its callers reference it.
  * **No failure path**: write() is \c noexcept and returns nothing, so a call site handles no error.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section sink_usage Usage Example

  \code
  #include "core.hpp"

  class LastRecordSink final : public toy::log::ISink {
  public:
    LastRecordSink() noexcept = default;

    ~LastRecordSink() noexcept override = default;

    TOYGINE_NO_COPY_MOVE(LastRecordSink);

    void write(const toy::log::Record & record) noexcept override {
      _last = record;
    }

  private:
    toy::log::Record _last;
  };

  static constexpr toy::log::Metadata metadata{"asset {} loaded", __FILE__, __LINE__, toy::log::Level::Info};

  LastRecordSink   sink;
  toy::log::ISink & target = sink;
  target.write({.meta = &metadata, .timestamp = 0, .message = "asset player loaded"});
  \endcode

  \section sink_performance Performance Characteristics

  * **write()**: one virtual call, plus whatever the derived sink does with the record.
  * **Memory usage**: one pointer to the virtual table on common ABIs; a derived sink adds its own fields.

  \section sink_safety Safety Guarantees

  * **Lifetime**: the caller owns the record passed to write(), and the interface promises nothing about it once the
    call returns, so a sink keeps a copy, never a pointer.
  * **Contracts**: the interface checks nothing; each derived sink states what it requires of a record, a null
    \ref toy::log::Record::meta included.
  * **Memory safety**: the interface allocates nothing; a derived sink states its own allocation.
  * **Thread safety**: nothing synchronizes a call. Whether write() may run on several threads at once is a property of
    the derived sink.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section sink_compatibility Compatibility

  * The interface allocates nothing and calls into no platform API, so it suits embedded and retro targets; a derived
    sink states its own requirements.

  \sa \ref toy::log::Record
  \sa \ref toy::log::Metadata
*/
class ISink {
public:
  /// Constructs the interface part of a derived sink.
  ISink() noexcept = default;

  /// Destroys the sink; virtual, so deleting a derived sink through a pointer to its interface runs its own destructor.
  virtual ~ISink() noexcept;

  TOYGINE_NO_COPY_MOVE(ISink);

  /*!
    \brief Hands one formatted record to the sink.

    The derived sink decides what reaches its destination: the message text, the metadata the record points at, the
    time stamp, or any part of them.

    \param record Record to write, owned by the caller.

    \note Borrows \a record for the call only; a sink that keeps the message copies the record.
  */
  virtual void write(const Record & record) noexcept = 0;
};

} // namespace toy::log

#endif // INCLUDE_CORE_LOG_SINK_HPP_
