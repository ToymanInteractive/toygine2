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
  \file   fix_string.hpp
  \brief  Template string class with fixed-size character buffer, wrapper to char[]
*/

#ifndef INCLUDE_CORE_FIX_STRING_HPP_
#define INCLUDE_CORE_FIX_STRING_HPP_

namespace toygine {

template <std::size_t allocatedSize>
class FixString {
public:
  /*!
    \brief Default constructor.

    \post The string is empty and ready for use.
  */
  constexpr inline FixString() noexcept;

  /*!
    \brief Destructor for the string.

    This destructor cleans up the string.

    \note Since the FixString does not manage dynamic memory, no special cleanup is required.
  */
  inline ~FixString() noexcept;

  /*!
    \brief Constructs a copy of \a string.

    This constructor initializes a string by copying the content from another \a string.

    \param string The source string to copy content from.

    \pre The source \a string must be valid and properly initialized.

    \post The new string has the same size as the source \a string.
  */
  constexpr inline FixString(const FixString<allocatedSize> & string) noexcept;

  /*!
    \brief Constructs a string initialized with a StringLike object.

    This constructor initializes a string by copying the content from a StringLike object.

    \tparam stringType The type of the source string. Must satisfy the StringLike concept.

    \param string The source StringLike object to copy content from.

    \pre The source StringLike object must be valid and properly initialized.
    \pre The source StringLike object size must not exceed the allocated capacity.

    \post The new string is created with the contents of the source StringLike object.
  */
  template <StringLike stringType>
  constexpr inline explicit FixString(const stringType & string) noexcept;

  /*!
    \brief Constructs a string initialized with the C \a string.

    This constructor initializes a string by copying the content from the C \a string. The constructor automatically
    determines the length of the source string and copies the content up to the allocated capacity.

    \param string The source C string to copy content from.

    \pre The source \a string must not be null.

    \post The new string is created with the contents of the source C \a string.
  */
  constexpr explicit inline FixString(const char * string) noexcept;

  /*!
    \brief Constructs a string of the given \a count of \a character.

    This constructor initializes a string with a \a character repeated the given \a count of times.

    \param character The character to repeat.
    \param count     The number of times to repeat the \a character (default: 1).

    \pre The \a count must not exceed the allocated size.

    \post The new string is created with the contents of the \a character repeated the given \a count of times.
    \post The string size equals the \a count.

    \note This is useful for creating strings with repeated patterns or filling strings with specific characters.
  */
  constexpr inline explicit FixString(char character, std::size_t count = 1) noexcept;

  /*!
    \brief Copy assigns other \a string to this string.

    This operator assigns the contents of another \a string to this string. The assignment operation performs a deep
    copy of the string data and updates the size accordingly.

    \param string The source string to copy content from.

    \return A reference to this string after assignment.

    \pre The source \a string must be valid and properly initialized.

    \post This string contains the same content as the source \a string.
    \post The size of this string equals the size of the source \a string.

    \note Self-assignment is handled correctly and safely.
  */
  constexpr inline FixString<allocatedSize> & operator=(const FixString<allocatedSize> & string) noexcept;

  /*!
    \brief Copy assigns a StringLike object to this string.

    This operator assigns the content from a StringLike object to this string. The assignment operation performs a deep
    copy of the string data and updates the size accordingly.

    \tparam stringType The type of the source string. Must satisfy the StringLike concept.

    \param string The source StringLike object to copy content from.

    \return A reference to this string after assignment.

    \pre The source StringLike object must be valid and properly initialized.
    \pre The source StringLike object size must not exceed the allocated capacity.

    \post This string contains the same content as the source StringLike object.
    \post The size of this string equals the size of the source StringLike object.

    \note Self-assignment is handled correctly and safely.
  */
  template <StringLike stringType>
  constexpr inline FixString<allocatedSize> & operator=(const stringType & string) noexcept;

