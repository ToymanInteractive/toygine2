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
  \file   c_string_view.hpp
  \brief  TODO
*/

#ifndef INCLUDE_CORE_C_STRING_VIEW_HPP_
#define INCLUDE_CORE_C_STRING_VIEW_HPP_

namespace toy {

class CStringView {
public:
  /*!
    \brief Default constructor.

    \post The string view is empty and ready for use.
  */
  constexpr CStringView() noexcept;

  /*!
    \brief Constructs a copy of \a string view.

    This constructor initializes a string view by copying the C-style string pointer from another \a string view.

    \param string The source string view to copy C-style string pointer from.

    \pre The source \a string view must be valid and properly initialized.
  */
  constexpr CStringView(const CStringView & string) noexcept;

  /*!
    \brief Constructs a string view from a C-style \a string view.

    Creates a new CStringView object that wraps the provided C-style \a string pointer.

    \param string The C-style string to wrap.

    \pre The source C \a string must not be null.
    \pre The source C \a string must be null-terminated.
  */
  constexpr CStringView(const char * string) noexcept;

  /*!
    \brief Destructor for the string view.

    \note Since the Destructor does not manage dynamic memory, no special cleanup is required.
  */
  constexpr ~CStringView() noexcept = default;

  /*!
    \brief Copy assigns other \a string view to this string view.

    This operator assigns the C-style string pointer of another \a string view to this string view.

    \param string The source string view to copy C-style string pointer from.

    \return A reference to this string view after assignment.
  */
  constexpr CStringView & operator=(const CStringView & string) noexcept;

  /*!
    \brief Copy assigns the C \a string to this string view.

    This operator assigns a new C \a string to this string view.

    \param string The C-style string to assign.

    \return A reference to this string view after assignment.

    \pre The source C \a string must not be null.
    \pre The source C \a string must be null-terminated.
  */
  constexpr CStringView & operator=(const char * string) noexcept;

  /*!
    \brief Copy assigns other \a string view to this string view.

    This method assigns the C-style string pointer of another \a string view to this string view.

    \param string The source string view to copy C-style string pointer from.

    \return A reference to this string view after assignment.

    \note This method is equivalent to the copy assignment operator.

    \see operator=(const CStringView & string)
  */
  constexpr CStringView & assign(const CStringView & string) noexcept;

  /*!
    \brief Copy assigns the C \a string to this string view.

    This method assigns a new C \a string to this string view.

    \param string The C-style string to assign.

    \return A reference to this string view after assignment.

    \pre The source C \a string must not be null.
    \pre The source C \a string must be null-terminated.

    \note This method is equivalent to the copy assignment operator.

    \see operator=(const char * string)
  */
  constexpr CStringView & assign(const char * string) noexcept;

  /*!
    \brief Access a character in the string view at a given \a offset.

    This method provides read-only access to a character at the specified \a offset within the string view.

    \param offset The offset of the character to access in the string view.

    \return A const reference to the character at the specified \a offset.

    \pre The \a offset should be less than the current string view size.

    \note The returned reference is read-only and cannot modify the character.
    \note This method is equivalent to the subscript operator.

    \see operator[](std::size_t offset) const
  */
  [[nodiscard]] constexpr const char & at(std::size_t offset) const noexcept;

  /*!
    \brief Access a character in the string view at a given \a offset.

    This operator provides read-only access to a character at the specified \a offset within the string view.

    \param offset The offset of the character to access in the string view.

    \return A const reference to the character at the specified \a offset.

    \pre The \a offset should be less than the current string view size.

    \note The returned reference is read-only and cannot modify the character.
  */
  [[nodiscard]] constexpr const char & operator[](std::size_t offset) const noexcept;

  /*!
    \brief Returns a const reference to the first character of the string view.

    This method provides read-only access to the first character of the string view.

    \return A const reference to the first character of the string view.

    \note The returned reference is read-only and cannot modify the character.
  */
  [[nodiscard]] constexpr const char & front() const noexcept;

  /*!
    \brief Returns a const reference to the last character of the string view.

    This method provides read-only access to the last character of the string view.

    \return A const reference to the last character of the string view.

    \pre The string view must not be empty.

    \note The returned reference is read-only and cannot modify the character.
  */
  [[nodiscard]] constexpr const char & back() const noexcept;

