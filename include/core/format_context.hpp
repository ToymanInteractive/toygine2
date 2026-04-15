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
  \file   format_context.hpp
  \brief  Type-erased write sink for the runtime formatting path.

  Defines \ref toy::FormatContext, the concrete type used by toy::vformat() and toy::vformatTo() to write formatted
  output without exposing the backing storage type.

  \note Included by core.hpp; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_CONTEXT_HPP_
#define INCLUDE_CORE_FORMAT_CONTEXT_HPP_

namespace toy {

/*!
  \class FormatContext

  \ingroup String

  \brief Type-erased write sink.

  Stores a \c void* context and a \c noexcept function-pointer callback so that the formatting engine can append bytes
  to any backing storage without knowing its concrete type. Used by toy::vformat() and toy::vformatTo() as the output
  target; both write() and put() delegate to the stored callback.

  \section features Key Features

  - **Type erasure**: Decouples the formatting loop from the concrete output type via a function pointer.
  - **Minimal footprint**: Two-pointer aggregate; constructed on the stack at each call site.
  - **No allocation**: Does not perform any dynamic allocation.
  - **noexcept guarantee**: Both write() and put() are \c noexcept; the stored callback must also be \c noexcept.

  \section usage Usage Example

  \code
  #include "core.hpp"

  toy::FixedString<64> buf;
  toy::FormatContext out{
    static_cast<void *>(&buf),
    [](void * ctx, const char * data, size_t n) noexcept {
      static_cast<toy::FixedString<64> *>(ctx)->append(data, n);
    }
  };
  out.write("hello", 5);
  out.put('!');
  \endcode

  \section performance Performance Characteristics

  - **Construction**: O(1) constant time; stores two pointers.
  - **write / put**: One indirect call through the stored function pointer.

  \section safety Safety Guarantees

  - **Exception safety**: write() and put() are \c noexcept; the stored callback must honour this contract.
  - **Lifetime**: The context pointer must remain valid for the lifetime of the object.
  - **Null safety**: Both the context pointer and the write callback must be non-null before write() or put() is called.

  \sa FormatArgument
*/
class FormatContext {
public:
  /// Type of the write callback.
  using WriteFunction = void (*)(void * context, const char * data, size_t count) noexcept;

  /*!
    \brief Constructs a FormatContext with a context pointer and write callback.

    \param context Opaque pointer to the underlying output object passed unchanged to \a writeFn.
    \param writeFn Callback that appends \a count bytes from \a data into the object at \a context.

    \pre \a writeFn must not be null.
    \pre \a context must remain valid for the lifetime of this object.

    \post write() and put() delegate to \a writeFn with \a context as the first argument.
  */
  constexpr FormatContext(void * context, WriteFunction writeFn) noexcept;

  /*!
    \brief Appends \a count bytes from \a data to the output.

    \param data  Pointer to the character data to append. Must not be null if \a count \c > \c 0.
    \param count Number of bytes to append.

    \sa put(char)
  */
  void write(const char * data, size_t count) noexcept;

  /*!
    \brief Appends a single character to the output.

    \param character The character to append.

    \sa write(const char *, size_t)
  */
  void put(char character) noexcept;

private:
  /// Opaque pointer to the backing output object.
  void *        _context;
  /// Callback invoked by write() and put() to append data.
  WriteFunction _writeFn;
};

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_CONTEXT_HPP_