  /*!
    \brief Copy assigns the C \a string to this string.

    This operator assigns the content from the C \a string to this string. The assignment operation automatically
    determines the length of the source content and performs a deep copy of the string data and updates the size
    accordingly.

    \param string The source C string to copy content from.

    \return A reference to this string after assignment.

    \pre The source C \a string must not be null.
    \pre The source C \a string size must not exceed the allocated capacity.

    \post This string contains the same content as the source C \a string.
    \post The size of this string equals the size of the source C \a string.

    \note Self-assignment is handled correctly and safely.
  */
  constexpr inline FixString<allocatedSize> & operator=(const char * string) noexcept;

  /*!
    \brief Character assignment to this string.

    This operator assigns a single \a character to this string, creating a string of length 1.

    \param character The character to assign.

    \return A reference to this string after assignment.

    \post This string contains only one specified \a character.

    \note This is useful for resetting a string to contain only a single character.
  */
  constexpr inline FixString<allocatedSize> & operator=(char character) noexcept;

  /*!
    \brief Copy assigns other \a string to this string.

    This method assigns the contents of another \a string to this string. The assignment operation performs a deep
    copy of the string data and updates the size accordingly.

    \param string The source string to copy content from.

    \return A reference to this string after assignment.

    \pre The source \a string must be valid and properly initialized.

    \post This string contains the same content as the source \a string.
    \post The size of this string equals the size of the source \a string.

    \note Self-assignment is handled correctly and safely.
    \note This method is equivalent to the copy assignment operator.

    \see operator=(const FixString<allocatedSize> & string)
  */
  constexpr inline FixString<allocatedSize> & assign(const FixString<allocatedSize> & string) noexcept;

  /*!
    \brief Copy assigns a StringLike object to this string.

    This method assigns the content from a StringLike object to this string. The assignment operation performs a deep
    copy of the string data and updates the size accordingly.

    \tparam stringType The type of the source string. Must satisfy the StringLike concept.

    \param string The source StringLike object to copy content from.

    \return A reference to this string after assignment.

    \pre The source StringLike object must be valid and properly initialized.
    \pre The source StringLike object size must not exceed the allocated capacity.

    \post This string contains the same content as the source StringLike object.
    \post The size of this string equals the size of the source StringLike object.

    \note Self-assignment is handled correctly and safely.
    \note This method is equivalent to the copy assignment operator.

    \see operator=(const stringType & string)
  */
  template <StringLike stringType>
  constexpr inline FixString<allocatedSize> & assign(const stringType & string) noexcept;

  /*!
    \brief Copy assigns the C \a string to this string.

    This method assigns the content from the C \a string to this string. The assignment operation automatically
    determines the length of the source content and performs a deep copy of the string data and updates the size
    accordingly.

    \param string The source C string to copy content from.

    \return A reference to this string after assignment.

    \pre The source C \a string must not be null.
    \pre The source C \a string size must not exceed the allocated capacity.

    \post This string contains the same content as the source C \a string.
    \post The size of this string equals the size of the source C \a string.

    \note Self-assignment is handled correctly and safely.
    \note This method is equivalent to the copy assignment operator.

    \see operator=(const char * string)
  */
  constexpr inline FixString<allocatedSize> & assign(const char * string) noexcept;

  /*!
    \brief Character fill assign method for the string.

    This method assigns a \a character repeated the given \a count of times to this string.

    \param character The character to repeat.
    \param count     The number of times to repeat the \a character (default: 1).

    \return A reference to this string after assignment.

    \pre The \a count must not exceed this object's allocated size.

    \post This string object contains the specified \a character repeated \a count of times.
    \post The string size equals the \a count parameter.

    \note This is useful for creating strings with repeated patterns or filling strings with specific characters.
  */
  constexpr inline FixString<allocatedSize> & assign(char character, std::size_t count = 1) noexcept;

  /*!
    \brief Access a character in the string at a given \a offset.

    This method provides direct access to a character at the specified \a offset within the string.

    \param offset The offset of the character to access in the string.

    \return A reference to the character at the specified \a offset.

    \pre The \a offset should be less than the current string size.

    \note The returned reference allows modification of the character.
    \note Use const version for read-only access.
    \note This method is equivalent to the subscript operator.

    \see operator[](std::size_t offset)
  */
  constexpr inline char & at(std::size_t offset) noexcept;