  /*!
    \brief Returns a constant pointer to the data of the string view.

    This method returns a constant pointer to the internal character array that stores the string view data. The
    returned pointer provides read-only access to the string view contents and can be used for low-level operations.

    \return A constant pointer to the internal character array.

    \note The returned pointer points to a null-terminated character array.
    \note The returned pointer is read-only and cannot modify the string view contents.
  */
  [[nodiscard]] constexpr const char * data() const noexcept;

  /*!
    \brief Returns a constant pointer to the C string representation of this string view.

    This method returns a constant pointer to the C string representation of this string view. The returned pointer
    provides read-only access to the string view contents and can be used with C functions that require a char pointer.

    \return A constant pointer to the C string representation of this string view.

    \note The returned pointer points to a null-terminated string view.
    \note The returned pointer is read-only and cannot modify the string view contents.
    \note This method is equivalent to data() const method.
  */
  [[nodiscard]] constexpr const char * c_str() const noexcept;

  /*!
    \brief Checks if the string view is empty.

    This method checks if the string view is empty, i.e. its size is zero. An empty string view contains no characters
    and has a length of zero.

    \return True if the string view is empty, false otherwise.

    \note An empty string view has size zero.
    \note An empty string view still contains a null terminator.
    \note This method is equivalent to the expression: ```size() == 0```.
  */
  [[nodiscard]] constexpr bool empty() const noexcept;

  /*!
    \brief Returns the size of the string view.

    This method returns the current number of characters in the string view, excluding the terminating null character.
    The size represents the actual length of the string view content.

    \return The number of characters in the string view, excluding the terminating null character.

    \note This method is equivalent to length() method.
  */
  [[nodiscard]] constexpr std::size_t size() const noexcept;

  /*!
    \brief Returns the size of the Unicode string in UTF-8 encoding.

    This method returns the number of Unicode characters in the UTF-8 encoded string, excluding the terminating null
    character. For ASCII strings, this value equals the size() method. For UTF-8 encoded strings, this method counts the
    number of Unicode characters rather than bytes. The method validates UTF-8 encoding and counts only complete,
    well-formed Unicode characters.

    \return The number of Unicode characters in the string view, excluding the terminating null character.

    \note For ASCII strings, utf8_size() equals size().
    \note For UTF-8 strings, utf8_size() may be less than size().
    \note Invalid UTF-8 sequences are handled gracefully and may affect the count.
    \note This method is useful for internationalization and text processing applications.
  */
  [[nodiscard]] inline std::size_t utf8_size() const noexcept;

  /*!
    \brief Returns the size of the string view.

    This method returns the current number of characters in the string view, excluding the terminating null character.
    The length represents the actual length of the string view content.

    \return The number of characters in the string view, excluding the terminating null character.

    \note This method is equivalent to size() method.

    \see size()
  */
  [[nodiscard]] constexpr std::size_t length() const noexcept;

  /*!
    \brief Returns the maximum observable size of this string view.

    This method returns the maximum possible size for the string view. The maximum size represents the size of the
    allocated buffer.

    \return The maximum number of characters in the string view, excluding the terminating null character.

    \note This method is equivalent to size() method.
  */
  [[nodiscard]] constexpr std::size_t max_size() const noexcept;

  /*!
    \brief Returns the capacity of the string view.

    This method returns the capacity of the allocated buffer for the string view.

    \return The capacity of the string view in characters, excluding the terminating null character.

    \note This method is equivalent to size() method.
  */
  [[nodiscard]] constexpr std::size_t capacity() const noexcept;

  /*!
    \brief Clears the contents by detaching from the current C string.

    This method resets this view to point at the internal empty sentinel. The previously referenced character buffer is
    not modified or freed.

    \note No allocation or deallocation occurs.
  */
  constexpr void clear() noexcept;

private:
  /// Pointer to the wrapped C-style string
  const char * _data;

  /// Static empty C-style string used as default value for null pointers
  static constexpr char _emptyString[]{'\0'};
};

} // namespace toy

#endif // INCLUDE_CORE_C_STRING_VIEW_HPP_
