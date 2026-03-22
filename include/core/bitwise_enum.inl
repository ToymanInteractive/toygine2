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
  \file   bitwise_enum.inl
  \brief  Inline implementations for \ref toy::EnableBitwiseOperators and scoped-enum bitwise operators.

  Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_BITWISE_ENUM_INL_
#define INCLUDE_CORE_BITWISE_ENUM_INL_

namespace toy {

template <typename T>
  requires EnableBitwiseOperators<T>::enable
[[nodiscard]] constexpr T operator|(T lhs, T rhs) noexcept {
  return static_cast<T>(std::to_underlying(lhs) | std::to_underlying(rhs));
}

template <typename T>
  requires EnableBitwiseOperators<T>::enable
[[nodiscard]] constexpr T operator&(T lhs, T rhs) noexcept {
  return static_cast<T>(std::to_underlying(lhs) & std::to_underlying(rhs));
}

template <typename T>
  requires EnableBitwiseOperators<T>::enable
[[nodiscard]] constexpr T operator^(T lhs, T rhs) noexcept {
  return static_cast<T>(std::to_underlying(lhs) ^ std::to_underlying(rhs));
}

template <typename T>
  requires EnableBitwiseOperators<T>::enable
[[nodiscard]] constexpr T operator~(T lhs) noexcept {
  return static_cast<T>(~std::to_underlying(lhs));
}

template <typename T>
  requires EnableBitwiseOperators<T>::enable
constexpr T & operator|=(T & lhs, T rhs) noexcept {
  return lhs = lhs | rhs;
}

template <typename T>
  requires EnableBitwiseOperators<T>::enable
constexpr T & operator&=(T & lhs, T rhs) noexcept {
  return lhs = lhs & rhs;
}

template <typename T>
  requires EnableBitwiseOperators<T>::enable
constexpr T & operator^=(T & lhs, T rhs) noexcept {
  return lhs = lhs ^ rhs;
}

} // namespace toy

#endif // INCLUDE_CORE_BITWISE_ENUM_INL_
