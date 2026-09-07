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
  \brief Concept satisfied when \a T exposes a character buffer, reports its length and capacity, and takes a new
         length.

  Constrains a string over the storage it writes into, so swapping that buffer for one that allocates leaves the string
  itself unchanged. The requirement expression binds a mutable lvalue and a \c const one, so data(), size() and
  capacity() must be callable on a \c const object.

  \section string_storage_requirements Requirements

  A type satisfies StringStorage if and only if, for an lvalue \a storage of type \a T with any reference stripped, an
  lvalue \a constStorage of type \c const \a T, and a \c size_t \a newSize, each expression below is well-formed and has
  exactly the type given:

  * \c storage.data() has type \c char \c *.
  * \c constStorage.data() has type \c const \c char \c *.
  * \c constStorage.size() has type \c size_t.
  * \c constStorage.capacity() has type \c size_t.
  * \c storage.reserve(\a newSize) has type \c bool.
  * \c storage.setSize(\a newSize) has type \c void.

  The expression checks the shape of those six members and nothing else. Four further guarantees belong to the storage
  type, and a type that breaks them still compiles:

  * Neither data() overload returns \c nullptr, and both point at the same bytes.
  * The character at index size() is \c '\\0', and size() never exceeds capacity().
  * setSize() records the length and writes that terminator, and touches no other character.
  * reserve() answers \c true only when a following setSize() of that length is within contract, and a \c false one
    leaves the storage as it was.

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
    toy::strncpy(storage.data(), text, length);

    return true;
  }

  toy::FixedStringStorage<32> storage;
  const bool assigned = assign(storage, "hi", 2);

  const size_t length = assigned ? storage.size() : 0;
  \endcode

  \note Every member type is matched exactly. A length reported as \c int, or a data() returning \c void \c *, fails.
  \note A reference is stripped from \a T before the members are looked up, so \a T and \a T \c & give the same answer.
  \note Neither \c noexcept nor use in a constant expression is part of the requirement; both stay the type's own
        contract.

  \warning What setSize() does past capacity() belongs to the type rather than to the concept: a buffer of a fixed size
           treats such a call as a broken precondition, while a storage that owns heap memory grows to fit. A caller
           that asks reserve() first stays within contract over either one.
  \warning A caller that must not allocate reads capacity() instead, since reserve() is where such a storage allocates.

  \sa \ref toy::FixedStringStorage
  \sa \ref toy::StringLike
*/
template <typename T>
concept StringStorage = requires(std::remove_reference_t<T> & storage, const std::remove_reference_t<T> & constStorage,
                                 size_t newSize) {
  { storage.data() } -> std::same_as<char *>;
  { constStorage.data() } -> std::same_as<const char *>;
  { constStorage.size() } -> std::same_as<size_t>;
  { constStorage.capacity() } -> std::same_as<size_t>;
  { storage.reserve(newSize) } -> std::same_as<bool>;
  { storage.setSize(newSize) } -> std::same_as<void>;
};

} // namespace toy

#endif // INCLUDE_CORE_STRING_STORAGE_HPP_
