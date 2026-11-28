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
  \file   format_arg_type.hpp
  \brief  The tag naming which union member of a format argument holds the value.

  Defines \ref toy::detail::FormatArgType, the tag toy::detail::packArg() writes to name the \ref toy::detail::FormatArg
  union member that holds the value. Used wherever an argument pack is built or read.
*/

#ifndef INCLUDE_CORE_DETAIL_FORMAT_ARG_TYPE_HPP_
#define INCLUDE_CORE_DETAIL_FORMAT_ARG_TYPE_HPP_

namespace toy::detail {

/*!
  \enum  FormatArgType
  \brief The union member of \ref toy::detail::FormatArg that holds the packed value.

  The packer selects the tag from the argument type alone; every other member stays inactive. An integer argument
  packs to the slot of its own width, so the exact type the caller passed stays visible in the pack.

  \sa \ref toy::detail::FormatArg
*/
enum class FormatArgType : uint8_t {
  none,        ///< The value carries nothing; the default state of an unpacked argument.
  charValue,   ///< The \c character member holds a single byte.
  boolValue,   ///< The \c boolean member holds a truth value.
  int8Value,   ///< The \c int8 member holds an 8-bit signed integer.
  int16Value,  ///< The \c int16 member holds a 16-bit signed integer.
  int32Value,  ///< The \c int32 member holds a 32-bit signed integer.
  int64Value,  ///< The \c int64 member holds a 64-bit signed integer.
  uint8Value,  ///< The \c uint8 member holds an 8-bit unsigned integer.
  uint16Value, ///< The \c uint16 member holds a 16-bit unsigned integer.
  uint32Value, ///< The \c uint32 member holds a 32-bit unsigned integer.
  uint64Value, ///< The \c uint64 member holds a 64-bit unsigned integer.
};

} // namespace toy::detail

#endif // INCLUDE_CORE_DETAIL_FORMAT_ARG_TYPE_HPP_
