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

    \post The vector is empty and ready for use.
  */
  constexpr FixedVector() noexcept;

  /*!
    \brief Destructor for the vector.

    This destructor cleans up the vector.

    \note Since the FixedVector manages its own elements, proper cleanup is performed.
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
  explicit FixedVector(std::size_t count);

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
  FixedVector(std::size_t count, const type & value);

  /*!
    \brief Constructs a copy of \a other FixedVector.

    This constructor initializes a FixedVector by copying the content from another FixedVector of the same type and
    capacity.

    \param other The source FixedVector to copy content from.

    \pre The source \a other must be valid and properly initialized.

    \post The new vector has the same size and content as the source \a other.
  */
  FixedVector(const FixedVector<type, allocatedSize> & other) noexcept;

  /*!
    \brief Constructs a FixedVector by copying from another FixedVector with different capacity.

    This constructor initializes a FixedVector by copying the content from another FixedVector of the same type
    but potentially different capacity.

    \tparam allocatedSize2 The capacity of the source FixedVector. Must be greater than zero.

    \param other The source FixedVector to copy content from.

    \pre The source \a other must be valid and properly initialized.
    \pre The source \a other size must not exceed the allocated capacity of this vector.

    \post The new vector has the same size and content as the source \a other.
  */
  template <std::size_t allocatedSize2>
  FixedVector(const FixedVector<type, allocatedSize2> & other) noexcept;

  /*!
    \brief Constructs a FixedVector by moving from another FixedVector.

    This constructor initializes a FixedVector by moving the content from another FixedVector of the same type and
    capacity. The source vector is left in a valid but unspecified state.

    \param other The source FixedVector to move content from.

    \pre The source \a other must be valid and properly initialized.

    \post The new vector has the same size and content as the source \a other.
    \post The source \a other is left in a valid but unspecified state.
  */
  FixedVector(FixedVector<type, allocatedSize> && other) noexcept;

  /*!
    \brief Constructs a FixedVector by moving from another FixedVector with different capacity.

    This constructor initializes a FixedVector by moving the content from another FixedVector of the same type
    but potentially different capacity. The source vector is left in a valid but unspecified state.

    \tparam allocatedSize2 The capacity of the source FixedVector. Must be greater than zero.

    \param other The source FixedVector to move content from.

    \pre The source \a other must be valid and properly initialized.
    \pre The source \a other size must not exceed the allocated capacity of this vector.

    \post The new vector has the same size and content as the source \a other.
    \post The source \a other is left in a valid but unspecified state.
  */
  template <std::size_t allocatedSize2>
  FixedVector(FixedVector<type, allocatedSize2> && other) noexcept;

  /*!
    \brief Constructs a FixedVector from an initializer list.

    This constructor initializes a FixedVector with the elements from the provided initializer list.

    \param init The initializer list containing the elements to initialize the vector with.

    \pre The \a init size must not exceed the allocated capacity.
    \pre All elements in \a init must be valid instances of type \a type.

    \post The new vector has the same size and content as the source \a init.
  */
  FixedVector(std::initializer_list<type> init) noexcept;

  /*!
    \brief Copy assigns other FixedVector to this FixedVector.

    This operator assigns the contents of another FixedVector with the same capacity to this vector.

    \param other The FixedVector to copy content from.

    \return A reference to this FixedVector after assignment.

    \pre The \a other vector must be valid and properly constructed.

    \post This vector has the same size and content as the source \a other vector.
    \post All elements are properly copied using copy construction.
    \post All previous elements are properly destroyed.

    \note Self-assignment is handled correctly and safely.
  */
  FixedVector<type, allocatedSize> & operator=(const FixedVector<type, allocatedSize> & other) noexcept;

  /*!
    \brief Copy assigns FixedVector with different capacity to this FixedVector.

    This operator assigns the contents of another FixedVector with different capacity to this vector.

    \tparam allocatedSize2 The capacity of the source FixedVector.

    \param other The FixedVector to copy content from.

    \return A reference to this FixedVector after assignment.

    \pre The \a other vector must be valid and properly constructed.
    \pre The \a other vector size must not exceed this vector's capacity.

    \post This vector has the same size and content as the source \a other vector.
    \post All elements are properly copied using copy construction.
    \post All previous elements are properly destroyed.
  */
  template <std::size_t allocatedSize2>
  FixedVector<type, allocatedSize> & operator=(const FixedVector<type, allocatedSize2> & other) noexcept;

  /*!
    \brief Move assigns other FixedVector to this FixedVector.

    This operator assigns the contents of another FixedVector with the same capacity to this vector using move
    semantics.

    \param other The FixedVector to move content from.

    \return A reference to this FixedVector after assignment.

    \pre The \a other vector must be valid and properly constructed.

    \post This vector has the same size and content as the source \a other vector.
    \post All elements are moved using move construction.
    \post All previous elements are properly destroyed.
    \post The source vector is left in a valid but unspecified state.

    \note Self-assignment is handled correctly and safely.
  */
  FixedVector<type, allocatedSize> & operator=(FixedVector<type, allocatedSize> && other) noexcept;

  /*!
    \brief Move assigns FixedVector with different capacity to this FixedVector.

    This operator assigns the contents of another FixedVector with different capacity to this vector using move
    semantics.

    \tparam allocatedSize2 The capacity of the source FixedVector.

    \param other The FixedVector to move content from.

    \return A reference to this FixedVector after assignment.

    \pre The \a other vector must be valid and properly constructed.
    \pre The \a other vector size must not exceed this vector's capacity.

    \post This vector has the same size and content as the source \a other vector.
    \post All elements are moved using move construction.
    \post All previous elements are properly destroyed.
    \post The source vector is left in a valid but unspecified state.
  */
  template <std::size_t allocatedSize2>
  FixedVector<type, allocatedSize> & operator=(FixedVector<type, allocatedSize2> && other) noexcept;

  /*!
    \brief Assigns initializer list to this FixedVector.

    This operator assigns the contents of an initializer list to this vector.

    \param init The initializer list containing the elements to assign.

    \return A reference to this FixedVector after assignment.

    \pre The \a init size must not exceed the allocated capacity.
    \pre All elements in \a init must be valid instances of type \a type.

    \post This vector has the same size and content as the source \a init.
    \post All elements are properly copied using copy construction.
    \post All previous elements are properly destroyed.

    \note The operator handles empty initializer lists correctly.
  */
  FixedVector<type, allocatedSize> & operator=(std::initializer_list<type> init) noexcept;

  // temporary

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
