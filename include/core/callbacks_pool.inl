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
  \file   callbacks_pool.inl
  \brief  Inline implementations for \ref toy::CallbacksPool.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_CALLBACKS_POOL_INL_
#define INCLUDE_CORE_CALLBACKS_POOL_INL_

namespace toy {

template <typename T, size_t AllocatedSize>
constexpr CallbacksPool<T, AllocatedSize>::CallbacksPool() noexcept {
  _callbacks.fill(StaticCallback());
}

template <typename T, size_t AllocatedSize>
constexpr bool CallbacksPool<T, AllocatedSize>::add(void (*method)(T arg)) noexcept {
  assert_message(method != nullptr, "Cannot add null callback");

  // Check if the method already exists
  for (size_t index = 0; index < _subscribersCount; ++index) {
    if (_callbacks[index].method == method)
      return true;
  }

  assert_message(_subscribersCount < AllocatedSize, "No room for new callback, increase pool size");
  if (_subscribersCount >= AllocatedSize)
    return false;

  _callbacks[_subscribersCount++].method = method;
  return true;
}

template <typename T, size_t AllocatedSize>
constexpr bool CallbacksPool<T, AllocatedSize>::remove(void (*method)(T arg)) noexcept {
  for (size_t index = 0; index < _subscribersCount; ++index) {
    if (_callbacks[index].method == method) {
      _callbacks[index].method = _callbacks[--_subscribersCount].method;
      return true;
    }
  }

  return false;
}

template <typename T, size_t AllocatedSize>
constexpr void CallbacksPool<T, AllocatedSize>::reset() noexcept {
  _subscribersCount = 0;
}

template <typename T, size_t AllocatedSize>
[[nodiscard]] constexpr size_t CallbacksPool<T, AllocatedSize>::subscribersAmount() const noexcept {
  return _subscribersCount;
}

template <typename T, size_t AllocatedSize>
constexpr void CallbacksPool<T, AllocatedSize>::call(T arg) const noexcept {
  for (size_t index = 0; index < _subscribersCount; ++index) {
    (*_callbacks[index].method)(arg);
  }
}

} // namespace toy

#endif // INCLUDE_CORE_CALLBACKS_POOL_INL_
