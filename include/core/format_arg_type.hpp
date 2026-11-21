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

  Defines \ref toy::FormatArgType, the tag toy::packArg() writes to name the \ref toy::FormatArg union member that
  holds the value. Used wherever an argument pack is built or read.

  \note Included by format_args.hpp and core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_ARG_TYPE_HPP_
#define INCLUDE_CORE_FORMAT_ARG_TYPE_HPP_

namespace toy {

/*!
  \enum  FormatArgType
  \brief The union member of \ref toy::FormatArg that holds the packed value.

  The packer selects the tag from the argument type alone; every other member stays inactive.

  \sa \ref toy::FormatArg
*/
enum class FormatArgType : uint8_t {
  none,      ///< The value carries nothing; the default state of an unpacked argument.
  charValue, ///< The \c character member holds a single byte.
  boolValue, ///< The \c boolean member holds a truth value.
};

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_ARG_TYPE_HPP_
