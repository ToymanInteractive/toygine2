//
// Copyright (c) 2025 Toyman Interactive
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
  \file   array_size.hpp
  \brief  Compile-time array size helper function.
*/

#ifndef INCLUDE_CORE_ARRAY_SIZE_HPP_
#define INCLUDE_CORE_ARRAY_SIZE_HPP_

namespace toy {

/*!
  \brief Returns the size of an array at compile-time.

  This is a compile-time helper function that returns the size of a C-style array. The size of the array is inferred
  from the parameter type, which is a reference to an array of length N. This function is useful for avoiding magic
  numbers and ensuring type safety when working with fixed-size arrays.

  \tparam T The element type of the array.
  \tparam N The size of the array (inferred from the parameter).

  \param array A reference to a C-style array of type T with size N.

  \return The number of elements in the array (N).

  \section usage Usage Example

  \code
  #include "core/array_size.hpp"

  int numbers[] = {1, 2, 3, 4, 5};
  constexpr size_t count = ArraySize(numbers);  // count == 5

  char buffer[256];
  constexpr size_t bufferSize = ArraySize(buffer);  // bufferSize == 256

  constexpr int values[] = {10, 20, 30};
  static_assert(ArraySize(values) == 3);
  \endcode

  \note This function only works with actual arrays, not pointers. Passing a pointer will result in a compile error.
  \note The function is constexpr and can be used in constant expressions, including static_assert.
  \note Prefer this function over sizeof(array) / sizeof(array[0]) for better type safety.
*/
template <typename T, size_t N>
constexpr size_t ArraySize(T (&)[N]) noexcept {
  return N;
}

} // namespace toy

#endif // INCLUDE_CORE_ARRAY_SIZE_HPP_
