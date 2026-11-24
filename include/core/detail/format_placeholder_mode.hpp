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
  \file   format_placeholder_mode.hpp
  \brief  The placeholder style a format pattern walk has seen.

  Defines \ref toy::detail::PlaceholderMode, the tag the pattern grammar helpers set as they read a pattern. Internal to
  the format implementation.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_DETAIL_FORMAT_PLACEHOLDER_MODE_HPP_
#define INCLUDE_CORE_DETAIL_FORMAT_PLACEHOLDER_MODE_HPP_

namespace toy::detail {

/// Tracks which placeholder style the pattern has used so far.
enum class PlaceholderMode {
  none,       ///< The pattern carries no placeholder yet.
  autoIndex,  ///< An auto placeholder has been seen.
  positional, ///< A positional placeholder has been seen.
};

} // namespace toy::detail

#endif // INCLUDE_CORE_DETAIL_FORMAT_PLACEHOLDER_MODE_HPP_
