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
  \file   format.hpp
  \brief  Compile-time format string validation class.
*/

#ifndef INCLUDE_CORE_FORMAT_HPP_
#define INCLUDE_CORE_FORMAT_HPP_

#include "string_like.hpp"

namespace toy {

/*!
  \class FormatString
  \ingroup String
  \brief Compile-time validated format string wrapper.

  FormatString is a template class that provides compile-time validation of format strings, similar to
  \c std::format_string. It ensures that the format string has valid syntax and that the number of placeholders matches
  the number of template arguments.

  The format string uses `{}` as placeholders, following the same rules as \c std::format. Escaped braces `{{` and `}}`
  are treated as literal braces and do not count as placeholders.

  \tparam Args Template parameter pack representing the types of arguments that will be formatted.

  \section features Key Features

  - 🔍 **Compile-Time Validation**: Format string syntax and placeholder count verified at compile time
  - 🎯 **Placeholder Verification**: Ensures placeholder count matches template arguments
  - 🔧 **ConstExpr Support**: Constructor and methods can be evaluated at compile time
  - 🛡️ **Exception Safety**: All operations are noexcept
  - 🔗 **STL Compatibility**: Similar interface to \c std::format_string

  \section usage Usage Example

  \code
  constexpr FormatString<int, float> format(CStringView("Value: {}, Float: {}"));
  constexpr auto str = format.get(); // Returns CStringView("Value: {}, Float: {}")

  // Invalid format strings cause compile-time errors:
  // FormatString<int> format1(CStringView("Value: {}")); // OK
  // FormatString<int> format2(CStringView("Value: {}, {}")); // Compile error: count mismatch
  // FormatString<int> format3(CStringView("Value: {")); // Compile error: unmatched brace
  \endcode

  \note The constructor is marked as \c consteval, meaning it must be evaluated at compile time.
  \note Invalid format strings result in compile-time errors, not runtime exceptions.
  \note This class is similar to \c std::format_string but designed for use with \ref toy::CStringView.

  \sa CStringView
  \sa StringLike
*/
template <class... Args>
class FormatString {
public:
  /*!
    \brief Constructs a FormatString with compile-time validation.

    This constructor validates the format string at compile time, ensuring:
    - The format string has valid brace matching (no unmatched braces)
    - The number of placeholders `{}` matches the number of template arguments

    \param string The format string to validate and store.

    \pre The \a string must be a valid format string with proper brace matching.
    \pre The number of placeholders in \a string must equal \c sizeof...(Args).

    \post The format string is stored and can be retrieved via get().

    \note If validation fails, a compile-time error is generated.
    \note Escaped braces `{{` and `}}` are not counted as placeholders.
    \note The constructor is \c consteval, so it must be evaluated at compile time.

    \sa get()
  */
  consteval explicit(false) FormatString(const CStringView & string) noexcept;

  /*!
    \brief Returns a copy of the stored format string.

    This method returns a copy of the \ref toy::CStringView containing the format string. Since \ref toy::CStringView
    is a lightweight non-owning view, copying it is inexpensive and does not involve memory allocation.

    \return A copy of the \ref toy::CStringView containing the format string.

    \note This method is constexpr-compatible and can be used in compile-time contexts.
    \note The returned \ref toy::CStringView is a non-owning view and does not copy the underlying string data.

    \sa FormatString(const CStringView &)
    \sa CStringView
  */
  constexpr CStringView get() const noexcept;

private:
  const CStringView _string; //!< The stored format string.

  /*!
    \brief Counts the number of format placeholders in a format string.

    This function counts the number of `{}` placeholders in the format string, ignoring escaped braces `{{` and `}}`.
    The function follows the same rules as \c std::format for placeholder counting.

    \param string The format string to analyze.

    \return The number of format placeholders found in the string, or \ref toy::CStringView::npos if the format string
            contains unmatched braces.

    \note Escaped braces `{{` and `}}` are not counted as placeholders.
    \note This function is constexpr-compatible and can be used in compile-time contexts.
    \note Returns \ref toy::CStringView::npos if unmatched braces are detected.

    \sa CStringView::npos
  */
  [[nodiscard]] static constexpr size_t _countFormatPlaceholders(const CStringView & string) noexcept;

  inline void _compileTimeError(const char *) noexcept {
    // Intentionally cause a compile-time error
  }
};

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_HPP_
