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
  \file   string_fixed_storage.inl
  \brief  Inline implementations for \ref toy::StringFixedStorage.

  Included by \ref core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_STRING_FIXED_STORAGE_INL_
#define INCLUDE_CORE_STRING_FIXED_STORAGE_INL_

namespace toy {

template <size_t AllocatedSize>
constexpr size_t StringFixedStorage<AllocatedSize>::capacity() noexcept {
  return AllocatedSize - 1;
}

template <size_t AllocatedSize>
constexpr size_t StringFixedStorage<AllocatedSize>::size() const noexcept {
  return _size;
}

template <size_t AllocatedSize>
constexpr char * StringFixedStorage<AllocatedSize>::data() noexcept {
  return _buffer;
}

template <size_t AllocatedSize>
constexpr const char * StringFixedStorage<AllocatedSize>::data() const noexcept {
  return _buffer;
}

template <size_t AllocatedSize>
constexpr void StringFixedStorage<AllocatedSize>::setSize(size_t newSize) noexcept {
  assert_message(newSize <= capacity(), "newSize must be less or equal than capacity");
  _size = newSize;
  _buffer[_size] = '\0';
}

} // namespace toy

#endif // INCLUDE_CORE_STRING_FIXED_STORAGE_INL_