  /*!
    \brief Access a character in the string at a given \a offset.

    This method provides read-only access to a character at the specified \a offset within the string.

    \param offset The offset of the character to access in the string.

    \return A const reference to the character at the specified \a offset.

    \pre The \a offset should be less than the current string size.

    \note The returned reference is read-only and cannot modify the character.
    \note Use the non-const overload to allow modification.
    \note This method is equivalent to the subscript operator.

    \see operator[](std::size_t offset) const
  */
  constexpr inline const char & at(std::size_t offset) const noexcept;

  /*!
    \brief Access a character in the string at a given \a offset.

    This operator provides direct access to a character at the specified \a offset within the string.

    \param offset The offset of the character to access in the string.

    \return A reference to the character at the specified \a offset.

    \pre The \a offset should be less than the current string size.

    \note The returned reference allows modification of the character.
    \note Use const version for read-only access.
  */
  constexpr inline char & operator[](std::size_t offset) noexcept;

  /*!
    \brief Access a character in the string at a given \a offset.

    This operator provides read-only access to a character at the specified \a offset within the string.

    \param offset The offset of the character to access in the string.

    \return A const reference to the character at the specified \a offset.

    \pre The \a offset should be less than the current string size.

    \note The returned reference is read-only and cannot modify the character.
    \note Use the non-const overload to allow modification.
  */
  constexpr inline const char & operator[](std::size_t offset) const noexcept;

  /*!
    \brief Returns a pointer to the data of the string.

    This method returns a pointer to the internal character array that stores the string data. The returned pointer
    provides direct access to the string contents and can be used for low-level operations.

    \return A pointer to the internal character array.

    \note The returned pointer points to a null-terminated character array.
    \note The returned pointer allows modification of the string contents.
    \note Use const version for read-only access.
  */
  constexpr inline char * data() noexcept;

  /*!
    \brief Returns a constant pointer to the data of the string.

    This method returns a constant pointer to the internal character array that stores the string data. The returned
    pointer provides read-only access to the string contents and can be used for low-level operations.

    \return A constant pointer to the internal character array.

    \note The returned pointer points to a null-terminated character array.
    \note The returned pointer is read-only and cannot modify the string contents.
    \note Use the non-const overload to allow modification.
  */
  constexpr inline const char * data() const noexcept;

  /*!
    \brief Returns a constant pointer to the C string representation of this string.

    This method returns a constant pointer to the C string representation of this string. The returned pointer provides
    read-only access to the string contents and can be used with C functions that require a char pointer.

    \return A constant pointer to the C string representation of this string.

    \note The returned pointer points to a null-terminated string.
    \note The returned pointer is read-only and cannot modify the string contents.
    \note This method is equivalent to data() const method.
  */
  constexpr inline const char * c_str() const noexcept;

  /*!
    \brief Checks if the string is empty.

    This method checks if the string is empty, i.e. its size is zero. An empty string contains no characters and has a
    length of zero.

    \return True if the string is empty, false otherwise.

    \note An empty string has size zero.
    \note An empty string still contains a null terminator.
    \note This method is equivalent to the expression: ```size() == 0```.
  */
  constexpr inline bool empty() const noexcept;

  /*!
    \brief Returns the size of the string.

    This method returns the current number of characters in the string, excluding the terminating null character. The
    size represents the actual length of the string content.

    \return The number of characters in the string, excluding the terminating null character.

    \note This method is equivalent to length() method.
  */
  constexpr inline std::size_t size() const noexcept;

  /*!
    \brief Returns the size of the Unicode string in UTF-8 encoding.

    This method returns the number of Unicode characters in the UTF-8 encoded string, excluding the terminating null
    character. For ASCII strings, this value equals the size() method. For UTF-8 encoded strings, this method counts the
    number of Unicode characters rather than bytes.

    \return The number of Unicode characters in the string, excluding the terminating null character.

    \note For ASCII strings, utf8Size() equals size().
    \note For UTF-8 strings, utf8Size() may be less than size().
  */
  constexpr inline std::size_t utf8Size() const noexcept;

