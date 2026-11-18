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

  void append(void * context, const char * data, size_t count) noexcept {
    auto & sink = *static_cast<Sink *>(context);
    for (size_t i = 0; i < count && sink.length < 16; ++i)
      sink.buffer[sink.length++] = data[i];
  }

  Sink sink;
  toy::FormatContext output{&sink, &append};
  output.write("hi", 2);
  output.put('!');
  \endcode

  \section format_context_performance Performance Characteristics

  * **Construction**: constant time.
  * **write()**: one indirect call carrying \a data and \a count untouched.
  * **put()**: one indirect call with the address of its character and a count of \c 1.

  \section format_context_safety Safety Guarantees

  * **Contracts**: a null callback fails at construction and a null \a data with a positive count fails in write(),
    both under assert_message in debug builds.
  * **Bounds safety**: write() passes \a count through; bounding the copy is the callback's task.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \note The context pointer is never dereferenced by this class; its type and lifetime belong to the callback.

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
  */
  constexpr FormatContext(void * context, WriteFunction writeFn) noexcept;

  /*!
    \brief Forwards a character range to the callback.

    A zero count returns without calling the callback; \a data may be null in that case.

    \param data  Bytes to emit.
    \param count Number of bytes at \a data.

    \pre \a data is non-null when \a count is positive, checked by assert_message in debug builds.

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
