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
  \file   callbacks_pool.hpp
  \brief  TODO
*/

#ifndef INCLUDE_CORE_CALLBACKS_POOL_HPP_
#define INCLUDE_CORE_CALLBACKS_POOL_HPP_

namespace toy {

template <typename type, std::size_t allocatedSize = 4>
class CallbacksPool {
public:
  constexpr CallbacksPool() noexcept;
  constexpr ~CallbacksPool() noexcept = default;

  constexpr bool add(void (*method)(type arg)) noexcept;
  constexpr bool remove(void (*method)(type arg)) noexcept;
  constexpr void reset() noexcept;

  [[nodiscard]] constexpr std::size_t subscribersAmount() const noexcept;

  constexpr void call(type parameter) const noexcept;

private:
  struct StaticCallback {
    void (*method)(type arg) = nullptr;
  };

  std::array<StaticCallback, allocatedSize> _callbacks;
};

} // namespace toy

#endif // INCLUDE_CORE_CALLBACKS_POOL_HPP_
