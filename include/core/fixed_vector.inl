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
  \file   fixed_vector.inl
  \brief  TODO
*/

#ifndef INCLUDE_CORE_FIXED_VECTOR_INL_
#define INCLUDE_CORE_FIXED_VECTOR_INL_

namespace toy {

template <typename type, std::size_t allocatedSize>
constexpr FixedVector<type, allocatedSize>::FixedVector() noexcept
  : _data() {}

template <typename type, std::size_t allocatedSize>
constexpr FixedVector<type, allocatedSize>::~FixedVector() noexcept {
  clear();
}

template <typename type, std::size_t allocatedSize>
inline FixedVector<type, allocatedSize>::FixedVector(size_type count)
  : _data()
  , _size(count) {
  assert_message(count <= allocatedSize, "Count must not exceed capacity.");

  for (size_type index = 0; index < _size; ++index)
    std::construct_at(data() + index, type{});
}

template <typename type, std::size_t allocatedSize>
inline FixedVector<type, allocatedSize>::FixedVector(size_type count, const type & value)
  : _data()
  , _size(count) {
  assert_message(count <= allocatedSize, "Count must not exceed capacity.");

  for (size_type index = 0; index < _size; ++index)
    std::construct_at(data() + index, value);
}

template <typename type, std::size_t allocatedSize>
template <typename InputIterator>
  requires(!std::is_integral_v<InputIterator>)
inline FixedVector<type, allocatedSize>::FixedVector(InputIterator first, InputIterator last)
  : _data() {
  for (auto it = first; it != last; ++it) {
    assert_message(_size < allocatedSize, "Iterator range size must not exceed capacity.");
    std::construct_at(data() + _size++, *it);
  }
}

template <typename type, std::size_t allocatedSize>
inline FixedVector<type, allocatedSize>::FixedVector(const FixedVector<type, allocatedSize> & other) noexcept
  : _data()
  , _size(other._size) {
  for (size_type index = 0; index < _size; ++index)
    std::construct_at(data() + index, other[index]);
}

template <typename type, std::size_t allocatedSize>
template <std::size_t allocatedSize2>
inline FixedVector<type, allocatedSize>::FixedVector(const FixedVector<type, allocatedSize2> & other) noexcept
  : _data()
  , _size(other.size()) {
  assert_message(other.size() <= allocatedSize, "Source vector size must not exceed capacity.");

  for (size_type index = 0; index < _size; ++index)
    std::construct_at(data() + index, other[index]);
}

template <typename type, std::size_t allocatedSize>
inline FixedVector<type, allocatedSize>::FixedVector(FixedVector<type, allocatedSize> && other) noexcept
  : _data()
  , _size(other._size) {
  for (size_type index = 0; index < _size; ++index)
    std::construct_at(data() + index, std::move(other[index]));

  other.clear();
}

template <typename type, std::size_t allocatedSize>
template <std::size_t allocatedSize2>
inline FixedVector<type, allocatedSize>::FixedVector(FixedVector<type, allocatedSize2> && other) noexcept
  : _data()
  , _size(other.size()) {
  assert_message(other.size() <= allocatedSize, "Source vector size must not exceed capacity.");

  for (size_type index = 0; index < _size; ++index)
    std::construct_at(data() + index, std::move(other[index]));

  other.clear();
}

template <typename type, std::size_t allocatedSize>
inline FixedVector<type, allocatedSize>::FixedVector(std::initializer_list<type> init)
  : _data()
  , _size(init.size()) {
  assert_message(init.size() <= allocatedSize, "Initializer list size must not exceed capacity.");

  size_type index = 0;
  for (const auto & element : init)
    std::construct_at(data() + index++, element);
}

template <typename type, std::size_t allocatedSize>
inline FixedVector<type, allocatedSize> & FixedVector<type, allocatedSize>::operator=(
  const FixedVector<type, allocatedSize> & other) noexcept {
  if (this == &other)
    return *this;

  clear();

  _size = other._size;

  for (size_type index = 0; index < _size; ++index)
    std::construct_at(data() + index, other[index]);

  return *this;
}

template <typename type, std::size_t allocatedSize>
template <std::size_t allocatedSize2>
inline FixedVector<type, allocatedSize> & FixedVector<type, allocatedSize>::operator=(
  const FixedVector<type, allocatedSize2> & other) noexcept {
  assert_message(other.size() <= allocatedSize, "Source vector size must not exceed capacity.");

  clear();

  _size = other.size();

  for (size_type index = 0; index < _size; ++index)
    std::construct_at(data() + index, other[index]);

  return *this;
}

template <typename type, std::size_t allocatedSize>
inline FixedVector<type, allocatedSize> & FixedVector<type, allocatedSize>::operator=(
  FixedVector<type, allocatedSize> && other) noexcept {
  if (this == &other)
    return *this;

  clear();

  _size = other._size;

  for (size_type index = 0; index < _size; ++index)
    std::construct_at(data() + index, std::move(other[index]));

  other.clear();

  return *this;
}

template <typename type, std::size_t allocatedSize>
template <std::size_t allocatedSize2>
inline FixedVector<type, allocatedSize> & FixedVector<type, allocatedSize>::operator=(
  FixedVector<type, allocatedSize2> && other) noexcept {
  assert_message(other.size() <= allocatedSize, "Source vector size must not exceed capacity.");

  clear();

  _size = other.size();

  for (size_type index = 0; index < _size; ++index)
    std::construct_at(data() + index, std::move(other[index]));

  other.clear();

  return *this;
}

template <typename type, std::size_t allocatedSize>
inline FixedVector<type, allocatedSize> & FixedVector<type, allocatedSize>::operator=(
  std::initializer_list<type> ilist) {
  assert_message(ilist.size() <= allocatedSize, "Initializer list size must not exceed capacity.");

  clear();

  _size = ilist.size();

  size_type index = 0;
  for (const auto & element : ilist)
    std::construct_at(data() + index++, element);

  return *this;
}

template <typename type, std::size_t allocatedSize>
inline void FixedVector<type, allocatedSize>::assign(size_type count, const type & value) {
  assert_message(count <= allocatedSize, "Count must not exceed capacity.");

  clear();

  _size = count;

  for (size_type index = 0; index < _size; ++index)
    std::construct_at(data() + index, value);
}

template <typename type, std::size_t allocatedSize>
template <typename InputIterator>
  requires(!std::is_integral_v<InputIterator>)
inline void FixedVector<type, allocatedSize>::assign(InputIterator first, InputIterator last) {
  clear();

  for (auto it = first; it != last; ++it) {
    assert_message(_size < allocatedSize, "Iterator range size must not exceed capacity.");
    std::construct_at(data() + _size++, *it);
  }
}

template <typename type, std::size_t allocatedSize>
inline void FixedVector<type, allocatedSize>::assign(std::initializer_list<type> ilist) {
  assert_message(ilist.size() <= allocatedSize, "Initializer list size must not exceed capacity.");

  clear();

  _size = ilist.size();

  size_type index = 0;
  for (const auto & element : ilist)
    std::construct_at(data() + index++, element);
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::reference FixedVector<type, allocatedSize>::at(
  size_type pos) noexcept {
  assert_message(pos < _size, "Position must be within bounds");

  return data()[pos];
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::const_reference FixedVector<type, allocatedSize>::at(
  size_type pos) const noexcept {
  assert_message(pos < _size, "Position must be within bounds");

  return data()[pos];
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::reference FixedVector<
  type, allocatedSize>::operator[](size_type pos) noexcept {
  assert_message(pos < _size, "Position must be within bounds");

  return data()[pos];
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::const_reference FixedVector<
  type, allocatedSize>::operator[](size_type pos) const noexcept {
  assert_message(pos < _size, "Position must be within bounds");

  return data()[pos];
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::reference FixedVector<
  type, allocatedSize>::front() noexcept {
  assert_message(_size > 0, "Vector must not be empty");

  return *data();
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::const_reference FixedVector<
  type, allocatedSize>::front() const noexcept {
  assert_message(_size > 0, "Vector must not be empty");

  return *data();
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::reference FixedVector<
  type, allocatedSize>::back() noexcept {
  assert_message(_size > 0, "Vector must not be empty");

  return data()[_size - 1];
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::const_reference FixedVector<
  type, allocatedSize>::back() const noexcept {
  assert_message(_size > 0, "Vector must not be empty");

  return data()[_size - 1];
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::pointer FixedVector<type,
                                                                                       allocatedSize>::data() noexcept {
  return reinterpret_cast<pointer>(_data);
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::const_pointer FixedVector<
  type, allocatedSize>::data() const noexcept {
  return reinterpret_cast<const_pointer>(_data);
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::iterator FixedVector<
  type, allocatedSize>::begin() noexcept {
  return data();
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::const_iterator FixedVector<
  type, allocatedSize>::begin() const noexcept {
  return data();
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::const_iterator FixedVector<
  type, allocatedSize>::cbegin() const noexcept {
  return begin();
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::iterator FixedVector<type,
                                                                                        allocatedSize>::end() noexcept {
  return data() + _size;
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::const_iterator FixedVector<
  type, allocatedSize>::end() const noexcept {
  return data() + _size;
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::const_iterator FixedVector<
  type, allocatedSize>::cend() const noexcept {
  return end();
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::reverse_iterator FixedVector<
  type, allocatedSize>::rbegin() noexcept {
  return reverse_iterator(end());
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::const_reverse_iterator FixedVector<
  type, allocatedSize>::rbegin() const noexcept {
  return const_reverse_iterator(end());
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::const_reverse_iterator FixedVector<
  type, allocatedSize>::crbegin() const noexcept {
  return rbegin();
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::reverse_iterator FixedVector<
  type, allocatedSize>::rend() noexcept {
  return reverse_iterator(begin());
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::const_reverse_iterator FixedVector<
  type, allocatedSize>::rend() const noexcept {
  return const_reverse_iterator(begin());
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::const_reverse_iterator FixedVector<
  type, allocatedSize>::crend() const noexcept {
  return rend();
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr bool FixedVector<type, allocatedSize>::empty() const noexcept {
  return _size == 0;
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::size_type FixedVector<type,
                                                                                         allocatedSize>::size() const
  noexcept {
  return _size;
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::size_type FixedVector<
  type, allocatedSize>::max_size() const noexcept {
  return allocatedSize;
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr typename FixedVector<type, allocatedSize>::size_type FixedVector<
  type, allocatedSize>::capacity() const noexcept {
  return allocatedSize;
}

// temporary

template <typename type, std::size_t allocatedSize>
constexpr void FixedVector<type, allocatedSize>::clear() noexcept {
  for (std::size_t index = 0; index < _size; ++index)
    data()[index].~type();

  _size = 0;
}

template <typename type, std::size_t allocatedSize>
constexpr void FixedVector<type, allocatedSize>::push_back(const type & val) noexcept {
  assert(_size < allocatedSize);

  if (_size < allocatedSize) {
    std::construct_at(end(), type(val));
    ++_size;
  }
}

} // namespace toy

#endif // INCLUDE_CORE_FIXED_VECTOR_INL_
