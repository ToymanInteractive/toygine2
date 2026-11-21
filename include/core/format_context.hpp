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
  * **Constexpr support**: construction and every write evaluate in a constant expression.
  * **Trivially copyable**: two scalars, so copies and moves are free.

  \section format_context_usage Usage Example

  \code
  #include "core.hpp"

  struct Sink {
    char   buffer[16];
    size_t length = 0;
  };

  // Writes the view only when it fits whole.
  void append(void * context, toy::span<const char> data) noexcept {
    auto & sink = *static_cast<Sink *>(context);
    if (sink.length + data.size() > 16)
      return;
    for (const char character : data)
      sink.buffer[sink.length++] = character;
  }

  // Returns false when the sink did not take the whole greeting.
  bool writeGreeting(Sink & sink) {
    const size_t       start = sink.length;
    toy::FormatContext output{&sink, &append};
    output.write(toy::span<const char>{"hi", 2});
    output.put('!');
    return sink.length - start == 3;
  }
  \endcode

  \section format_context_performance Performance Characteristics

  * **Construction**: constant time.
  * **write()**: one indirect call for a non-empty view, none when it is empty; \a data passes through untouched.
  * **put()**: one indirect call with a one-character view of its character.

  \section format_context_safety Safety Guarantees

  * **Contracts**: with assertions enabled, assert_message checks a null callback at construction and a null pointer in
    a non-empty write() view; a registered handler decides whether a failed check continues, and with none registered
    the check stops the run. A shipping build skips both checks: a stored null callback makes the next dispatching
    write() or put() undefined behavior, and a null pointer in a non-empty view reaches the callback unchecked.
  * **Bounds safety**: write() passes the view through; bounding the copy is the callback's task.
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

    Receives the context exactly as passed at construction, and the readable characters of \a data for the duration of
    the call. The pointer type carries \c noexcept, so a function that can throw does not fit.

    \param context  Caller-owned value from the constructor; only the callback interprets it.
    \param data     Characters to emit.
  */
  using WriteFunction = void (*)(void * context, span<const char> data) noexcept;

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

    An empty view returns without calling the callback; its pointer may be null in that case.

    \param data  Characters to emit.

    \pre \a data names readable characters for the duration of the callback.
    \pre \a data is empty or names a non-null pointer, checked by assert_message in debug builds.

    \post For a non-empty view the callback has run once with the constructor's context and \a data.

    \sa put()
  */
  constexpr void write(span<const char> data) noexcept;

  /*!
    \brief Writes one character through the callback.

    \param character Byte to emit.

    \post The callback has run once with a one-character view of \a character.

    \sa write(span<const char>)
  */
  constexpr void put(char character) noexcept;

private:
  /// Value handed to every callback invocation; only the callback reads it.
  void * _context;

  /// Callback write() and put() dispatch through.
  WriteFunction _writeFn;
};

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_CONTEXT_HPP_
