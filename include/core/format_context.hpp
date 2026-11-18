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
  \file   format_context.hpp
  \brief  Character sink that formatting code writes its output through.

  Defines \ref toy::FormatContext, a caller-owned context pointer paired with a write callback, so output lands in
  caller storage without std::function, virtual dispatch, or allocation.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_CONTEXT_HPP_
#define INCLUDE_CORE_FORMAT_CONTEXT_HPP_

namespace toy {

/*!
  \class FormatContext
  \brief Character sink that pairs a caller-owned context with a write callback.

  A formatter emits through the stored callback and never learns what the context points at, so the same formatting
  code fills any caller storage, from a fixed buffer to a log record. The object holds two pointers and allocates
  nothing.

  \section format_context_features Key Features

  * **Type-erased output**: one callback signature serves every destination; the context pointer rides along
    uninterpreted.
  * **Zero-overhead dispatch**: a plain function pointer, no std::function and no virtual call.
  * **Constexpr construction**: the object builds in a constant expression.
  * **Trivially copyable**: two scalars, so copies and moves are free.

  \section format_context_usage Usage Example

  \code
  #include "core.hpp"

  struct Sink {
    char   buffer[16];
    size_t length = 0;
  };

  // Writes the range only when it fits whole.
  void append(void * context, const char * data, size_t count) noexcept {
    auto & sink = *static_cast<Sink *>(context);
    if (sink.length + count > 16)
      return;
    for (size_t i = 0; i < count; ++i)
      sink.buffer[sink.length++] = data[i];
  }

  // Returns false when the sink did not take the whole greeting.
  bool writeGreeting(Sink & sink) {
    const size_t start = sink.length;
    toy::FormatContext output{&sink, &append};
    output.write("hi", 2);
    output.put('!');
    return sink.length - start == 3;
  }
  \endcode

  \section format_context_performance Performance Characteristics

  * **Construction**: constant time.
  * **write()**: one indirect call for a positive count, none for \c 0; \a data and \a count pass through untouched.
  * **put()**: one indirect call with the address of its character and a count of \c 1.

  \section format_context_safety Safety Guarantees

  * **Contracts**: with assertions enabled, assert_message checks a null callback at construction and a null \a data
    with a positive count in write(); a registered handler decides whether a failed check continues, and with none
    registered the check stops the run. A shipping build skips both checks: a stored null callback makes the next
    dispatching write() or put() undefined behavior, and a null \a data reaches the callback unchecked.
  * **Bounds safety**: write() passes \a count through; bounding the copy is the callback's task.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \note The context is borrowed: the object and every copy store the address, and the pointee must stay valid for
        every dispatching call; a stale context reaches the callback as a dangling pointer.
  \note The class never dereferences the context; \c nullptr stays valid for a callback that accepts it.
  \note write() and put() allocate nothing and lock nothing; real-time legality is the callback's property.
  \note Concurrent dispatch, including through copies, is safe exactly when the callback is; the class adds no
        synchronization.

  \sa \ref toy::FormatString
*/
class FormatContext {
public:
  /*!
    \brief Callback write() and put() forward their characters to.

    Receives the context exactly as passed at construction, and a readable range of \a count bytes at \a data for the
    duration of the call. The pointer type carries \c noexcept, so a function that can throw does not fit.

    \param context  Caller-owned value from the constructor; only the callback interprets it.
    \param data     Bytes to emit.
    \param count    Number of bytes at \a data.
  */
  using WriteFunction = void (*)(void * context, const char * data, size_t count) noexcept;

  /*!
    \brief Couples \a context with the callback that receives it on every write.

    \param context  Value forwarded to each callback invocation; only the callback reads it.
    \param writeFn  Callback write() and put() dispatch through.

    \pre \a writeFn is non-null, checked by assert_message in debug builds.

    \post The object holds both values; a copy writes through the same pair.

    \note Where assertions are enabled, a failed check in a constant expression makes the construction non-constant,
          so the build fails.
  */
  constexpr FormatContext(void * context, WriteFunction writeFn) noexcept;

  /*!
    \brief Forwards a character range to the callback.

    A zero count returns without calling the callback; \a data may be null in that case.

    \param data  Bytes to emit.
    \param count Number of bytes at \a data.

    \pre For a positive \a count, \a data names \a count readable bytes for the duration of the callback; only the
         null check is enforced, by assert_message in debug builds.

    \post For a positive count the callback has run once with the constructor's context, \a data, and \a count.

    \sa put()
  */
  void write(const char * data, size_t count) noexcept;

  /*!
    \brief Writes one character through the callback.

    \param character Byte to emit.

    \post The callback has run once with the address of \a character and a count of \c 1.
  */
  void put(char character) noexcept;

private:
  /// Value handed to every callback invocation; only the callback reads it.
  void * _context;

  /// Callback write() and put() dispatch through.
  WriteFunction _writeFn;
};

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_CONTEXT_HPP_
