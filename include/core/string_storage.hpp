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
  \file   string_storage.hpp
  \brief  Concept for the character buffer a string writes into and records a length in.

  Defines \ref toy::StringStorage, the requirement a buffer meets to back a string. Constrains a string over its
  storage, so one string works over a fixed-size buffer and over one that owns heap memory.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_STRING_STORAGE_HPP_
#define INCLUDE_CORE_STRING_STORAGE_HPP_

namespace toy {

/*!
  \concept StringStorage
  \brief Concept satisfied when \a T is default-constructible and movable, exposes a character buffer, reports its
         length and capacity, and takes a new length.

  Constrains a string over the storage it writes into, so swapping that buffer for one that allocates leaves the string
  itself unchanged. The requirement expression binds a mutable lvalue and a \c const one, so data(), size(), capacity()
  and max_size() must be callable on a \c const object.

  \section string_storage_requirements Requirements

  A type satisfies StringStorage if and only if \a T with any reference stripped models \c std::default_initializable
  and \c std::movable, and, for an lvalue \a storage of that type, an lvalue \a constStorage of type \c const \a T, and
  \c size_t values \a newCapacity and \a newSize, each expression below is well-formed and has exactly the type given:

  * \c storage.data() has type \c char \c *.
  * \c constStorage.data() has type \c const \c char \c *.
  * \c constStorage.size() has type \c size_t.
  * \c constStorage.capacity() has type \c size_t.
  * \c constStorage.max_size() has type \c size_t.
  * \c storage.reserve(\a newCapacity) has type \c bool.
  * \c storage.shrink_to_fit() has type \c void.
  * \c storage.setSize(\a newSize) has type \c void.

  The concept checks only the two standard concepts and the shape of those eight members. The guarantees below belong
  to the storage type, and a type that breaks them still compiles:

  * A default-constructed storage has a size() of \c 0 and \c '\\0' at data()[0].
  * Neither data() overload returns \c nullptr, and both point at the same bytes.
  * The character at index size() is \c '\\0', and size() never exceeds capacity().
  * setSize() takes a length no greater than capacity() over every storage, records it, writes that terminator, and
    touches no other character; a caller that needs more room asks reserve() first.
  * reserve() answers \c true only when a following setSize() of that length is within contract, and a \c false one
    leaves the storage as it was.
  * data() points at capacity() + 1 writable bytes, so a caller may write characters up to capacity() before it calls
    setSize().
  * A \c true reserve() keeps the first size() characters, moving them along when it grows the buffer.
  * reserve() of a length no greater than capacity() keeps the buffer where it is, so a pointer into the characters
    stays valid across the call.
  * max_size() never changes and is never less than capacity(), and reserve() refuses any length above it.
  * shrink_to_fit() keeps the first size() characters and leaves capacity() no less than size(); it may move the
    buffer, so a pointer into the characters must be taken again after the call.
  * A moved-from storage still meets every guarantee in this list; which characters it holds belongs to the type.

  \section string_storage_usage Usage Example

  \code
  #include "core.hpp"

  static_assert(toy::StringStorage<toy::FixedStringStorage<8>>);

  template <toy::StringStorage T>
  [[nodiscard]] bool assign(T & storage, const char * text, size_t length) noexcept {
    if (!storage.reserve(length)) {
      return false;
    }

    storage.setSize(length);
    std::strncpy(storage.data(), text, length);

    return true;
  }

  toy::FixedStringStorage<32> storage;
  const bool assigned = assign(storage, "hi", 2);

  const size_t length = assigned ? storage.size() : 0;
  \endcode

  \note Every member type is matched exactly. A length reported as \c int, or a data() returning \c void \c *, fails.
  \note A reference is stripped from \a T before any check, so \a T and \a T \c & give the same answer.
  \note Neither \c noexcept nor use in a constant expression is part of the requirement; both stay the type's own
        contract.
  \note Copying is not required; a \ref toy::String over a storage without copy operations is not copyable either.

  \warning Only reserve() and shrink_to_fit() may allocate, so a caller that must not allocate checks capacity() instead
           of calling reserve().

  \sa \ref toy::FixedStringStorage
  \sa \ref toy::StringLike
*/
template <typename T>
concept StringStorage = std::default_initializable<std::remove_reference_t<T>>
                        && std::movable<std::remove_reference_t<T>>
                        && requires(std::remove_reference_t<T> &       storage,
                                    const std::remove_reference_t<T> & constStorage, size_t newCapacity,
                                    size_t newSize) {
                             { storage.data() } -> std::same_as<char *>;
                             { constStorage.data() } -> std::same_as<const char *>;
                             { constStorage.size() } -> std::same_as<size_t>;
                             { constStorage.capacity() } -> std::same_as<size_t>;
                             { constStorage.max_size() } -> std::same_as<size_t>;
                             { storage.reserve(newCapacity) } -> std::same_as<bool>;
                             { storage.shrink_to_fit() } -> std::same_as<void>;
                             { storage.setSize(newSize) } -> std::same_as<void>;
                           };

} // namespace toy

#endif // INCLUDE_CORE_STRING_STORAGE_HPP_