  /*!
    \brief Returns the size of the string.

    This method returns the current number of characters in the string, excluding the terminating null character. The
    length represents the actual length of the string content.

    \return The number of characters in the string, excluding the terminating null character.

    \note This method is equivalent to size() method.

    \see size()
  */
  constexpr inline std::size_t length() const noexcept;

  /*!
    \brief Returns the maximum size of the string.

    This method returns the maximum possible size for the string, which is determined by the template parameter
    allocatedSize. The maximum size represents the size of the allocated buffer.

    \return The maximum number of characters in the string, excluding the terminating null character.

    \note The maximum size is determined at compile time.
    \note This method is equivalent to capacity() method.
  */
  constexpr inline std::size_t max_size() const noexcept;

  /*!
    \brief Returns the capacity of the string.

    This method returns the capacity of the allocated buffer for the string, which is determined by the template
    parameter allocatedSize.

    \return The capacity of the string in characters, excluding the terminating null character.

    \note The capacity is determined at compile time.
    \note This method is equivalent to max_size() method.
  */
  constexpr inline std::size_t capacity() const noexcept;

  /*!
    \brief Clears the contents of the string.

    This method resets the size of the string to zero and effectively clears the string.

    \note The string object is cleared, and its size is set to zero.
  */
  constexpr inline void clear() noexcept;

  /*!
    \brief Inserts other \a string at the specified \a index.

    This method inserts the contents from another \a string at the specified position. The insertion shifts existing
    characters to the right to make room for the new content.

    \param index  The position where the \a string should be inserted.
    \param string The source string to insert content from.

    \return A reference to this string after insertion.

    \pre The \a index must be within the bounds of the current string.
    \pre The combined length after insertion must not exceed the allocated size.
  */
  constexpr inline FixString<allocatedSize> & insert(std::size_t index,
                                                     const FixString<allocatedSize> & string) noexcept;

  /*!
    \brief Inserts a StringLike object at the specified \a index.

    This method inserts the contents from a StringLike object at the specified position. The insertion shifts existing
    characters to the right to make room for the new content.

    \tparam stringType The type of the source string. Must satisfy the StringLike concept.

    \param index  The position where the StringLike object should be inserted.
    \param string The source StringLike object to insert content from.

    \return A reference to this string after insertion.

    \pre The \a index must be within the bounds of the current string.
    \pre The combined length after insertion must not exceed the allocated size.
  */
  template <StringLike stringType>
  constexpr inline FixString<allocatedSize> & insert(std::size_t index, const stringType & string) noexcept;

  /*!
    \brief Inserts the C \a string at the specified \a index.

    This method inserts the contents of the C \a string at the specified position. The insertion shifts existing
    characters to the right to make room for the new content.

    \param index  The position where the C \a string should be inserted.
    \param string The source C string to insert content from.

    \return A reference to this string after insertion.

    \pre The \a index must be within the bounds of the current string.
    \pre The combined length after insertion must not exceed the allocated size.
    \pre The source C \a string must not be null.
  */
  constexpr inline FixString<allocatedSize> & insert(std::size_t index, const char * string) noexcept;

  /*!
    \brief Inserts a \a character repeated the given \a count times at the specified \a index.

    This method inserts a \a character repeated the given \a count of times at the specified position. The insertion
    shifts existing characters to the right to make room for the new content.

    \param index     The position where the characters should be inserted.
    \param character The character to insert.
    \param count     The number of times to repeat the \a character (default: 1).

    \return A reference to this string after insertion.

    \pre The \a index must be within the bounds of the current string.
    \pre The combined length after insertion must not exceed the allocated size.
  */
  constexpr inline FixString<allocatedSize> & insert(std::size_t index, char character, std::size_t count = 1) noexcept;

