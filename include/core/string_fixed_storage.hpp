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
  \file   string_fixed_storage.hpp
  \brief  Fixed-size raw character buffer with explicit length for string-like containers.

  Defines \ref toy::StringFixedStorage.

  Included by \ref core.hpp; do not include this file directly.
*/

#ifndef INCLUDE_CORE_STRING_FIXED_STORAGE_HPP_
#define INCLUDE_CORE_STRING_FIXED_STORAGE_HPP_

namespace toy {

/*!
  \class StringFixedStorage
  \ingroup String
  \brief Fixed-size character storage with a tracked length and implicit null terminator at \c data()[size()].

  Holds \c allocatedSize bytes (including space for a terminating \c '\\0' after the logical string). The usable
  character count is capacity() = \c allocatedSize - 1. size() is the number of characters before that terminator;
  setSize() updates the length and writes \c '\\0' at \c data()[size()]. Callers fill \c data()[0] … \c data()[size()-1]
  when \c size() \c > \c 0.

  \tparam allocatedSize Total byte count of the internal buffer. Must be \c > \c 0 (compile-time check).

  \section features Key Features

  - **No heap**: Storage is inline; suitable for embedded and deterministic code paths.
  - **Explicit length**: size() and setSize() separate from buffer contents (caller maintains valid UTF-8 or other
    encoding as needed).
  - **Null-terminated**: After setSize(\a n), \c data()[\a n] is \c '\\0' when \a n \c < \c allocatedSize.

  \section usage Usage Example

  \code
  #include "core.hpp"

  toy::StringFixedStorage<32> storage;
  storage.data()[0] = 'h';
  storage.data()[1] = 'i';
  storage.setSize(2);
  // storage.data() is "hi" (null-terminated)
  \endcode

  \section performance Performance Characteristics

  capacity(), size(), and data() are O(1). setSize() is O(1). No allocations.

  \section safety Safety Guarantees

  - **Preconditions**: setSize(\a newSize) requires \a newSize \c < \c allocatedSize (asserted in debug) so the
    written terminator stays within the buffer.
  - **Exception safety**: All operations are noexcept.

  \note This type does not validate encoding or copy characters; higher-level types (e.g. \ref toy::FixedString) build
        on similar storage patterns.
*/
template <size_t allocatedSize>
class StringFixedStorage {
  static_assert(allocatedSize > 0, "StringFixedStorage capacity must be greater than zero.");

public:
  /*!
    \brief Returns the maximum number of characters that fit before the mandatory null terminator.

    Evaluated entirely at compile time from \a allocatedSize. The buffer holds \c allocatedSize bytes; one byte is
    reserved for \c '\\0' at index capacity(), so the usable span for character data is \c [0, capacity()).

    \return \c allocatedSize - 1.

    \note \c static \c constexpr; same for all instances of \ref toy::StringFixedStorage with this \a allocatedSize.
  */
  [[nodiscard]] static constexpr size_t capacity() noexcept;

  /*!
    \brief Returns the current logical string length in bytes (not including the terminator).

    The value is the \a newSize last passed to setSize(), or \c 0 after default construction. It does not inspect buffer
    contents.

    \return Number of characters in \c [0, size()) before \c data()[size()] (the written \c '\\0' after setSize).

    \note Runtime state; constexpr where the object is usable in constant evaluation.
  */
  [[nodiscard]] constexpr size_t size() const noexcept;

  /*!
    \brief Mutable pointer to the inline character buffer.

    Use this to write bytes \c [0, capacity()) before calling setSize(). Until setSize() runs, the buffer may be
    uninitialized except for default zero-initialization. After setSize(\a n), bytes \c [0, n) form the string and
    \c data()[n] is \c '\\0'.

    \return Pointer to the first element of the storage array.

    \pre If you write past index \c capacity()-1 or omit the terminator before setSize, behavior is undefined.

    \note Same address as the const overload on the same object.
  */
  [[nodiscard]] constexpr char * data() noexcept;

  /*!
    \brief Const pointer to the same buffer as data().

    \return Pointer to the first character; after a valid setSize(\a n), safe to read \c n characters plus the
            following \c '\\0' (i.e. \c [0, n] inclusive for the terminator).

    \note Does not synchronize with concurrent writers; use on const objects or when the string is not being modified.
  */
  [[nodiscard]] constexpr const char * data() const noexcept;

  /*!
    \brief Updates the stored length and writes a null terminator at \c data()[newSize].

    Sets the tracked length to \a newSize and assigns \c '\\0' to \c data()[newSize]. Does not clear or copy the range
    \c [0, newSize); callers must have written valid content there if \a newSize \c > \c 0.

    \param newSize New character count. Must be strictly less than \a allocatedSize so the terminator fits in the
    buffer.

    \pre \a newSize \c < \c allocatedSize (checked with assert_message in debug builds).

    \post size() equals \a newSize and \c data()[newSize] is \c '\\0'.

    \note \c constexpr; invalid \a newSize is a debug assertion failure, not a throwing error.
  */
  constexpr void setSize(size_t newSize) noexcept;

private:
  char _buffer[allocatedSize] = {'\0'};

  size_t _size{0};
};

} // namespace toy

#endif // INCLUDE_CORE_STRING_FIXED_STORAGE_HPP_
