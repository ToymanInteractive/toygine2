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
  using value_type = type; //!< Type of elements stored in the vector.

  using size_type = std::size_t; //!< Type used for vector size and capacity.
  using difference_type = std::ptrdiff_t; //!< Type used for pointer differences.

  using reference = value_type &; //!< Reference to vector element.
  using const_reference = const value_type &; //!< Const reference to vector element.

  using pointer = type *; //!< Pointer to vector element.
  using const_pointer = const type *; //!< Const pointer to vector element.

  //! Iterator type for vector elements.
  using iterator = value_type *;
  //! Const iterator type for vector elements.
  using const_iterator = const value_type *;
  //! Reverse iterator type for vector elements.
  using reverse_iterator = std::reverse_iterator<iterator>;
  //! Const reverse iterator type for vector elements.
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

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

    This constructor creates a FixedVector containing \a count elements, each initialized with the default value of
    type.

    \param count The number of elements to create. Must not exceed the allocated capacity.

    \pre The \a count must not exceed the allocated capacity.

    \post The vector contains \a count elements, each initialized with the default value of type.
    \post The vector size is equal to \a count.
  */
  explicit FixedVector(size_type count);

  /*!
    \brief Constructs a FixedVector with \a count elements initialized to \a value.

    This constructor creates a FixedVector containing \a count elements, each initialized with the specified \a value.

    \param count The number of elements to create. Must not exceed the allocated capacity.
    \param value The value to initialize all elements with.

    \pre The \a count must not exceed the allocated capacity.
    \pre The \a value must be a valid instance of type.

    \post The vector contains \a count elements, each initialized with \a value.
    \post The vector size is equal to \a count.
  */
  FixedVector(size_type count, const type & value);

  /*!
    \brief Constructs a FixedVector from a range defined by iterators.

    This constructor initializes a FixedVector with elements from the range [ \a first, \a last ).

    \tparam InputIterator The type of the input iterator.

    \param first Iterator pointing to the first element to copy.
    \param last  Iterator pointing to one past the last element to copy.

    \pre The \a first and \a last iterators must be valid.
    \pre The range [ \a first, \a last ) must not exceed the allocated capacity.
    \pre All elements in the range must be valid instances of type.

    \post The new vector contains all elements from the range [ \a first, \a last ).
  */
  template <typename InputIterator>
    requires(!std::is_integral_v<InputIterator>)
  FixedVector(InputIterator first, InputIterator last);

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
    \pre All elements in \a init must be valid instances of type.

    \post The new vector has the same size and content as the source \a init.
  */
  explicit FixedVector(std::initializer_list<type> init);

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

    \param ilist The initializer list containing the elements to assign.

    \return A reference to this FixedVector after assignment.

    \pre The \a ilist size must not exceed the allocated capacity.
    \pre All elements in \a ilist must be valid instances of type.

    \post This vector has the same size and content as the source \a ilist.
    \post All elements are properly copied using copy construction.
    \post All previous elements are properly destroyed.

    \note The operator handles empty initializer lists correctly.
  */
  FixedVector<type, allocatedSize> & operator=(std::initializer_list<type> ilist);

  /*!
    \brief Assigns \a count copies of \a value to the vector.

    This method replaces the current contents of the vector with \a count copies of \a value.

    \param count The number of copies of \a value to assign.
    \param value The value to assign \a count copies of.

    \pre The \a count must not exceed the allocated capacity.

    \post The vector contains \a count elements, each initialized with \a value.
    \post The vector size is equal to \a count.
    \post All previous elements are properly destroyed.

    \note This method clears existing elements before assignment.
    \note The \a value is copied for each element.
  */
  void assign(size_type count, const type & value);

  /*!
    \brief Assigns elements from the range [ \a first, \a last ) to the vector.

    This method replaces the current contents of the vector with elements from the range [ \a first, \a last ).

    \tparam InputIterator The type of the input iterator.

    \param first Iterator pointing to the first element to assign.
    \param last  Iterator pointing to one past the last element to assign.

    \pre The \a first and \a last iterators must be valid.
    \pre The range [ \a first, \a last ) must not exceed the allocated capacity.

    \post The vector contains all elements from the range [ \a first, \a last ).
    \post The vector size is equal to the size of the range.
    \post All previous elements are properly destroyed.

    \note This method clears existing elements before assignment.
    \note Elements from the range are copied into the vector.
  */
  template <typename InputIterator>
    requires(!std::is_integral_v<InputIterator>)
  void assign(InputIterator first, InputIterator last);

  /*!
    \brief Assigns elements from an initializer list to the vector.

    This method replaces the current contents of the vector with elements from the initializer list.

    \param ilist The initializer list containing the elements to assign.

    \pre The \a ilist size must not exceed the allocated capacity.

    \post The vector contains all elements from the initializer list.
    \post The vector size is equal to the size of the initializer list.
    \post All previous elements are properly destroyed.

    \note This method clears existing elements before assignment.
    \note The operator handles empty initializer lists correctly.
  */
  void assign(std::initializer_list<type> ilist);

  /*!
    \brief Access an element at the specified position.

    This method provides direct access to an element at the specified \a pos.

    \param pos The position of the element to access in the vector.

    \return A reference to the element at the specified \a pos.

    \pre The \a pos must be less than the current vector size.

    \note The returned reference allows modification of the element.
    \note Use const version for read-only access.
    \note This method is equivalent to the subscript operator.

    \see operator[](size_type pos)
  */
  [[nodiscard]] constexpr reference at(size_type pos) noexcept;

  /*!
    \brief Access an element at the specified position.

    This method provides read-only access to an element at the specified \a pos.

    \param pos The position of the element to access in the vector.

    \return A const reference to the element at the specified \a pos.

    \pre The \a pos must be less than the current vector size.

    \note The returned reference is read-only and cannot modify the element.
    \note Use the non-const overload to allow modification.
    \note This method is equivalent to the subscript operator.

    \see operator[](size_type pos) const
  */
  [[nodiscard]] constexpr const_reference at(size_type pos) const noexcept;

  /*!
    \brief Access an element at the specified position.

    This operator provides direct access to an element at the specified \a pos.

    \param pos The position of the element to access in the vector.

    \return A reference to the element at the specified \a pos.

    \pre The \a pos should be less than the current vector size.

    \note The returned reference allows modification of the element.
    \note Use const version for read-only access.
  */
  [[nodiscard]] constexpr reference operator[](size_type pos) noexcept;

  /*!
    \brief Access an element at the specified position.

    This operator provides read-only access to an element at the specified \a pos.

    \param pos The position of the element to access in the vector.

    \return A const reference to the element at the specified \a pos.

    \pre The \a pos should be less than the current vector size.

    \note The returned reference is read-only and cannot modify the element.
    \note Use the non-const overload to allow modification.
  */
  [[nodiscard]] constexpr const_reference operator[](size_type pos) const noexcept;

  /*!
    \brief Returns a reference to the first element of the vector.

    This method provides direct access to the first element of the vector.

    \return A reference to the first element of the vector.

    \pre The vector must not be empty.

    \note The returned reference allows modification of the element.
    \note Use const version for read-only access.

    \see back()
  */
  [[nodiscard]] constexpr reference front() noexcept;

  /*!
    \brief Returns a const reference to the first element of the vector.

    This method provides read-only access to the first element of the vector.

    \return A const reference to the first element of the vector.

    \pre The vector must not be empty.

    \note The returned reference is read-only and cannot modify the element.
    \note Use the non-const overload to allow modification.

    \see back() const
  */
  [[nodiscard]] constexpr const_reference front() const noexcept;

  /*!
    \brief Returns a reference to the last element of the vector.

    This method provides direct access to the last element of the vector.

    \return A reference to the last element of the vector.

    \pre The vector must not be empty.

    \note The returned reference allows modification of the element.
    \note Use const version for read-only access.

    \see front()
  */
  [[nodiscard]] constexpr reference back() noexcept;

  /*!
    \brief Returns a const reference to the last element of the vector.

    This method provides read-only access to the last element of the vector.

    \return A const reference to the last element of the vector.

    \pre The vector must not be empty.

    \note The returned reference is read-only and cannot modify the element.
    \note Use the non-const overload to allow modification.

    \see front() const
  */
  [[nodiscard]] constexpr const_reference back() const noexcept;

  /*!
    \brief Returns a pointer to the underlying data array.

    This method provides direct access to the underlying data storage of the vector.

    \return A pointer to the first element in the underlying array.

    \note The returned pointer is valid as long as the vector exists.
    \note The pointer points to a contiguous block of memory containing the vector elements.
    \note The array contains exactly \a size() elements.
    \note Modifying elements through the returned pointer directly modifies the vector contents.
    \note Use const version for read-only access.

    \warning Accessing elements beyond \a size() through the returned pointer results in undefined behavior.

    \see size()
  */
  [[nodiscard]] constexpr pointer data() noexcept;

  /*!
    \brief Returns a const pointer to the underlying data array.

    This method provides read-only access to the underlying data storage of the vector.

    \return A const pointer to the first element in the underlying array.

    \note The returned pointer is valid as long as the vector exists.
    \note The pointer points to a contiguous block of memory containing the vector elements.
    \note The array contains exactly \a size() elements.
    \note The returned pointer does not allow modification of the vector contents.

    \warning Accessing elements beyond \a size() through the returned pointer results in undefined behavior.

    \see size()
  */
  [[nodiscard]] constexpr const_pointer data() const noexcept;

  /*!
    \brief Returns an iterator to the first element of the vector.

    This method returns an iterator pointing to the first element of the vector.

    \return An iterator to the first element of the vector.

    \note If the vector is empty, the returned iterator will be equal to \a end().
    \note The iterator allows modification of elements.

    \see end()
  */
  [[nodiscard]] constexpr iterator begin() noexcept;

  /*!
    \brief Returns a const iterator to the first element of the vector.

    This method returns a const iterator pointing to the first element of the vector.

    \return A const iterator to the first element of the vector.

    \note If the vector is empty, the returned iterator will be equal to \a end() const.
    \note The iterator is read-only and cannot modify elements.

    \see end() const
  */
  [[nodiscard]] constexpr const_iterator begin() const noexcept;

  /*!
    \brief Returns a const iterator to the first element of the vector.

    This method returns a const iterator pointing to the first element of the vector.

    \return A const iterator to the first element of the vector.

    \note If the vector is empty, the returned iterator will be equal to \a cend().
    \note The iterator is read-only and cannot modify elements.
    \note This method is provided for consistency with standard containers.

    \see cend()
  */
  [[nodiscard]] constexpr const_iterator cbegin() const noexcept;

  /*!
    \brief Returns an iterator to the element following the last element of the vector.

    This method returns an iterator pointing to the element following the last element of the vector. This iterator acts
    as a placeholder and cannot be dereferenced.

    \return An iterator to the element following the last element of the vector.

    \note The returned iterator should not be dereferenced.
    \note It is used to indicate the end of a range [begin(), end()).
    \note For an empty vector, \a begin() == \a end().

    \see begin()
  */
  [[nodiscard]] constexpr iterator end() noexcept;

  /*!
    \brief Returns a const iterator to the element following the last element of the vector.

    This method returns a const iterator pointing to the element following the last element of the vector. This iterator
    acts as a placeholder and cannot be dereferenced.

    \return A const iterator to the element following the last element of the vector.

    \note The returned iterator should not be dereferenced.
    \note It is used to indicate the end of a range [begin(), end()).
    \note For an empty vector, \a begin() const == \a end() const.

    \see begin() const
  */
  [[nodiscard]] constexpr const_iterator end() const noexcept;

  /*!
    \brief Returns a const iterator to the element following the last element of the vector.

    This method returns a const iterator pointing to the element following the last element of the vector. This iterator
    acts as a placeholder and cannot be dereferenced.

    \return A const iterator to the element following the last element of the vector.

    \note The returned iterator should not be dereferenced.
    \note It is used to indicate the end of a range [cbegin(), cend()).
    \note For an empty vector, \a cbegin() == \a cend().
    \note This method is provided for consistency with standard containers.

    \see cbegin()
  */
  [[nodiscard]] constexpr const_iterator cend() const noexcept;

  /*!
    \brief Returns a reverse iterator to the last element of the vector.

    This method returns a reverse iterator pointing to the last element of the vector.

    \return A reverse iterator to the last element of the vector.

    \note If the vector is empty, the returned iterator will be equal to \a rend().
    \note The iterator allows modification of elements.
    \note Iteration through the vector in reverse order can be done using [rbegin(), rend()).

    \see rend()
  */
  [[nodiscard]] constexpr reverse_iterator rbegin() noexcept;

  /*!
    \brief Returns a const reverse iterator to the last element of the vector.

    This method returns a const reverse iterator pointing to the last element of the vector.

    \return A const reverse iterator to the last element of the vector.

    \note If the vector is empty, the returned iterator will be equal to \a rend() const.
    \note The iterator is read-only and cannot modify elements.
    \note Iteration through the vector in reverse order can be done using [rbegin(), rend()) const.

    \see rend() const
  */
  [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept;

  /*!
    \brief Returns a const reverse iterator to the last element of the vector.

    This method returns a const reverse iterator pointing to the last element of the vector.

    \return A const reverse iterator to the last element of the vector.

    \note If the vector is empty, the returned iterator will be equal to \a crend().
    \note The iterator is read-only and cannot modify elements.
    \note This method is provided for consistency with standard containers.
    \note Iteration through the vector in reverse order can be done using [crbegin(), crend()).

    \see crend()
  */
  [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept;

  /*!
    \brief Returns a reverse iterator to the element preceding the first element of the vector.

    This method returns a reverse iterator pointing to the element preceding the first element of the vector. This
    iterator acts as a placeholder and cannot be dereferenced.

    \return A reverse iterator to the element preceding the first element of the vector.

    \note The returned iterator should not be dereferenced.
    \note It is used to indicate the end of a reverse range [rbegin(), rend()).
    \note For an empty vector, \a rbegin() == \a rend().

    \see rbegin()
  */
  [[nodiscard]] constexpr reverse_iterator rend() noexcept;

  /*!
    \brief Returns a const reverse iterator to the element preceding the first element of the vector.

    This method returns a const reverse iterator pointing to the element preceding the first element of the vector. This
    iterator acts as a placeholder and cannot be dereferenced.

    \return A const reverse iterator to the element preceding the first element of the vector.

    \note The returned iterator should not be dereferenced.
    \note It is used to indicate the end of a reverse range [rbegin(), rend()) const.
    \note For an empty vector, \a rbegin() const == \a rend() const.

    \see rbegin() const
  */
  [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept;

  /*!
    \brief Returns a const reverse iterator to the element preceding the first element of the vector.

    This method returns a const reverse iterator pointing to the element preceding the first element of the vector. This
    iterator acts as a placeholder and cannot be dereferenced.

    \return A const reverse iterator to the element preceding the first element of the vector.

    \note The returned iterator should not be dereferenced.
    \note It is used to indicate the end of a reverse range [crbegin(), crend()).
    \note For an empty vector, \a crbegin() == \a crend().
    \note This method is provided for consistency with standard containers.

    \see crbegin()
  */
  [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept;

  /*!
    \brief Checks if the vector is empty.

    This method returns whether the vector contains any elements.

    \return \c true if the vector is empty (size is 0), \c false otherwise.

    \note This method provides a convenient way to check if the vector has no elements.
    \note Equivalent to \a size() == 0.

    \see size()
  */
  [[nodiscard]] constexpr bool empty() const noexcept;

  /*!
    \brief Returns the number of elements currently stored in the vector.

    This method returns the current number of elements in the vector.

    \return The number of elements in the vector.

    \note The size is always less than or equal to the capacity.
    \note For an empty vector, this method returns 0.

    \see capacity()
    \see empty()
  */
  [[nodiscard]] constexpr size_type size() const noexcept;

  /*!
    \brief Returns the maximum number of elements the vector can hold.

    This method returns the theoretical maximum number of elements that the vector can hold. For FixedVector, this is
    equal to the allocated capacity.

    \return The maximum number of elements the vector can hold.

    \note For FixedVector, \a max_size() is always equal to \a capacity().
    \note This value is fixed at compile time and determined by the template parameter \a allocatedSize.

    \see capacity()
  */
  [[nodiscard]] constexpr size_type max_size() const noexcept;

  /*!
    \brief Returns the maximum number of elements that can be stored in the vector.

    This method returns the maximum number of elements that the vector can store without reallocation. This value is
    fixed at compile time and determined by the template parameter \a allocatedSize.

    \return The maximum number of elements the vector can store.

    \note The capacity is always greater than or equal to the current size.
    \note The capacity cannot change during the lifetime of the vector.
    \note Attempting to add more elements than the capacity will result in undefined behavior.

    \see size()
  */
  [[nodiscard]] constexpr size_type capacity() const noexcept;

  // temporary

  constexpr void push_back(const type & val) noexcept;

  constexpr void clear() noexcept;

private:
  static_assert(allocatedSize > 0, "FixedVector capacity must be greater than zero.");

  /// Internal buffer for storing elements.
  std::byte _data[allocatedSize * sizeof(type)];

  /// Current number of elements in the vector.
  size_type _size = 0;
};

} // namespace toy

#endif // INCLUDE_CORE_FIXED_VECTOR_HPP_