  /*!
    \brief Erases characters from the string starting at the specified \a offset.

    This method removes characters from the string starting at the specified \a offset. The method can remove a specific
    number of characters or all characters from the \a offset to the end of the string. The remaining characters are
    shifted left to fill the gap, and the string size is updated accordingly.

    \param offset The starting position for erasing characters.
    \param count  The number of characters to erase. If count is \ref npos or equal to the remaining length, then all
                  characters from \a offset to the end are removed.

    \return A reference to this string after erasing.

    \pre The \a offset must be within the bounds of the current string.
    \pre The sum of \a offset and \a count must be less than or equal to the current string size.

    \note If \a count is npos, all characters from \a offset to the end are removed.
    \note If \a count is 0, the operation is a no-op.
    \note Erasing from an empty string has no effect.
  */
  constexpr inline FixString<allocatedSize> & erase(std::size_t offset, std::size_t count = npos) noexcept;

  /*!
    \brief Appends a single \a character to the end of the string.

    This method appends a single \a character to the end of the current string, increasing its size by one.

    \param character The character to append.

    \pre The combined length after append must not exceed the allocated size.

    \note This method is equivalent to the addition assignment operator but more explicit in intent.

    \see operator+=(char)
  */
  constexpr inline void push_back(char character) noexcept;

  /*!
    \brief Removes the last character from the string.

    This method removes the last character from the current string, decreasing its size by one.

    \pre The string must not be empty.

    \post The string size is decreased by one.

    \note If the string is empty, the behavior is undefined.
  */
  constexpr inline void pop_back() noexcept;

  /*!
    \brief Appends other \a string to the end of this string.

    This method appends the contents of another \a string to the end of this string.

    \param string The source string to append content from.

    \return A reference to this string after appending.

    \pre The combined length after appending must not exceed the allocated size.

    \post The string is extended with the appended content.

    \note This method is equivalent to the addition assignment operator.

    \see operator+=(const FixString<allocatedSize> &)
  */
  constexpr inline FixString<allocatedSize> & append(const FixString<allocatedSize> & string) noexcept;

  /*!
    \brief Appends a StringLike object to the end of this string.

    This method appends the contents of a StringLike object to the end of this string.

    \tparam stringType The type of the source string. Must satisfy the StringLike concept.

    \param string The source StringLike object to append content from.

    \return A reference to this string after appending.

    \pre The combined length after appending must not exceed the allocated size.

    \post The string is extended with the appended content.

    \note This method is equivalent to the addition assignment operator.

    \see operator+=(const stringType &)
  */
  template <StringLike stringType>
  constexpr inline FixString<allocatedSize> & append(const stringType & string) noexcept;

  /*!
    \brief Appends the C \a string to the end of this string.

    This method appends the contents of the C \a string to the end of this string.

    \param string The source C string to append content from.

    \return A reference to this string after appending.

    \pre The combined length after appending must not exceed the allocated size.
    \pre The source C \a string must not be null.

    \post The string is extended with the appended content.

    \note This method is equivalent to the addition assignment operator.

    \see operator+=(const char *)
  */
  constexpr inline FixString<allocatedSize> & append(const char * string) noexcept;

  /*!
    \brief Appends a \a character repeated the given \a count times to the end of this string.

    This method appends a \a character repeated the given \a count of times to the end of this string.

    \param character The character to append.
    \param count     The number of times to repeat the \a character (default: 1).

    \return A reference to this string after appending.

    \pre The combined length after appending must not exceed the allocated size.

    \post The string is extended with the appended content.
  */
  constexpr inline FixString<allocatedSize> & append(char character, std::size_t count = 1) noexcept;

  /*!
    \brief Appends other \a string to the end of this string.

    This operator appends the contents of another \a string to the end of this string.

    \param string The source string to append content from.

    \return A reference to this string after appending.

    \pre The combined length after appending must not exceed the allocated size.

    \post The string is extended with the appended content.
  */
  constexpr inline FixString<allocatedSize> & operator+=(const FixString<allocatedSize> & string) noexcept;

