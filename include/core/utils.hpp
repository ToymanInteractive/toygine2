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
  two, three, or four bytes counts once. The walk trusts the width the lead byte announces; toy::validateUtf8() is what
  checks the bytes behind it.

  \param str Null-terminated UTF-8 string to measure.

  \return Count of encoded characters, \c 0 over an empty string, and \c 0 where the walk stops early.

  \pre \a str is non-null, checked by assert_message in debug builds; a shipping build counts \c 0 instead.
  \pre \a str holds well-formed UTF-8, checked with toy::validateUtf8() by assert_message in debug builds. A shipping
       build skips that check: a byte starting no sequence and a sequence the terminator cuts short each end the count
       at \c 0, while an overlong, a surrogate, and a code point past U+10FFFF each count as one character.

  \note The count is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.
  \note The walk stops at the terminator and never reads past it, whatever width a lead byte announces.

  \sa toy::validateUtf8()
*/
[[nodiscard]] size_t utf8Len(const char * str) noexcept;

/*!
  \brief Reports whether a byte string is well-formed UTF-8.

  Checks every sequence against Unicode table 3-7: the lead byte, the narrowed range of the byte after it, and the
  10xxxxxx form of the rest. That rejects an overlong encoding, a surrogate, and a code point past U+10FFFF, each of
  which a lead byte alone admits.

  \param str Null-terminated byte string to check.

  \return \c true when every sequence is well-formed, \c false at the first that is not and \c false for a null \a str.
          An empty string is well-formed.

  \note Asserts nothing, so it reads as the condition of an assert_message check — which is how utf8Len() states its
        precondition.
  \note The check is a runtime one: the definition lives in a translation unit, out of reach of a constant expression.
  \note Cost is O(n) in the length of the string, one pass, so a caller checking before a walk pays for two.

  \sa toy::utf8Len()
*/
[[nodiscard]] bool validateUtf8(const char * str) noexcept;

} // namespace toy

#endif // INCLUDE_CORE_UTILS_HPP_
