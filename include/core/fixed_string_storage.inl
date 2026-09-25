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
  \file   fixed_string_storage.inl
  \brief  Inline implementations for buffer access, capacity, and the length setter of \ref toy::FixedStringStorage.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FIXED_STRING_STORAGE_INL_
#define INCLUDE_CORE_FIXED_STRING_STORAGE_INL_

namespace toy {

template <size_t AllocatedSize>
constexpr FixedStringStorage<AllocatedSize>::FixedStringStorage() noexcept {
  if consteval {
    std::ranges::fill(_buffer, '\0');
  } else {
    _buffer[0] = '\0';
  }

  static_assert(sizeof(FixedStringStorage) == _objectSize, "_objectSize must match the layout the compiler chose");
}

template <size_t AllocatedSize>
constexpr FixedStringStorage<AllocatedSize>::FixedStringStorage(const FixedStringStorage & other) noexcept
  requires(_copiesByLength)
  : _size(other._size) {
  if consteval {
    // A constant expression may hold no indeterminate byte, so the tail the copy skips is cleared first.
    std::ranges::fill(_buffer, '\0');
  }

  std::char_traits<char>::copy(_buffer, other._buffer, other._size + 1);
}

template <size_t AllocatedSize>
constexpr FixedStringStorage<AllocatedSize>::FixedStringStorage(FixedStringStorage && other) noexcept
  requires(_copiesByLength)
  : FixedStringStorage(other) {}

template <size_t AllocatedSize>
constexpr FixedStringStorage<AllocatedSize> & FixedStringStorage<AllocatedSize>::operator=(
  const FixedStringStorage & other
) noexcept
  requires(_copiesByLength) {
  if (this != &other) {
    std::char_traits<char>::copy(_buffer, other._buffer, other._size + 1);
    _size = other._size;
  }

  return *this;
}

template <size_t AllocatedSize>
constexpr FixedStringStorage<AllocatedSize> & FixedStringStorage<AllocatedSize>::operator=(
  FixedStringStorage && other
) noexcept
  requires(_copiesByLength) {
  return *this = other;
}

template <size_t AllocatedSize>
constexpr char * FixedStringStorage<AllocatedSize>::data() noexcept {
  return _buffer;
}

template <size_t AllocatedSize>
constexpr const char * FixedStringStorage<AllocatedSize>::data() const noexcept {
  return _buffer;
}

template <size_t AllocatedSize>
constexpr size_t FixedStringStorage<AllocatedSize>::capacity() const noexcept {
  return AllocatedSize - 1;
}

template <size_t AllocatedSize>
constexpr bool FixedStringStorage<AllocatedSize>::reserve(size_t newCapacity) noexcept {
  return newCapacity <= capacity();
}

template <size_t AllocatedSize>
constexpr size_t FixedStringStorage<AllocatedSize>::size() const noexcept {
  return _size;
}

template <size_t AllocatedSize>
constexpr void FixedStringStorage<AllocatedSize>::setSize(size_t newSize) noexcept {
  assert_message(newSize <= capacity(), "newSize must be less or equal than capacity");

  _size          = newSize;
  _buffer[_size] = '\0';
}

} // namespace toy

#endif // INCLUDE_CORE_FIXED_STRING_STORAGE_INL_