  /*!
    \brief Appends a StringLike object to the end of this string.

    This operator appends the contents of a StringLike object to the end of this string.

    \tparam stringType The type of the source string. Must satisfy the StringLike concept.

    \param string The source StringLike object to append content from.

    \return A reference to this string after appending.

    \pre The combined length after appending must not exceed the allocated size.

    \post The string is extended with the appended content.
  */
  template <StringLike stringType>
  constexpr inline FixString<allocatedSize> & operator+=(const stringType & string) noexcept;

  /*!
    \brief Appends the C \a string to the end of this string.

    This operator appends the contents of the C \a string to the end of this string.

    \param string The source C string to append content from.

    \return A reference to this string after appending.

    \pre The combined length after appending must not exceed the allocated size.
    \pre The source C \a string must not be null.

    \post The string is extended with the appended content.
  */
  constexpr inline FixString<allocatedSize> & operator+=(const char * string) noexcept;

  /*!
    \brief Appends a \a character to the end of this string.

    This operator appends a \a character to the end of this string.

    \param character The character to append.

    \return A reference to this string after appending.

    \pre The combined length after appending must not exceed the allocated size.

    \post The string is extended with the appended content.
  */
  constexpr inline FixString<allocatedSize> & operator+=(char character) noexcept;

  /*!
    \brief Replaces a portion of the string with another \a string.

    This method replaces a specified range of characters in the current string with the contents of another \a string.
    The replacement operation shifts existing characters as needed to accommodate the new content.

    \param pos    The starting position for the replacement.
    \param count  The number of characters to replace.
    \param string The source string to replace content from.

    \return A reference to this string after replacement.

    \pre The \a pos must be within the bounds of the current string.
    \pre The replacement range ( \a pos + \a count ) must be within the string bounds.
    \pre The resulting string size must not exceed the allocated capacity.
  */
  constexpr inline FixString<allocatedSize> & replace(std::size_t pos, std::size_t count,
                                                      const FixString<allocatedSize> & string) noexcept;

  /*!
    \brief Replaces a portion of the string with a StringLike object.

    This method replaces a specified range of characters in the current string with the contents of a StringLike object.
    The replacement operation shifts existing characters as needed to accommodate the new content.

    \tparam stringType The type of the source string. Must satisfy the StringLike concept.

    \param pos    The starting position for the replacement.
    \param count  The number of characters to replace.
    \param string The source StringLike object to replace content from.

    \return A reference to this string after replacement.

    \pre The \a pos must be within the bounds of the current string.
    \pre The replacement range ( \a pos + \a count ) must be within the string bounds.
    \pre The resulting string size must not exceed the allocated capacity.
  */
  template <StringLike stringType>
  constexpr inline FixString<allocatedSize> & replace(std::size_t pos, std::size_t count,
                                                      const stringType & string) noexcept;

  /*!
    \brief Replaces a portion of the string with the C \a string.

    This method replaces a specified range of characters in the current string with the contents of the C \a string. The
    replacement operation shifts existing characters as needed to accommodate the new content.

    \param pos    The starting position for the replacement.
    \param count  The number of characters to replace.
    \param string The source C string to replace content from.

    \return A reference to this string after replacement.

    \pre The \a pos must be within the bounds of the current string.
    \pre The replacement range ( \a pos + \a count ) must be within the string bounds.
    \pre The source C \a string must not be null.
    \pre The resulting string size must not exceed the allocated capacity.
  */
  constexpr inline FixString<allocatedSize> & replace(std::size_t pos, std::size_t count, const char * string) noexcept;

  /*!
    \brief Replaces a portion of the string with a \a character repeated the given \a count of times.

    This method replaces a specified range of characters in the current string with a \a character repeated the given \a
    count of times. The replacement operation shifts existing characters as needed to accommodate the new content.

    \param pos             The starting position for the replacement.
    \param count           The number of characters to replace.
    \param character       The character to replace.
    \param charactersCount The number of times to repeat the \a character (default: 1).

    \return A reference to this string after replacement.

    \pre The \a pos must be within the bounds of the current string.
    \pre The replacement range ( \a pos + \a count ) must be within the string bounds.
    \pre The resulting string size must not exceed the allocated capacity.
  */
  constexpr inline FixString<allocatedSize> & replace(std::size_t pos, std::size_t count, char character,
                                                      std::size_t charactersCount = 1) noexcept;

