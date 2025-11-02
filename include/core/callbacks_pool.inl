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
  \file   callbacks_pool.inl
  \brief  Inline implementations for template callback pool class with fixed-size callback storage.
*/

#ifndef INCLUDE_CORE_CALLBACKS_POOL_INL_
#define INCLUDE_CORE_CALLBACKS_POOL_INL_

namespace toy {

template <typename type, std::size_t allocatedSize>
constexpr CallbacksPool<type, allocatedSize>::CallbacksPool() noexcept
  : _callbacks() {
  reset();
}

template <typename type, std::size_t allocatedSize>
constexpr bool CallbacksPool<type, allocatedSize>::add(void (*method)(type tArg)) noexcept {
  assert_message(method != nullptr, "Cannot add null callback");

  // Check if the method already exists
  for (auto const & callback : _callbacks) {
    if (callback.method == method)
      return true;
  }

  // Find a free slot
  for (auto & callback : _callbacks) {
    if (callback.method == nullptr) {
      callback.method = method;
      return true;
    }
  }

  // Should never get here if pool size is sufficient
  assert_message(false, "No room for new callback, increase pool size");
  return false;
}

template <typename type, std::size_t allocatedSize>
constexpr bool CallbacksPool<type, allocatedSize>::remove(void (*method)(type arg)) noexcept {
  for (auto & callback : _callbacks) {
    if (callback.method == method) {
      callback.method = nullptr;
      return true;
    }
  }

  return false;
}

template <typename type, std::size_t allocatedSize>
constexpr void CallbacksPool<type, allocatedSize>::reset() noexcept {
  _callbacks.fill(StaticCallback());
}

template <typename type, std::size_t allocatedSize>
[[nodiscard]] constexpr std::size_t CallbacksPool<type, allocatedSize>::subscribersAmount() const noexcept {
  std::size_t subscribers = 0;

  for (auto const & callback : _callbacks) {
    if (callback.method != nullptr)
      ++subscribers;
  }

  return subscribers;
}

template <typename type, std::size_t allocatedSize>
constexpr void CallbacksPool<type, allocatedSize>::call(type arg) const noexcept {
  for (auto const & callback : _callbacks) {
    if (callback.method != nullptr)
      (*callback.method)(arg);
  }
}

} // namespace toy

#endif // INCLUDE_CORE_CALLBACKS_POOL_INL_
