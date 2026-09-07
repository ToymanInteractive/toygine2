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
  \file   fixed_string_storage.hpp
  \brief  Inline character buffer of a compile-time size that tracks the length of the string held in it.

  Defines \ref toy::FixedStringStorage, which holds a byte array sized at compile time, the length of the string written
  into it, and a terminator the class keeps behind that length. Used where a string needs bytes to write into and no
  heap to take them from.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FIXED_STRING_STORAGE_HPP_
#define INCLUDE_CORE_FIXED_STRING_STORAGE_HPP_

namespace toy {

/*!
  \class FixedStringStorage
  \brief Character buffer of a compile-time size that keeps the string in it null-terminated.

  Holds the characters inside the object and allocates nothing. One of the \a AllocatedSize bytes belongs to the
  terminator, so capacity() reports one less. The class writes no characters of its own: a caller fills the buffer
  through data() and then records the new length through setSize(), which puts the terminator behind it.

  \tparam AllocatedSize Size of the buffer in bytes, the terminator included. Must be greater than \c 0, checked by a
                        static assertion.

  \section fixed_string_storage_features Key Features

  * **No allocation**: the buffer lives inside the object, so the type works on targets without a heap.
  * **Compile-time capacity**: \a AllocatedSize fixes the buffer once, and capacity() reports it minus the terminator.
  * **Explicit length**: content and length move separately: the caller writes through data() and records the result
    with setSize().
  * **Null-terminated**: setSize() writes the terminator, so data() stays valid input for a C interface.
  * **Constexpr support**: every operation evaluates in a constant expression.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \section fixed_string_storage_usage Usage Example

  \code
  #include "core.hpp"

  toy::FixedStringStorage<32> storage;

  storage.data()[0] = 'h';
  storage.data()[1] = 'i';
  storage.setSize(2);

  const size_t length = storage.size();  // 2, and storage.data() reads "hi"
  \endcode

  \section fixed_string_storage_performance Performance Characteristics

  * **Default construction**: O(n) in \a AllocatedSize; the buffer starts filled with null characters.
  * **Access and length**: O(1) for data(), capacity(), size(), and setSize().
  * **Memory usage**: \a AllocatedSize bytes of characters and one length field, the object aligned as \c size_t.

  \section fixed_string_storage_safety Safety Guarantees

  * **Contracts**: setSize() checks its argument against capacity() with assert_message in debug builds. A shipping
    build skips the check, and a length past the buffer writes the terminator out of bounds.
  * **Bounds safety**: what a caller writes through data() goes unchecked; staying inside capacity() is the caller's
    responsibility.
  * **Type safety**: an \a AllocatedSize of \c 0 fails to compile.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \note A default-constructed buffer holds null characters throughout, so data() reads as an empty string before the
        first setSize().
  \note The bytes between size() and capacity() keep whatever was written there; a shorter length moves the terminator
        and clears nothing.
*/
template <size_t AllocatedSize>
class FixedStringStorage {
  static_assert(AllocatedSize > 0, "FixedStringStorage capacity must be greater than zero.");

public:
  /*!
    \brief Returns the pointer to the buffer for writing.

    \return Pointer to the first character of the buffer, never \c nullptr.

    \note setSize() writes the terminator behind the characters; this call does not.

    \sa setSize()
  */
  [[nodiscard]] constexpr char * data() noexcept;

  /*!
    \brief Returns the pointer to the stored characters.

    \return Pointer to the first character of the buffer, never \c nullptr.

    \note The characters end in a null one, so the pointer reaches a C interface unchanged.

    \sa data()
  */
  [[nodiscard]] constexpr const char * data() const noexcept;

  /*!
    \brief Returns the number of characters the buffer takes.

    \return \a AllocatedSize less the byte the terminator occupies.

    \note The value follows from \a AllocatedSize alone and never changes.

    \sa size()
  */
  [[nodiscard]] constexpr size_t capacity() const noexcept;

  /*!
    \brief Returns the length of the stored string.

    \return Count of characters before the terminator, \c 0 for an empty buffer.

    \note The count is in bytes, not characters; under a multi-byte encoding the two differ.

    \sa capacity()
    \sa setSize()
  */
  [[nodiscard]] constexpr size_t size() const noexcept;

  /*!
    \brief Records the length of the string in the buffer and terminates it there.

    Changes the length only: the characters already in the buffer stay as they are, and the call writes nothing but the
    terminator.

    \param newSize Count of characters the buffer now holds, the terminator excluded.

    \pre \a newSize is at most capacity(), checked by assert_message in debug builds.

    \post size() returns \a newSize, and the character at that index is \c '\\0'.

    \note The buffer never grows: a length past capacity() breaks the precondition instead of reaching for memory, which
          is where a storage that owns heap memory parts ways with this one.

    \sa size()
    \sa capacity()
  */
  constexpr void setSize(size_t newSize) noexcept;

private:
  /// Characters of the string, the terminator, and the bytes past it
  char _buffer[AllocatedSize] = {'\0'};

  /// Length of the string in bytes, the terminator excluded
  size_t _size{0};
};

} // namespace toy

#endif // INCLUDE_CORE_FIXED_STRING_STORAGE_HPP_
