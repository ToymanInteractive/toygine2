//
// Copyright (c) 2025 by Toyman Interactive
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
  \file   fixed_vector.hpp
  \brief  TODO
*/

#ifndef INCLUDE_CORE_FIXED_VECTOR_HPP_
#define INCLUDE_CORE_FIXED_VECTOR_HPP_

namespace toy {

template <typename type, std::size_t allocatedSize>
class FixedVector {
public:
  /*!
    \brief Default constructor.

    Constructs an empty FixedVector with no elements. The vector is ready for use and has a size of 0.

    \post The vector is empty and ready for use.
  */
  constexpr FixedVector() noexcept;

  /*!
    \brief Destructor.

    Destroys the FixedVector and all its elements. Calls the destructor of each element in the vector.

    \note This destructor is constexpr and can be used in compile-time contexts.
  */
  constexpr ~FixedVector() noexcept;

  /*!
    \brief Constructs a FixedVector with \a count default-initialized elements.

    This constructor creates a FixedVector containing \a count elements, each initialized with the default value of type
    \a type.

    \param count The number of elements to create. Must not exceed the allocated capacity.

    \pre The \a count must not exceed the allocated capacity.

    \post The vector contains \a count elements, each initialized with the default value of type \a type.
    \post The vector size is equal to \a count.
  */
  inline explicit FixedVector(std::size_t count) noexcept;

  /*!
    \brief Constructs a FixedVector with \a count elements initialized to \a value.

    This constructor creates a FixedVector containing \a count elements, each initialized with the specified \a value.

    \param count The number of elements to create. Must not exceed the allocated capacity.
    \param value The value to initialize all elements with.

    \pre The \a count must not exceed the allocated capacity.
    \pre The \a value must be a valid instance of type \a type.

    \post The vector contains \a count elements, each initialized with \a value.
    \post The vector size is equal to \a count.
  */
  inline FixedVector(std::size_t count, const type & value) noexcept;

  /*!
    \brief Constructs a copy of \a other FixedVector.

    This constructor initializes a FixedVector by copying the content from another FixedVector of the same type and
    capacity.

    \param other The source FixedVector to copy content from.

    \pre The source \a other must be valid and properly initialized.

    \post The new vector has the same size and content as the source \a other.
  */
  inline FixedVector(const FixedVector<type, allocatedSize> & other) noexcept;

  // temporary

  template <std::size_t allocatedSize2>
  constexpr FixedVector(const FixedVector<type, allocatedSize2> & array) noexcept;

  constexpr std::size_t size() const noexcept;
  constexpr std::size_t capacity() const noexcept;

  constexpr void push_back(const type & val) noexcept;

  constexpr void clear() noexcept;

  constexpr type * end() noexcept;

  constexpr const type & operator[](size_t index) const noexcept;

private:
  static_assert(allocatedSize > 0, "FixedVector capacity must be greater than zero.");

  /// Internal buffer for storing elements.
  std::byte _data[allocatedSize * sizeof(type)];

  /// Current number of elements in the vector.
  std::size_t _size;
};

} // namespace toy

#endif // INCLUDE_CORE_FIXED_VECTOR_HPP_
