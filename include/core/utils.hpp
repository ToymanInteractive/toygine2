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
  \file   utils.hpp
  \brief  Utility functions the core module shares across its types.

  Declares toy::utf8Len(), which reads a byte string as UTF-8 and reports how many characters it encodes. Called where
  a byte length has to become a character count, as \ref toy::CStringView does.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_UTILS_HPP_
#define INCLUDE_CORE_UTILS_HPP_

namespace toy {

/*!
  \brief Counts the characters a UTF-8 string encodes.

  Walks the string one lead byte at a time and steps over the sequence that byte announces, so a character stored in
  two, three, or four bytes counts once.

  \param str Null-terminated UTF-8 string to measure.

  \return Count of encoded characters, \c 0 over an empty string, and \c 0 when a byte starts no valid sequence or
          the terminator cuts one short.

  \pre \a str is non-null, checked by assert_message in debug builds; a shipping build counts \c 0 instead.
  \pre \a str holds well-formed UTF-8. A malformed byte and a truncated sequence each fail an assert_message check in
       debug builds and end the count at \c 0 in a shipping build.

  \note The count is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.
  \note The walk stops at the terminator and never reads past it, whatever width a lead byte announces.
  \note Continuation bytes are tested against the terminator alone; their values stay unvalidated.
*/
[[nodiscard]] size_t utf8Len(const char * str) noexcept;

} // namespace toy

#endif // INCLUDE_CORE_UTILS_HPP_
