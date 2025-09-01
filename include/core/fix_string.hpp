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
    \brief Default constructor for the string.

    This is the default constructor for the string.

    \post The string is empty and ready for use.

    \post The string has a size of 0 and a null character in the data array.
  */
  constexpr inline FixString() noexcept;

  /*!
    \brief Destructor for the string.

    This destructor cleans up the string.

    \note: Since the FixString does not manage dynamic memory, no special cleanup is required.
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

    This constructor initializes a string by copying the content from StringLike object.

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

    \param string The C string to copy content from.

    \pre The source \a string must not be null.

    \post The new string is created with the contents of the source C \a string.
  */
  constexpr explicit inline FixString(const char * string) noexcept;

  /*!
    \brief Constructs a string of the given \a count of symbol.

    This constructor initializes a string with a character repeated the given number of times.

    \param symbol The character to repeat.
    \param count  The number of times to repeat the character.

    \pre The \a count must not exceed the allocated size.

    \post The new string is created with the contents of the character repeated the given number of times.
    \post The string size equals the \a count.

    \note This is useful for creating strings with repeated patterns or filling strings with specific characters.
  */
  constexpr inline explicit FixString(char symbol, std::size_t count = 1) noexcept;

  /*!
    \brief Copy assigns other \a string to this string.

    This operator assigns the contents of another \a string to this string. The assignment operation performs a deep
    copy of the string data and updates the size accordingly.

    \param string The source string to copy content from.

    \return A reference to this string after assignment.

    \pre The source string must be valid and properly initialized.

    \post This string contains the same content as the source \a string.
    \post The size of this string equals the size of the source \a string.

    \note Self-assignment is handled correctly and safely.
  */
  constexpr inline FixString<allocatedSize> & operator=(const FixString<allocatedSize> & string) noexcept;

  /*!
    \brief Copy assigns a StringLike object to this string.

    This operator assigns the content from StringLike object to this string. The assignment operation performs a deep
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

    \pre The source C string must not be null.
    \pre The source C string size must not exceed the allocated capacity.

    \post This string contains the same content as the source C string.
    \post The size of this string equals the size of the source C string.

    \note Self-assignment is handled correctly and safely.
  */
  constexpr inline FixString<allocatedSize> & operator=(const char * string) noexcept;

  /*!
    \brief Character assignment to this string.

    This operator assigns a single character to this string, creating a string of length 1. This is useful for
    resetting a string to contain only a single character.

    \param symbol The character to assign.

    \return A reference to this string after assignment.

    \post This string contains only one specified character.
  */
  constexpr inline FixString<allocatedSize> & operator=(char symbol) noexcept;

  /*!
    \brief Copy assigns other \a string to this string.

    This method assigns the contents of another \a string to this string. The assignment operation performs a deep
    copy of the string data and updates the size accordingly.

    \param string The source string to copy content from.

    \return A reference to this string after assignment.

    \pre The source string must be valid and properly initialized.

    \post This string contains the same content as the source \a string.
    \post The size of this string equals the size of the source \a string.

    \note Self-assignment is handled correctly and safely.
    \note This method is equivalent to the copy assignment operator.

    \see operator=(const FixString<allocatedSize> & string)
  */
  constexpr inline FixString<allocatedSize> & assign(const FixString<allocatedSize> & string) noexcept;

  /*!
    \brief Copy assigns a StringLike object to this string.

    This method assigns the content from StringLike object to this string. The assignment operation performs a deep copy
    of the string data and updates the size accordingly.

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

    \pre The source C string must not be null.
    \pre The source C string size must not exceed the allocated capacity.

    \post This string contains the same content as the source C string.
    \post The size of this string equals the size of the source C string.

    \note The method automatically handles string length calculation.
    \note This method is equivalent to the C string assignment operator.

    \note Self-assignment is handled correctly and safely.
    \note This method is equivalent to the copy assignment operator.

    \see operator=(const char * string)
  */
  constexpr inline FixString<allocatedSize> & assign(const char * string) noexcept;

  /*!
    \brief Character fill assign method for the string class.

    This method assigns a character repeated the specified number of times to this string. This is useful for
    creating strings with repeated patterns or filling strings with specific characters.

    \param symbol The character to repeat.
    \param count  The number of times to repeat the character.

    \return A reference to this string after assignment.

    \pre The count must not exceed this object's allocated size.

    \post This string object contains the specified character repeated count times.
    \post The string size equals the count parameter.
  */
  constexpr inline FixString<allocatedSize> & assign(char symbol, std::size_t count = 1) noexcept;

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
    \note Use non-const version for allows modification.
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
    \note Use non-const version for allows modification.
  */
  constexpr inline const char & operator[](std::size_t offset) const noexcept;

  /*!
    \brief Returns a pointer to the data of the string.

    This method returns a pointer to the internal character array that stores the string data. The returned pointer
    provides direct access to the string contents and can be used for low-level operations.

    \return A pointer to the internal character array.

    \note The returned pointer points to a null-terminated string.
    \note The returned pointer allows modification of the string contents.
    \note Use const version for read-only access.
  */
  constexpr inline char * data() noexcept;

  /*!
    \brief Returns a constant pointer to the data of the string.

    This method returns a constant pointer to the internal character array that stores the string data. The returned
    pointer provides read-only access to the string contents and can be used for low-level operations.

    \return A constant pointer to the internal character array.

    \note The returned pointer points to a null-terminated string.
    \note The returned pointer is read-only and cannot modify the string contents.
    \note Use non-const version for allows modification.
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

    This method checks if the string is empty, i.e. its size is zero. An empty string contains no characters
    and has a length of 0.

    \return True if the string is empty, false otherwise.

    \note An empty string has size() == 0.
    \note An empty string still contains a null terminator.
    \note This method is equivalent to the expression: ```size() == 0```.
  */
  constexpr inline bool empty() const noexcept;

  /*!
    \brief Returns the size of the string.

    This method returns the current number of characters in the string, excluding the terminating null character. The
    size represents the actual length of the string content.

    \return The number of the characters in the string, excluding the terminating null character.

    \note The size is always less than allocatedSize.
    \note The size represents the actual string content length.
    \note This method is equivalent to length() method.
  */
  constexpr inline std::size_t size() const noexcept;

  /*!
    \brief Returns the size of the Unicode string in UTF-8 encoding.

    This method returns the number of Unicode characters in the string, excluding the terminating null character.
    For ASCII strings, this value equals the size() method. For UTF-8 encoded strings, this method counts the
    actual Unicode characters rather than bytes.

    \return The number of Unicode characters in the string, excluding the terminating null character.

    \note For ASCII strings, utf8Size() equals size().
    \note For UTF-8 strings, utf8Size() may be less than size().
  */
  constexpr inline std::size_t utf8Size() const noexcept;

  /*!
    \brief Returns the size of the string.

    This method returns the current number of characters in the string, excluding the terminating null character. The
    length represents the actual length of the string content.

    \return The number of the characters in the string, excluding the terminating null character.

    \note The length is always less than allocatedSize.
    \note The length represents the actual string content length.
    \note This method is equivalent to size() method.

    \see size()
  */
  constexpr inline std::size_t length() const noexcept;

  /*!
    \brief Returns the maximum size of the string.

    This method returns the maximum possible size of the string, which is determined by the template parameter
    allocatedSize. The maximum size represents the capacity of the string buffer.

    \return The maximum number of the characters in the string, excluding the terminating null character.

    \note The maximum size is determined at compile time.
    \note This method is equivalent to capacity() method.
  */
  constexpr inline std::size_t max_size() const noexcept;

  /*!
    \brief Returns the capacity of the string.

    This method returns the maximum number of characters that can be stored in the string, which is determined by
    the template parameter allocatedSize. The capacity represents the size of the allocated buffer.

    \return The maximum size of the string in characters, excluding the terminating null character.

    \note The capacity is always greater than 0.
    \note The capacity is determined at compile time.
    \note This method is equivalent to max_size() method.
    \note The capacity cannot be changed after object creation.
  */
  constexpr inline std::size_t capacity() const noexcept;

  /*!
    \brief Clears the contents of the FixString object.

    This method resets the size of the string object to zero and effectively clear the string.

    \post The FixString object is cleared, and its size is set to zero.
  */
  constexpr inline void clear() noexcept;

  /*!
    \brief Inserts a string at the specified index.

    This method inserts the contents of another FixString object at the specified position. The insertion shifts
    existing characters to the right to make room for the new content.

    \param index  The position where the string should be inserted.
    \param string The FixString object to insert.

    \return A reference to this FixString object after insertion.

    \pre The index must be within the bounds of the current string.
    \pre The combined length after insertion must not exceed the allocated size.

    \post The string is modified with the inserted content at the specified position.
  */
  constexpr inline FixString<allocatedSize> & insert(std::size_t index,
                                                     const FixString<allocatedSize> & string) noexcept;

  /*!
    \brief Inserts a string object at the specified index.

    This method inserts the contents of another string object at the specified position. The insertion shifts existing
    characters to the right to make room for the new content.

    \tparam stringType The type of the source string. Must satisfy the StringLike concept.

    \param index  The position where the string should be inserted.
    \param string The string object to insert.

    \return A reference to this FixString object after insertion.

    \pre The index must be within the bounds of the current string.
    \pre The combined length after insertion must not exceed the allocated size.

    \post The string is modified with the inserted content at the specified position.
  */
  template <StringLike stringType>
  constexpr inline FixString<allocatedSize> & insert(std::size_t index, const stringType & string) noexcept;

  /*!
    \brief Inserts a C string at the specified index.

    This method inserts the contents of a C string at the specified position. The insertion shifts existing characters
    to the right to make room for the new content.

    \param index  The position where the string should be inserted.
    \param string The C string to insert.

    \return A reference to this FixString object after insertion.

    \pre The index must be within the bounds of the current string.
    \pre The combined length after insertion must not exceed the allocated size.
    \pre The source string must not be null.

    \post The string is modified with the inserted content at the specified position.
  */
  constexpr inline FixString<allocatedSize> & insert(std::size_t index, const char * string) noexcept;

  /*!
    \brief Inserts a character repeated count times at the specified index.

    This method inserts a character repeated the specified number of times at the specified position. The insertion
    shifts existing characters to the right to make room for the new content.

    \param index  The position where the character should be inserted.
    \param symbol The character to insert.
    \param count  The number of times to repeat the character.

    \return A reference to this FixString object after insertion.

    \pre The index must be within the bounds of the current string.
    \pre The combined length after insertion must not exceed the allocated size.

    \post The string is modified with the inserted characters at the specified position.
  */
  constexpr inline FixString<allocatedSize> & insert(std::size_t index, char symbol, std::size_t count = 1) noexcept;

  /*!
    \brief Erases characters from the string starting at the specified offset.

    This method removes characters from the string starting at the specified offset. The method can remove a specific
    number of characters or all characters from the offset to the end of the string. The remaining characters are
    shifted left to fill the gap, and the string size is updated accordingly.

    \param offset The starting position for erasing characters.
    \param count  The number of characters to erase. If count is `npos` or equal to the remaining length, all characters
                  from offset to the end are removed.

    \return A reference to this FixString object after erasing.

    \pre The offset must be within the bounds of the current string.
    \pre The sum of offset and count must be less than or equal to the current string size.

    \post Characters are removed from the specified position.
    \post The string size is reduced by the number of erased characters.

    \note If count is npos, all characters from offset to the end are removed.
    \note If count is 0, the operation is a no-op.
    \note Erasing from an empty string has no effect.
  */
  constexpr inline FixString<allocatedSize> & erase(std::size_t offset, std::size_t count = npos) noexcept;

  /*!
    \brief Appends a single character to the end of the string.

    This method appends a single character to the end of the current string, increasing its size by one.

    \param symbol The character to append.

    \pre The current string size plus one must be strictly less than the allocated size (i.e., size() < capacity()).

    \post The string size is increased by one.
    \post The new character is added at the end of the string.
    \post The string remains null-terminated.

    \note This method is equivalent to operator+=(char) but more explicit in intent.
  */
  constexpr inline void push_back(char symbol) noexcept;

  /*!
    \brief Removes the last character from the string.

    This method removes the last character from the current string, decreasing its size by one.

    \pre The string must not be empty.

    \post The string size is decreased by one.
    \post The last character is removed from the string.
    \post The string remains null-terminated.

    \note If the string is empty, the behavior is undefined.
  */
  constexpr inline void pop_back() noexcept;

  /*!
    \brief Appends a string to the end of this string.

    This method appends the contents of another FixString object to the end of this string.

    \param string The FixString object to append.

    \return A reference to this FixString object after appending.

    \pre The combined length after appending must not exceed the allocated size.

    \post The string is extended with the appended content.
  */
  constexpr inline FixString<allocatedSize> & append(const FixString<allocatedSize> & string) noexcept;

  /*!
    \brief Appends a string object to the end of this string.

    This method appends the contents of a string object to the end of this string.

    \tparam stringType The type of the source string. Must satisfy the StringLike concept.

    \param string The string object to append.

    \return A reference to this FixString object after appending.

    \pre The combined length after appending must not exceed the allocated size.

    \post The string is extended with the appended content.
  */
  template <StringLike stringType>
  constexpr inline FixString<allocatedSize> & append(const stringType & string) noexcept;

  /*!
    \brief Appends a C string to the end of this string.

    This method appends the contents of a C string to the end of this string.

    \param string The C string to append.

    \return A reference to this FixString object after appending.

    \pre The combined length after appending must not exceed the allocated size.

    \post The string is extended with the appended content.
  */
  constexpr inline FixString<allocatedSize> & append(const char * string) noexcept;

  /*!
    \brief Appends a character repeated the given number of times to the end of this string.

    This method appends the contents of a character repeated the given number of times to the end of this string.

    \param symbol The character to append.
    \param count  The number of times to repeat the character.

    \return A reference to this FixString object after appending.

    \pre The combined length after appending must not exceed the allocated size.

    \post The string is extended with the appended content.
  */
  constexpr inline FixString<allocatedSize> & append(char symbol, std::size_t count = 1) noexcept;

  /*!
    \brief Appends a string to this string.

    This operator appends the contents of another FixString object to the end of this string.

    \param string The FixString object to append.

    \return A reference to this FixString object after appending.

    \pre The combined length after appending must not exceed the allocated size.

    \post The string is extended with the appended content.
  */
  constexpr inline FixString<allocatedSize> & operator+=(const FixString<allocatedSize> & string) noexcept;

  /*!
    \brief Appends a string object to the end of this string.

    This operator appends the contents of a string object to the end of this string.

    \tparam stringType The type of the source string. Must satisfy the StringLike concept.

    \param string The string object to append.

    \return A reference to this FixString object after appending.

    \pre The combined length after appending must not exceed the allocated size.

    \post The string is extended with the appended content.
  */
  template <StringLike stringType>
  constexpr inline FixString<allocatedSize> & operator+=(const stringType & string) noexcept;

  /*!
    \brief Appends a C string to the end of this string.

    This operator appends the contents of a C string to the end of this string.

    \param string The C string to append.

    \return A reference to this FixString object after appending.

    \pre The combined length after appending must not exceed the allocated size.

    \post The string is extended with the appended content.
  */
  constexpr inline FixString<allocatedSize> & operator+=(const char * string) noexcept;

  /*!
    \brief Appends a character to the end of this string.

    This operator appends the contents of a character to the end of this string.

    \param symbol The character to append.

    \return A reference to this FixString object after appending.

    \pre The combined length after appending must not exceed the allocated size.

    \post The string is extended with the appended content.
  */
  constexpr inline FixString<allocatedSize> & operator+=(char symbol) noexcept;

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
