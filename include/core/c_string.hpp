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
  \file   c_string.hpp
  \brief  TODO
*/

#ifndef INCLUDE_CORE_C_STRING_HPP_
#define INCLUDE_CORE_C_STRING_HPP_

namespace toy {

class CString {
public:
  /*!
    \brief Default constructor.

    \post The string is empty and ready for use.
  */
  constexpr CString() noexcept;

  /*!
    \brief Constructs a copy of \a string.

    This constructor initializes a string by copying the C-style string pointer from another \a string.

    \param string The source string to copy C-style string pointer from.

    \pre The source \a string must be valid and properly initialized.
  */
  constexpr CString(const CString & string) noexcept;

  /*!
    \brief Constructs a CString from a C-style \a string.

    Creates a new CString object that wraps the provided C-style \a string pointer.

    \param string The C-style string to wrap.

    \pre The source C \a string must not be null.
    \pre The source C \a string must be null-terminated.
  */
  constexpr CString(const char * string) noexcept;

  /*!
    \brief Destructor for the string.

    \note Since the Destructor does not manage dynamic memory, no special cleanup is required.
  */
  constexpr ~CString() noexcept = default;

  /*!
    \brief Copy assigns other \a string to this string.

    This operator assigns the C-style string pointer of another \a string to this string.

    \param string The source string to copy C-style string pointer from.

    \return A reference to this string after assignment.
  */
  constexpr CString & operator=(const CString & string) noexcept;

  /*!
    \brief Copy assigns the C \a string to this string.

    This operator assigns a new C \a string to this string.

    \param string The C-style string to assign.

    \return A reference to this string after assignment.

    \pre The source C \a string must not be null.
    \pre The source C \a string must be null-terminated.
  */
  constexpr CString & operator=(const char * string) noexcept;

  /*!
    \brief Copy assigns other \a string to this string.

    This method assigns the C-style string pointer of another \a string to this string.

    \param string The source string to copy C-style string pointer from.

    \return A reference to this string after assignment.

    \note This method is equivalent to the copy assignment operator.

    \see operator=(const CString & string)
  */
  constexpr CString & assign(const CString & string) noexcept;

  /*!
    \brief Copy assigns the C \a string to this string.

    This method assigns a new C \a string to this string.

    \param string The C-style string to assign.

    \return A reference to this string after assignment.

    \pre The source C \a string must not be null.
    \pre The source C \a string must be null-terminated.

    \note This method is equivalent to the copy assignment operator.

    \see operator=(const char * string)
  */
  constexpr CString & assign(const char * string) noexcept;

  [[nodiscard]] constexpr std::size_t size() const noexcept;

  [[nodiscard]] constexpr const char * c_str() const noexcept;

private:
  /// Pointer to the wrapped C-style string
  const char * _data;

  static constexpr char _emptyString[]{'\0'};
};

} // namespace toy

#endif // INCLUDE_CORE_C_STRING_HPP_
