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

  /*!
    \brief Access a character in the string at a given \a offset.

    This method provides read-only access to a character at the specified \a offset within the string.

    \param offset The offset of the character to access in the string.

    \return A const reference to the character at the specified \a offset.

    \pre The \a offset should be less than the current string size.

    \note The returned reference is read-only and cannot modify the character.
    \note This method is equivalent to the subscript operator.

    \see operator[](std::size_t offset) const
  */
  [[nodiscard]] constexpr const char & at(std::size_t offset) const noexcept;

  /*!
    \brief Access a character in the string at a given \a offset.

    This operator provides read-only access to a character at the specified \a offset within the string.

    \param offset The offset of the character to access in the string.

    \return A const reference to the character at the specified \a offset.

    \pre The \a offset should be less than the current string size.

    \note The returned reference is read-only and cannot modify the character.
  */
  [[nodiscard]] constexpr const char & operator[](std::size_t offset) const noexcept;

  /*!
    \brief Returns a const reference to the first character of the string.

    This method provides read-only access to the first character of the string.

    \return A const reference to the first character of the string.

    \note The returned reference is read-only and cannot modify the character.
  */
  [[nodiscard]] constexpr const char & front() const noexcept;

  /*!
    \brief Returns a const reference to the last character of the string.

    This method provides read-only access to the last character of the string.

    \return A const reference to the last character of the string.

    \pre The string must not be empty.

    \note The returned reference is read-only and cannot modify the character.
  */
  [[nodiscard]] constexpr const char & back() const noexcept;

  /*!
    \brief Returns a constant pointer to the data of the string.

    This method returns a constant pointer to the internal character array that stores the string data. The returned
    pointer provides read-only access to the string contents and can be used for low-level operations.

    \return A constant pointer to the internal character array.

    \note The returned pointer points to a null-terminated character array.
    \note The returned pointer is read-only and cannot modify the string contents.
  */
  [[nodiscard]] constexpr const char * data() const noexcept;

  /*!
    \brief Returns a constant pointer to the C string representation of this string.

    This method returns a constant pointer to the C string representation of this string. The returned pointer provides
    read-only access to the string contents and can be used with C functions that require a char pointer.

    \return A constant pointer to the C string representation of this string.

    \note The returned pointer points to a null-terminated string.
    \note The returned pointer is read-only and cannot modify the string contents.
    \note This method is equivalent to data() const method.
  */
  [[nodiscard]] constexpr const char * c_str() const noexcept;

  /*!
    \brief Checks if the string is empty.

    This method checks if the string is empty, i.e. its size is zero. An empty string contains no characters and has a
    length of zero.

    \return True if the string is empty, false otherwise.

    \note An empty string has size zero.
    \note An empty string still contains a null terminator.
    \note This method is equivalent to the expression: ```size() == 0```.
  */
  [[nodiscard]] constexpr bool empty() const noexcept;

  /*!
    \brief Returns the size of the string.

    This method returns the current number of characters in the string, excluding the terminating null character. The
    size represents the actual length of the string content.

    \return The number of characters in the string, excluding the terminating null character.

    \note This method is equivalent to length() method.
  */
  [[nodiscard]] constexpr std::size_t size() const noexcept;

  /*!
    \brief Returns the size of the Unicode string in UTF-8 encoding.

    This method returns the number of Unicode characters in the UTF-8 encoded string, excluding the terminating null
    character. For ASCII strings, this value equals the size() method. For UTF-8 encoded strings, this method counts the
    number of Unicode characters rather than bytes. The method validates UTF-8 encoding and counts only complete,
    well-formed Unicode characters.

    \return The number of Unicode characters in the string, excluding the terminating null character.

    \note For ASCII strings, utf8_size() equals size().
    \note For UTF-8 strings, utf8_size() may be less than size().
    \note Invalid UTF-8 sequences are handled gracefully and may affect the count.
    \note This method is useful for internationalization and text processing applications.
  */
  [[nodiscard]] inline std::size_t utf8_size() const noexcept;

  /*!
    \brief Returns the size of the string.

    This method returns the current number of characters in the string, excluding the terminating null character. The
    length represents the actual length of the string content.

    \return The number of characters in the string, excluding the terminating null character.

    \note This method is equivalent to size() method.

    \see size()
  */
  [[nodiscard]] constexpr std::size_t length() const noexcept;

private:
  /// Pointer to the wrapped C-style string
  const char * _data;

  /// Static empty string used as default value for null pointers
  static constexpr char _emptyString[]{'\0'};
};

} // namespace toy

#endif // INCLUDE_CORE_C_STRING_HPP_