  /*!
    \brief Copies characters from the string to a destination buffer.

    This method copies up to \a count characters from the string starting at position \a pos to the destination buffer.
    The method returns the actual number of characters copied, which may be less than \a count if the end of the string
    is reached.

    \param dest  The destination buffer to copy characters to.
    \param count The maximum number of characters to copy.
    \param pos   The starting position in the string to copy from (default: 0).

    \return The actual number of characters copied.

    \pre The \a pos must be within the bounds of the current string.
    \pre The \a dest pointer must not be null.
    \pre The \a dest buffer must not overlap this string's internal storage.
    \pre The \a dest buffer must have sufficient space for the copied characters.

    \note If \a count is \ref npos or exceeds the remaining characters from \a pos, all remaining characters are copied.
    \note The destination buffer is not null-terminated by this method.
  */
  constexpr inline std::size_t copy(char * dest, std::size_t count, std::size_t pos = 0) const noexcept;

  /*!
    \brief Concatenates two FixString objects and returns the result.

    This operator creates a new FixString object by concatenating the current FixString object with another provided
    FixString object. The resulting FixString will contain the contents of both strings, and its size will be the sum of
    the sizes of the two operands.

    \param string The FixString object to concatenate with the current FixString object.

    \return A new FixString object representing the concatenation of the current FixString and the provided FixString
            object.

    \pre The sum of the sizes of the two FixString objects must be less than the allocated size.
    \pre The allocated size of both FixString objects must be greater than zero.

    \post A new FixString object is created with the contents of both FixString objects.
  */
  constexpr inline FixString<allocatedSize> operator+(const FixString<allocatedSize> & string) const noexcept;

  /*!
    \brief Concatenates two FixString objects and returns the result.

    This operator creates a new FixString object by concatenating the current FixString object with another provided
    FixString object. The resulting FixString will contain the contents of both strings, and its size will be the sum of
    the sizes of the two operands.

    \param string The FixString object to concatenate with the current FixString object.

    \return A new FixString object representing the concatenation of the current FixString and the provided FixString
            object.

    \pre The sum of the sizes of the two FixString objects must be less than the allocated size.
    \pre The allocated size of both FixString objects must be greater than zero.

    \post A new FixString object is created with the contents of both FixString objects.
  */
  template <std::size_t allocatedSize2>
  constexpr inline FixString<allocatedSize> operator+(const FixString<allocatedSize2> & string) const noexcept;

  /*!
    \brief Concatenates two strings and returns the result.

    This operator creates a new FixString object by concatenating the current FixString object with a C string. The
    resulting FixString will contain the contents of both strings, and its size will be the sum of the sizes of the two
    operands.

    \param string The C string to concatenate with the current FixString object.

    \return A new FixString object representing the concatenation of the current FixString and the provided C string.

    \pre The sum of the sizes of the two strings must be less than the allocated size.
    \pre The allocated size must be greater than zero.

    \post A new FixString object is created with the contents of both strings.
  */
  constexpr inline FixString<allocatedSize> operator+(const char * string) const noexcept;

  /*!
    \brief Concatenates this FixString object with a character.

    This operator adds a character to the end of the FixString object and returns the new FixString object.

    \param symbol The character to add to the end of the FixString object.

    \pre The allocated size of the FixString object must be greater than zero.
    \pre The size of the FixString object plus one must be less than the allocated size.

    \post A new FixString object is created with the contents of the original FixString object plus the given character.
  */
  constexpr inline FixString<allocatedSize> operator+(char symbol) const noexcept;

  static const std::size_t npos = SIZE_MAX;

private:
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");

  char _data[allocatedSize];
  std::size_t _size;
};

} // namespace toygine

#endif // INCLUDE_CORE_FIX_STRING_HPP_
