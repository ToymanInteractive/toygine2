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
  : _data()
  , _size(0) {}

template <typename type, std::size_t allocatedSize>
constexpr FixedVector<type, allocatedSize>::~FixedVector() noexcept {
  clear();
}

template <typename type, std::size_t allocatedSize>
inline FixedVector<type, allocatedSize>::FixedVector(std::size_t count) noexcept
  : _data()
  , _size(count) {
  assert_message(count <= allocatedSize, "Count must not exceed capacity.");

  for (std::size_t index = 0; index < count; ++index) {
    std::construct_at(reinterpret_cast<type *>(_data) + index, type(type{}));
  }
}

template <typename type, std::size_t allocatedSize>
inline FixedVector<type, allocatedSize>::FixedVector(std::size_t count, const type & value) noexcept
  : _data()
  , _size(count) {
  assert_message(count <= allocatedSize, "Count must not exceed capacity.");

  for (std::size_t index = 0; index < count; ++index) {
    std::construct_at(reinterpret_cast<type *>(_data) + index, type(value));
  }
}

template <typename type, std::size_t allocatedSize>
constexpr FixedVector<type, allocatedSize>::FixedVector(const FixedVector<type, allocatedSize> & vector) noexcept
  : _data()
  , _size(0) {
  for (std::size_t index = 0; index < vector._size; ++index)
    push_back(vector[index]);
}

template <typename type, std::size_t allocatedSize>
template <std::size_t allocatedSize2>
constexpr FixedVector<type, allocatedSize>::FixedVector(const FixedVector<type, allocatedSize2> & vector) noexcept
  : _data()
  , _size(0) {
  static_assert(allocatedSize2 > 0, "FixedVector capacity must be greater than zero.");
  assert_message(vector.size() <= allocatedSize, "Source vector size must not exceed capacity.");

  for (std::size_t index = 0; index < vector.size(); ++index)
    push_back(vector[index]);
}

// temporary

template <typename type, std::size_t allocatedSize>
constexpr std::size_t FixedVector<type, allocatedSize>::size() const noexcept {
  return _size;
}

template <typename type, std::size_t allocatedSize>
constexpr std::size_t FixedVector<type, allocatedSize>::capacity() const noexcept {
  return allocatedSize;
}

template <typename type, std::size_t allocatedSize>
constexpr void FixedVector<type, allocatedSize>::clear() noexcept {
  for (std::size_t index = 0; index < _size; ++index)
    (reinterpret_cast<type *>(_data))[index].~type();

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

template <typename type, std::size_t allocatedSize>
constexpr type * FixedVector<type, allocatedSize>::end() noexcept {
  return reinterpret_cast<type *>(_data) + _size;
}

template <typename type, std::size_t allocatedSize>
constexpr const type & FixedVector<type, allocatedSize>::operator[](std::size_t index) const noexcept {
  assert(index < _size);

  return (reinterpret_cast<const type *>(_data))[index];
}

} // namespace toy

#endif // INCLUDE_CORE_FIXED_VECTOR_INL_
