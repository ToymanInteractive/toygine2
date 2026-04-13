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
  \brief  Typed format context that binds a formatting pass to a concrete output backend.

  Defines \ref toy::FormatContext: a zero-overhead wrapper that holds a reference to an \ref toy::OStringStreamBackend
  storage and exposes write() / put() for the formatting engine.

  \note Included by core.hpp; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_CONTEXT_HPP_
#define INCLUDE_CORE_FORMAT_CONTEXT_HPP_

#include "o_string_stream.hpp"

namespace toy {

/*!
  \class FormatContext

  \ingroup String

  \brief Zero-overhead format context that binds a formatting pass to a concrete output backend.

  Holds a reference to an \ref toy::OStringStreamBackend storage and forwards write() / put() calls directly to the
  backend's append / push_back methods.

  \tparam BackendType The concrete output storage type. Must satisfy \ref toy::OStringStreamBackend.

  \section features Key Features

  - **Zero overhead**: Calls backend methods directly; no function-pointer indirection.
  - **Typed backend access**: backend() exposes the underlying storage for inspection or advanced use.
  - **constexpr**: All operations are \c constexpr when \a BackendType allows it.
  - **noexcept**: Every member is \c noexcept, matching the \ref toy::OStringStreamBackend contract.

  \section usage Usage Example

  \code
  #include "core.hpp"

  toy::FixedString<64> buf;
  toy::FormatContext<toy::FixedString<64>> ctx(buf);
  ctx.write("hello", 5);
  ctx.put(' ');
  ctx.write("world", 5);
  // buf == "hello world"
  \endcode

  \section performance Performance Characteristics

  - **Construction**: O(1) constant time; stores one reference.
  - **write()**: O(n) where \a n is \a count; forwards to backend append(const char *, size_t).
  - **put()**: O(1) constant time; forwards to backend push_back(char).
  - **backend()**: O(1) constant time; returns a reference.

  \section safety Safety Guarantees

  - **Exception safety**: All operations are \c noexcept (guaranteed by \ref toy::OStringStreamBackend).
  - **Lifetime**: The \a BackendType reference must remain valid for the lifetime of the context.
  - **Bounds**: Overflow behaviour follows \a BackendType capacity semantics.

  \section compatibility Compatibility

  - **C++ standard**: C++20 or later (concepts, \c constexpr as implemented).
  - **Embedded**: Suitable when \a BackendType uses fixed storage (e.g. \ref toy::FixedString).
*/
template <OStringStreamBackend BackendType>
class FormatContext {
public:
  /*!
    \brief Constructs a FormatContext that writes into \a output.

    \param output Reference to the backend storage to write into.

    \pre \a output must remain valid for the lifetime of this context.

    \post write() and put() append to \a output.
  */
  explicit constexpr FormatContext(BackendType & output) noexcept;

  /*!
    \brief Appends \a count bytes from \a data to the output backend.

    \param data  Pointer to the character data to append.
    \param count Number of bytes to append.

    \pre \a data must not be null when \a count \c > \c 0.

    \post \a count bytes starting at \a data are appended to the backend.

    \sa put(char)
  */
  constexpr void write(const char * data, size_t count) noexcept;

  /*!
    \brief Appends a single character to the output backend.

    \param character The character to append.

    \post \a character is appended to the end of the backend storage.

    \sa write(const char *, size_t)
  */
  constexpr void put(char character) noexcept;

  /*!
    \brief Returns a mutable reference to the underlying backend storage.

    \return Mutable reference to the \a BackendType instance passed at construction.

    \sa backend() const
  */
  [[nodiscard]] constexpr BackendType & backend() noexcept;

  /*!
    \brief Returns a const reference to the underlying backend storage.

    \return Const reference to the \a BackendType instance passed at construction.

    \sa backend()
  */
  [[nodiscard]] constexpr const BackendType & backend() const noexcept;

private:
  /// Non-owning reference to the output storage; set at construction, never reseated.
  BackendType & _output;
};

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_CONTEXT_HPP_
