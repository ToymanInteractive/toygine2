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
  \file   string.inl
  \brief  Inline implementations for \ref toy::String constructors and accessors.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_STRING_INL_
#define INCLUDE_CORE_STRING_INL_

namespace toy {

template <StringStorage storageType>
constexpr String<storageType>::String(const value_type * string) noexcept
  : String(string, string != nullptr ? traits_type::length(string) : 0) {
  assert_message(string != nullptr, "C string must not be null");
}

template <StringStorage storageType>
constexpr String<storageType>::size_type String<storageType>::size() const noexcept {
  return _storage.size();
}

template <StringStorage storageType>
constexpr String<storageType>::const_pointer String<storageType>::c_str() const noexcept {
  return _storage.data();
}

} // namespace toy

#endif // INCLUDE_CORE_STRING_INL_
