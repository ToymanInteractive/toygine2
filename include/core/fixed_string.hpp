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
  \file   fixed_string.hpp
  \brief  Template string class with fixed-size character buffer.
*/

#ifndef INCLUDE_CORE_FIXED_STRING_HPP_
#define INCLUDE_CORE_FIXED_STRING_HPP_

#include "string_like.hpp"

namespace toy {

template <size_t allocatedSize>
struct FixedStringStorage {
  static_assert(allocatedSize > 0, "FixedStringStorage capacity must be greater than zero.");

  /// Internal character buffer storing the storage data
  char buffer[allocatedSize] = {'\0'};

  /// Current number of characters in the storage (excluding null terminator)
  size_t size = 0;
};

/*!
  \class FixedString
  \ingroup String
  \brief Template string class with fixed-size character buffer.

  FixedString is a lightweight, high-performance string class that uses a fixed-size character buffer allocated on the
  stack. It provides a std::string-like interface while avoiding dynamic memory allocation, making it suitable for
  embedded systems, real-time applications, and performance-critical code where memory allocation overhead must be
  minimized.

  \tparam allocatedSize The size of the internal character buffer, including space for the null terminator.
                        Must be greater than zero.

  \section features Key Features

  - ⚙️ **Zero Dynamic Allocation**: All memory is allocated on the stack at compile time
  - 🔧 **ConstExpr Support**: Most operations can be evaluated at compile time
  - 🛡️ **Exception Safety**: All operations are noexcept
  - 🔗 **STL Compatibility**: Provides std::string-like interface
  - 🌍 **UTF-8 Support**: Built-in UTF-8 character counting and manipulation
  - 🧬 **Type Safety**: Uses C++20 concepts for type safety

  \section usage Usage Example

  \code
  #include "fixed_string.hpp"

  // Create a string with 32-character capacity
  toy::FixedString<32> str("Hello, World!");

  // Append more content
  str += " This is a test.";

  // Use in constexpr context
  constexpr auto greeting = toy::FixedString<16>("Hello");
  constexpr auto world = toy::FixedString<16>("World");
  constexpr auto combined = greeting + " " + world;
  \endcode

  \section performance Performance Characteristics

  - ⚙️ **Construction**: O(n) where n is the length of the source string
  - 📝 **Assignment**: O(n) where n is the length of the source string
  - 🔗 **Concatenation**: O(n) where n is the length of the appended string
  - 🔍 **Search Operations**: O(n*m) where n is the string length and m is the pattern length
  - ⚖️ **Comparison Operations**: O(n) where n is the string length
  - 💾 **Memory Usage**: Fixed at compile time, no heap allocation
  - ⚡ **Cache Performance**: Excellent due to stack allocation and contiguous memory layout
  - 📋 **Copy Performance**: Fast due to memcpy/memmove optimizations
  - 🎯 **String Operations**: Optimized for common cases

  \section safety Safety Guarantees

  - 🛡️ **Contracts & Debug Checks**: Bounds/capacity are asserted in debug; in production, violating preconditions is UB
  - 🔒 **Null Pointer Safety**: All C-string operations validate pointers via assertions in debug
  - 📐 **Type Safety**: Template parameters and concepts prevent invalid usage
  - ⚠️ **Exception Safety**: All operations are noexcept, no exceptions thrown

  \section compatibility Compatibility

  - 🆕 **C++23**: Requires C++23 or later for full functionality
  - 🔗 **STL Integration**: Compatible with STL algorithms and containers
  - 🌐 **Cross-Platform**: Works on all platforms supported by the compiler
  - 🔧 **Embedded Systems**: Suitable for resource-constrained environments

  \note The internal buffer size is allocatedSize, but the maximum string length is allocatedSize - 1 (null terminator).

  \sa std::string
  \sa toy::StringLike
  \sa toy::CStringView
*/
template <size_t allocatedSize>
class FixedString {
  static_assert(allocatedSize > 0, "FixedString capacity must be greater than zero.");

public:
  using value_type = char; //!< Type of characters stored in the string.

  using size_type = size_t; //!< Type used for string size and capacity.
  using difference_type = std::ptrdiff_t; //!< Type used for pointer differences.

  using reference = value_type &; //!< Reference to string character.
  using const_reference = const value_type &; //!< Const reference to string character.

  using pointer = value_type *; //!< Pointer to string character.
  using const_pointer = const value_type *; //!< Const pointer to string character.

  using iterator = value_type *; //!< Iterator type for string characters.
  using const_iterator = const value_type *; //!< Const iterator type for string characters.

  //! Reverse iterator type for string characters.
  using reverse_iterator = std::reverse_iterator<iterator>;
  //! Const reverse iterator type for string characters.
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  /*!
    \brief Default constructor.

    \post The string is empty and ready for use.
  */
  constexpr FixedString() noexcept = default;

  /*!
    \brief Destructor for the fixed string.
  */
  constexpr ~FixedString() noexcept = default;

  /*!
    \brief Constructs a string of the given \a count of \a character.

    This constructor initializes a string with a \a character repeated the given \a count of times.

    \param count     The number of times to repeat the \a character.
    \param character The character to repeat.

    \pre The \a count must not exceed the allocated size.

    \post The new string is created with the contents of the \a character repeated the given \a count of times.
    \post The string size equals the \a count.

    \note This is useful for creating strings with repeated patterns or filling with specific characters.
  */
  constexpr FixedString(size_type count, char character) noexcept;

  /*!
    \brief Constructs a string initialized with a \ref toy::StringLike object.

    This constructor initializes a string by copying the content from a \ref toy::StringLike object.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string The source \ref toy::StringLike object to copy content from.

    \pre The source \ref toy::StringLike object must be valid and properly initialized.
    \pre The source \ref toy::StringLike object size must not exceed the allocated capacity.

    \post The new string is created with the contents of the source \ref toy::StringLike object.
  */
  template <StringLike stringType>
  constexpr explicit FixedString(const stringType & string) noexcept;

  /*!
    \brief Constructs a string initialized with the C \a string.

    This constructor initializes a string by copying the content from the C \a string. The constructor automatically
    determines the length of the source string and copies the content up to the allocated capacity.

    \param string The source C string to copy content from.

    \pre The source \a string must not be null.

    \post The new string is created with the contents of the source C \a string.
  */
  constexpr explicit FixedString(const char * string) noexcept;

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
  constexpr FixedString<allocatedSize> & operator=(const FixedString<allocatedSize> & string) noexcept;

  /*!
    \brief Copy assigns a \ref toy::StringLike object to this string.

    This operator assigns the content from a \ref toy::StringLike object to this string. The assignment operation
    performs a deep copy of the string data and updates the size accordingly.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string The source \ref toy::StringLike object to copy content from.

    \return A reference to this string after assignment.

    \pre The source \ref toy::StringLike object must be valid and properly initialized.
    \pre The source \ref toy::StringLike object size must not exceed the allocated capacity.

    \post This string contains the same content as the source \ref toy::StringLike object.
    \post The size of this string equals the size of the source \ref toy::StringLike object.

    \note Self-assignment is handled correctly and safely.

    \sa assign(const stringType &)
  */
  template <StringLike stringType>
  constexpr FixedString<allocatedSize> & operator=(const stringType & string) noexcept;

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

    \sa assign(const char *)
  */
  constexpr FixedString<allocatedSize> & operator=(const char * string) noexcept;

  /*!
    \brief Character assignment to this string.

    This operator assigns a single \a character to this string, creating a string of length 1.

    \param character The character to assign.

    \return A reference to this string after assignment.

    \post This string contains only one specified \a character.
    \post The string size equals \c 1.

    \note This is useful for resetting a string to contain only a single character.
  */
  constexpr FixedString<allocatedSize> & operator=(char character) noexcept;

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

    \sa operator=(const FixedString<allocatedSize> & string)
  */
  constexpr FixedString<allocatedSize> & assign(const FixedString<allocatedSize> & string) noexcept;

  /*!
    \brief Copy assigns a \ref toy::StringLike object to this string.

    This method assigns the content from a \ref toy::StringLike object to this string. The assignment operation performs
    a deep copy of the string data and updates the size accordingly.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string The source \ref toy::StringLike object to copy content from.

    \return A reference to this string after assignment.

    \pre The source \ref toy::StringLike object must be valid and properly initialized.
    \pre The source \ref toy::StringLike object size must not exceed the allocated capacity.

    \post This string contains the same content as the source \ref toy::StringLike object.
    \post The size of this string equals the size of the source \ref toy::StringLike object.

    \note Self-assignment is handled correctly and safely.
    \note This method is equivalent to the copy assignment operator.

    \sa operator=(const stringType & string)
  */
  template <StringLike stringType>
  constexpr FixedString<allocatedSize> & assign(const stringType & string) noexcept;

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

    \sa operator=(const char * string)
  */
  constexpr FixedString<allocatedSize> & assign(const char * string) noexcept;

  /*!
    \brief Character fill assign method for the string.

    This method assigns a \a character repeated the given \a count of times to this string.

    \param character The character to repeat.
    \param count     The number of times to repeat the \a character (default: \c 1).

    \return A reference to this string after assignment.

    \pre The \a count must not exceed this object's allocated size.

    \post This string object contains the specified \a character repeated \a count of times.
    \post The string size equals the \a count parameter.

    \note This is useful for creating strings with repeated patterns or filling with specific characters.
  */
  constexpr FixedString<allocatedSize> & assign(char character, size_t count = 1) noexcept;

  /*!
    \brief Access a character in the string at a given \a offset.

    This method provides direct access to a character at the specified \a offset within the string.

    \param offset The offset of the character to access in the string.

    \return A reference to the character at the specified \a offset.

    \pre The \a offset must be less than the current string size.

    \note The returned reference allows modification of the character.
    \note Use const version for read-only access.
    \note This method is equivalent to the subscript operator.

    \sa operator[](size_t offset)
  */
  [[nodiscard]] constexpr char & at(size_t offset) noexcept;

  /*!
    \brief Access a character in the string at a given \a offset.

    This method provides read-only access to a character at the specified \a offset within the string.

    \param offset The offset of the character to access in the string.

    \return A const reference to the character at the specified \a offset.

    \pre The \a offset must be less than the current string size.

    \note The returned reference is read-only and cannot modify the character.
    \note Use the non-const overload to allow modification.
    \note This method is equivalent to the subscript operator.

    \sa operator[](size_t offset) const
  */
  [[nodiscard]] constexpr const char & at(size_t offset) const noexcept;

  /*!
    \brief Access a character in the string at a given \a offset.

    This operator provides direct access to a character at the specified \a offset within the string.

    \param offset The offset of the character to access in the string.

    \return A reference to the character at the specified \a offset.

    \pre The \a offset must be less than the current string size.

    \note The returned reference allows modification of the character.
    \note Use const version for read-only access.

    \sa operator[](size_t offset) const
    \sa at()
  */
  [[nodiscard]] constexpr char & operator[](size_t offset) noexcept;

  /*!
    \brief Access a character in the string at a given \a offset.

    This operator provides read-only access to a character at the specified \a offset within the string.

    \param offset The offset of the character to access in the string.

    \return A const reference to the character at the specified \a offset.

    \pre The \a offset must be less than the current string size.

    \note The returned reference is read-only and cannot modify the character.
    \note Use the non-const overload to allow modification.
  */
  [[nodiscard]] constexpr const char & operator[](size_t offset) const noexcept;

  /*!
    \brief Returns a reference to the first character of the string.

    This method provides direct access to the first character of the string.

    \return A reference to the first character of the string.

    \pre The string must not be empty.

    \note The returned reference allows modification of the character.
    \note Use const version for read-only access.

    \sa front() const
  */
  [[nodiscard]] constexpr char & front() noexcept;

  /*!
    \brief Returns a const reference to the first character of the string.

    This method provides read-only access to the first character of the string.

    \return A const reference to the first character of the string.

    \pre The string must not be empty.

    \note The returned reference is read-only and cannot modify the character.
    \note Use the non-const overload to allow modification.

    \sa front() noexcept
  */
  [[nodiscard]] constexpr const char & front() const noexcept;

  /*!
    \brief Returns a reference to the last character of the string.

    This method provides direct access to the last character of the string.

    \return A reference to the last character of the string.

    \pre The string must not be empty.

    \note The returned reference allows modification of the character.
    \note Use const version for read-only access.

    \sa back() const
  */
  [[nodiscard]] constexpr char & back() noexcept;

  /*!
    \brief Returns a const reference to the last character of the string.

    This method provides read-only access to the last character of the string.

    \return A const reference to the last character of the string.

    \pre The string must not be empty.

    \note The returned reference is read-only and cannot modify the character.
    \note Use the non-const overload to allow modification.

    \sa back() noexcept
  */
  [[nodiscard]] constexpr const char & back() const noexcept;

  /*!
    \brief Returns a pointer to the data of the string.

    This method returns a pointer to the internal character array that stores the string data. The returned pointer
    provides direct access to the string contents and can be used for low-level operations.

    \return A pointer to the internal character array.

    \note The returned pointer points to a null-terminated character array.
    \note The returned pointer allows modification of the string contents.
    \note Use const version for read-only access.

    \sa data() const
    \sa c_str()
  */
  [[nodiscard]] constexpr char * data() noexcept;

  /*!
    \brief Returns a constant pointer to the data of the string.

    This method returns a constant pointer to the internal character array that stores the string data. The returned
    pointer provides read-only access to the string contents and can be used for low-level operations.

    \return A constant pointer to the internal character array.

    \note The returned pointer points to a null-terminated character array.
    \note The returned pointer is read-only and cannot modify the string contents.
    \note Use the non-const overload to allow modification.

    \sa data()
    \sa c_str()
  */
  [[nodiscard]] constexpr const char * data() const noexcept;

  /*!
    \brief Returns a constant pointer to the C string representation of this string.

    This method returns a constant pointer to the C string representation of this string. The returned pointer provides
    read-only access to the string contents and can be used with C functions that require a char pointer.

    \return A constant pointer to the C string representation of this string.

    \note The returned pointer is read-only and cannot modify the string contents.
    \note This method is equivalent to data() const.

    \sa data() const
  */
  [[nodiscard]] constexpr const char * c_str() const noexcept;

  /*!
    \brief Checks if the string is empty.

    This method checks if the string is empty, i.e. its size is zero. An empty string contains no characters and has a
    length of zero.

    \return \c true if the string is empty (size is \c 0), \c false otherwise.

    \note An empty string has size zero.
    \note An empty string still contains a null terminator.
    \note This method is equivalent to the expression: `size() == 0`.

    \sa size()
  */
  [[nodiscard]] constexpr bool empty() const noexcept;

  /*!
    \brief Returns the size of the string.

    This method returns the current number of characters in the string, excluding the terminating null character. The
    size represents the actual length of the string content.

    \return The number of characters in the string, excluding the terminating null character.

    \note This method is equivalent to length().

    \sa length()
  */
  [[nodiscard]] constexpr size_t size() const noexcept;

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
    \note This method is useful for internationalization and text processing.
  */
  [[nodiscard]] size_t utf8_size() const noexcept;

  /*!
    \brief Returns the size of the string.

    This method returns the current number of characters in the string, excluding the terminating null character. The
    length represents the actual length of the string content.

    \return The number of characters in the string, excluding the terminating null character.

    \note This method is equivalent to size().

    \sa size()
  */
  [[nodiscard]] constexpr size_t length() const noexcept;

  /*!
    \brief Returns the maximum size of the string.

    This method returns the maximum possible size for the string, which is determined by the template parameter
    allocatedSize. The maximum size represents the size of the allocated buffer.

    \return The maximum number of characters in the string, excluding the terminating null character.

    \note The maximum size is determined at compile time.
    \note This method is equivalent to \a capacity() method.

    \sa capacity()
  */
  [[nodiscard]] constexpr size_t max_size() const noexcept;

  /*!
    \brief Returns the capacity of the string.

    This method returns the capacity of the allocated buffer for the string, which is determined by the template
    parameter allocatedSize.

    \return The capacity of the string in characters, excluding the terminating null character.

    \note The capacity is determined at compile time.
    \note This method is equivalent to \a max_size() method.

    \sa max_size()
  */
  [[nodiscard]] constexpr size_t capacity() const noexcept;

  /*!
    \brief Clears the contents of the string.

    This method resets the size of the string to zero and effectively clears the string.

    \post The string is empty (size is \c 0).
    \post All characters are cleared, but the buffer remains allocated.

    \note The string object is cleared, and its size is set to zero.
  */
  constexpr void clear() noexcept;

  /*!
    \brief Inserts other \a string at the specified \a index.

    This method inserts the contents from another \a string at the specified position. The insertion shifts existing
    characters to the right to make room for the new content.

    \param index  The position where the \a string should be inserted.
    \param string The source string to insert content from.

    \return A reference to this string after insertion.

    \pre The \a index must be within the bounds of the current string.
    \pre The combined length after insertion must not exceed the allocated size.

    \post The \a string is inserted at position \a index.
    \post The string size is increased by the size of the inserted \a string.
    \post All characters after \a index are shifted right.
  */
  constexpr FixedString<allocatedSize> & insert(size_t index, const FixedString<allocatedSize> & string) noexcept;

  /*!
    \brief Inserts a \ref toy::StringLike object at the specified \a index.

    This method inserts the contents from a \ref toy::StringLike object at the specified position. The insertion shifts
    existing characters to the right to make room for the new content.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param index  The position where the \ref toy::StringLike object should be inserted.
    \param string The source \ref toy::StringLike object to insert content from.

    \return A reference to this string after insertion.

    \pre The \a index must be within the bounds of the current string.
    \pre The combined length after insertion must not exceed the allocated size.

    \post The \ref toy::StringLike object is inserted at position \a index.
    \post The string size is increased by the size of the inserted \ref toy::StringLike object.
    \post All characters after \a index are shifted right.
  */
  template <StringLike stringType>
  constexpr FixedString<allocatedSize> & insert(size_t index, const stringType & string) noexcept;

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

    \post The C \a string is inserted at position \a index.
    \post The string size is increased by the size of the inserted C \a string.
    \post All characters after \a index are shifted right.
  */
  constexpr FixedString<allocatedSize> & insert(size_t index, const char * string) noexcept;

  /*!
    \brief Inserts a \a character repeated the given \a count times at the specified \a index.

    This method inserts a \a character repeated the given \a count of times at the specified position. The insertion
    shifts existing characters to the right to make room for the new content.

    \param index     The position where the characters should be inserted.
    \param character The character to insert.
    \param count     The number of times to repeat the \a character (default: \c 1).

    \return A reference to this string after insertion.

    \pre The \a index must be within the bounds of the current string.
    \pre The combined length after insertion must not exceed the allocated size.
  */
  constexpr FixedString<allocatedSize> & insert(size_t index, char character, size_t count = 1) noexcept;

  /*!
    \brief Erases characters from the string starting at the specified \a offset.

    This method removes characters from the string starting at the specified \a offset. The method can remove a specific
    number of characters or all characters from the \a offset to the end of the string. The remaining characters are
    shifted left to fill the gap, and the string size is updated accordingly.

    \param offset The starting position for erasing characters.
    \param count  The number of characters to erase (default: \ref npos). If count is \ref npos or equal to the
                  remaining length, then all characters from \a offset to the end are removed.

    \return A reference to this string after erasing.

    \pre The \a offset must be within the bounds of the current string.
    \pre The sum of \a offset and \a count must be less than or equal to the current string size.

    \post The specified range of characters is removed from the string.
    \post The string size is decreased by the number of erased characters.
    \post All characters after the erased range are shifted left.

    \note If \a count is \c npos, all characters from \a offset to the end are removed.
    \note If \a count is \c 0, the operation is a no-op.
    \note Erasing from an empty string has no effect.
  */
  constexpr FixedString<allocatedSize> & erase(size_t offset, size_t count = npos) noexcept;

  /*!
    \brief Appends a single \a character to the end of the string.

    This method appends a single \a character to the end of the current string, increasing its size by one.

    \param character The character to append.

    \pre The combined length after append must not exceed the allocated size.

    \note This method is equivalent to operator+= but more explicit in intent.

    \sa operator+=(char)
  */
  constexpr void push_back(char character) noexcept;

  /*!
    \brief Removes the last character from the string.

    This method removes the last character from the current string, decreasing its size by one.

    \pre The string must not be empty.

    \post The string size is decreased by one.

    \note If the string is empty, the behavior is undefined.
  */
  constexpr void pop_back() noexcept;

  /*!
    \brief Removes the last UTF-8 encoded character from the string.

    This method removes the last UTF-8 encoded character from the current string. Unlike pop_back() which removes a
    single character, this method removes the complete UTF-8 encoded character, which may consist of multiple bytes.

    \pre The string must not be empty.
    \pre The string must contain valid UTF-8 encoded text.

    \post The string size is decreased by the number of bytes that constituted the last UTF-8 encoded character.

    \note If the string is empty, the behavior is undefined.
    \note For ASCII strings, this method behaves identically to pop_back().
    \note This method is useful for internationalized text with non-ASCII characters.
  */
  constexpr void utf8_pop_back() noexcept;

  /*!
    \brief Appends other \a string to the end of this string.

    This method appends the contents of another \a string to the end of this string.

    \param string The source string to append content from.

    \return A reference to this string after appending.

    \pre The combined length after appending must not exceed the allocated size.

    \post The string is extended with the appended content.

    \note This method is equivalent to the addition assignment operator.

    \sa operator+=(const FixedString<allocatedSize> &)
  */
  constexpr FixedString<allocatedSize> & append(const FixedString<allocatedSize> & string) noexcept;

  /*!
    \brief Appends a \ref toy::StringLike object to the end of this string.

    This method appends the contents of a \ref toy::StringLike object to the end of this string.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string The source \ref toy::StringLike object to append content from.

    \return A reference to this string after appending.

    \pre The combined length after appending must not exceed the allocated size.

    \post The \ref toy::StringLike object is appended to the end of this string.
    \post The string size is increased by the size of the appended \ref toy::StringLike object.

    \note This method is equivalent to the addition assignment operator.

    \sa operator+=(const stringType &)
  */
  template <StringLike stringType>
  constexpr FixedString<allocatedSize> & append(const stringType & string) noexcept;

  /*!
    \brief Appends the C \a string to the end of this string.

    This method appends the contents of the C \a string to the end of this string.

    \param string The source C string to append content from.

    \return A reference to this string after appending.

    \pre The combined length after appending must not exceed the allocated size.
    \pre The source C \a string must not be null.

    \post The string is extended with the appended content.

    \note This method is equivalent to the addition assignment operator.

    \sa operator+=(const char *)
  */
  constexpr FixedString<allocatedSize> & append(const char * string) noexcept;

  /*!
    \brief Appends a \a character repeated the given \a count times to the end of this string.

    This method appends a \a character repeated the given \a count of times to the end of this string.

    \param count     The number of times to repeat the \a character.
    \param character The character to append.

    \return A reference to this string after appending.

    \pre The combined length after appending must not exceed the allocated size.

    \post The \a character is appended \a count times to the end of the string.
    \post The string size is increased by \a count.
  */
  constexpr FixedString<allocatedSize> & append(size_type count, char character) noexcept;

  /*!
    \brief Appends other \a string to the end of this string.

    This operator appends the contents of another \a string to the end of this string.

    \param string The source string to append content from.

    \return A reference to this string after appending.

    \pre The combined length after appending must not exceed the allocated size.

    \post The \a string is appended to the end of this string.
    \post The string size is increased by the size of the appended \a string.
  */
  constexpr FixedString<allocatedSize> & operator+=(const FixedString<allocatedSize> & string) noexcept;

  /*!
    \brief Appends a \ref toy::StringLike object to the end of this string.

    This operator appends the contents of a \ref toy::StringLike object to the end of this string.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string The source \ref toy::StringLike object to append content from.

    \return A reference to this string after appending.

    \pre The combined length after appending must not exceed the allocated size.

    \post The string is extended with the appended content.
  */
  template <StringLike stringType>
  constexpr FixedString<allocatedSize> & operator+=(const stringType & string) noexcept;

  /*!
    \brief Appends the C \a string to the end of this string.

    This operator appends the contents of the C \a string to the end of this string.

    \param string The source C string to append content from.

    \return A reference to this string after appending.

    \pre The combined length after appending must not exceed the allocated size.
    \pre The source C \a string must not be null.

    \post The string is extended with the appended content.
  */
  constexpr FixedString<allocatedSize> & operator+=(const char * string) noexcept;

  /*!
    \brief Appends a \a character to the end of this string.

    This operator appends a \a character to the end of this string.

    \param character The character to append.

    \return A reference to this string after appending.

    \pre The current string size must be less than the allocated capacity.

    \post The \a character is appended to the end of this string.
    \post The string size is increased by \c 1.

    \sa append(size_type, char)
    \sa push_back(char)
  */
  constexpr FixedString<allocatedSize> & operator+=(char character) noexcept;

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

    \post The specified range is replaced with the \a string.
  */
  constexpr FixedString<allocatedSize> & replace(size_t pos, size_t count,
                                                 const FixedString<allocatedSize> & string) noexcept;

  /*!
    \brief Replaces a portion of the string with a \ref toy::StringLike object.

    This method replaces a specified range of characters in the current string with the contents of a \ref
    toy::StringLike object. The replacement operation shifts existing characters as needed to accommodate the new
    content.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param pos    The starting position for the replacement.
    \param count  The number of characters to replace.
    \param string The source \ref toy::StringLike object to replace content from.

    \return A reference to this string after replacement.

    \pre The \a pos must be within the bounds of the current string.
    \pre The replacement range ( \a pos + \a count ) must be within the string bounds.
    \pre The resulting string size must not exceed the allocated capacity.

    \post The specified range is replaced with the \ref toy::StringLike object.
  */
  template <StringLike stringType>
  constexpr FixedString<allocatedSize> & replace(size_t pos, size_t count, const stringType & string) noexcept;

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

    \post The specified range is replaced with the C \a string.
  */
  constexpr FixedString<allocatedSize> & replace(size_t pos, size_t count, const char * string) noexcept;

  /*!
    \brief Replaces a portion of the string with a \a character repeated the given \a count of times.

    This method replaces a specified range of characters in the current string with a \a character repeated the given \a
    count of times. The replacement operation shifts existing characters as needed to accommodate the new content.

    \param pos             The starting position for the replacement.
    \param count           The number of characters to replace.
    \param character       The character to replace with.
    \param charactersCount The number of times to repeat the \a character (default: \c 1).

    \return A reference to this string after replacement.

    \pre The \a pos must be within the bounds of the current string.
    \pre The replacement range ( \a pos + \a count ) must be within the string bounds.
    \pre The resulting string size must not exceed the allocated capacity.

    \post The specified range is replaced with the \a character repeated \a charactersCount times.
  */
  constexpr FixedString<allocatedSize> & replace(size_t pos, size_t count, char character,
                                                 size_t charactersCount = 1) noexcept;

  /*!
    \brief Copies characters from the string to a destination buffer.

    This method copies up to \a count characters from the string starting at position \a pos to the destination buffer.
    The method returns the actual number of characters copied, which may be less than \a count if the end of the string
    is reached.

    \param dest  The destination buffer to copy characters to.
    \param count The maximum number of characters to copy.
    \param pos      The starting position in the string to copy from (default: \c 0).

    \return The actual number of characters copied.

    \pre The \a pos must be within the bounds of the current string.
    \pre The \a dest pointer must not be null.
    \pre The \a dest buffer must not overlap this string's internal storage.
    \pre The \a dest buffer must have sufficient space for the copied characters.

    \note If \a count is \ref npos or exceeds remaining characters from \a pos, all remaining characters are copied.
    \note The destination buffer is not null-terminated by this method.
  */
  constexpr size_t copy(char * dest, size_t count, size_t pos = 0) const noexcept;

  /*!
    \brief Swaps the contents of this string with another \a string.

    This method exchanges the contents of this string with another \a string of the same type. Both strings must have
    the same allocated size. The operation is performed efficiently by swapping the internal data and size information.

    \param string The string to swap contents with.

    \post This string contains the contents that were in other \a string.
    \post The other \a string contains the contents that were in this string.
    \post The sizes of both strings are exchanged.

    \note Self-swap is handled correctly and safely (no-op).
  */
  constexpr void swap(FixedString<allocatedSize> & string) noexcept;

  /*!
    \brief Finds the first occurrence of other \a string in the string.

    This method searches for the first occurrence of the specified \a string within this string, starting from the given
    \a position.

    \param string   The source string to search for.
    \param position The position to start searching from (default: \c 0).

    \return The position of the first occurrence of other \a string, or \ref npos if not found.

    \pre The \a position must be less than the string size.

    \note The search is case-sensitive.
  */
  [[nodiscard]] constexpr size_t find(const FixedString<allocatedSize> & string, size_t position = 0) const noexcept;

  /*!
    \brief Finds the first occurrence of a \ref toy::StringLike object in the string.

    This method searches for the first occurrence of a \ref toy::StringLike object within this string, starting from the
    given \a position.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string   The source \ref toy::StringLike object to search for.
    \param position The position to start searching from (default: \c 0).

    \return The position of the first occurrence of a \ref toy::StringLike object, or \ref npos if not found.

    \pre The \a position must be less than the string size.

    \note The search is case-sensitive.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr size_t find(const stringType & string, size_t position = 0) const noexcept;

  /*!
    \brief Finds the first occurrence of the C \a string in the string.

    This method searches for the first occurrence of the C \a string within this string, starting from the given \a
    position.

    \param string   The source C string to search for.
    \param position The position to start searching from (default: \c 0).

    \return The position of the first occurrence of the C \a string, or \ref npos if not found.

    \pre The \a position must be less than the string size.
    \pre The \a string must not be null.

    \note The search is case-sensitive.
  */
  [[nodiscard]] constexpr size_t find(const char * string, size_t position = 0) const noexcept;

  /*!
    \brief Finds the first occurrence of a \a character in the string.

    This method searches for the first occurrence of the specified \a character within this string, starting from the
    given \a position.

    \param character The character to search for.
    \param position  The position to start searching from (default: \c 0).

    \return The position of the first occurrence of the \a character, or \ref npos if not found.

    \pre The \a position must be less than the string size.

    \note The search is case-sensitive.
  */
  [[nodiscard]] constexpr size_t find(char character, size_t position = 0) const noexcept;

  /*!
    \brief Finds the last occurrence of other \a string in the string.

    This method searches for the last occurrence of the specified \a string within this string, starting from the given
    \a position and searching backwards.

    \param string   The source string to search for.
    \param position The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of other \a string, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than or equal to the maximum valid start index: the string
         size minus the size of \a string.

    \note The search is case-sensitive.
    \note If \a string is empty, returns \a position if within bounds, otherwise returns the string size.
  */
  [[nodiscard]] constexpr size_t rfind(const FixedString<allocatedSize> & string,
                                       size_t position = npos) const noexcept;

  /*!
    \brief Finds the last occurrence of a \ref toy::StringLike object in the string.

    This method searches for the last occurrence of a \ref toy::StringLike object within this string, starting from the
    given \a position and searching backwards.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string   The source \ref toy::StringLike object to search for.
    \param position The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of a \ref toy::StringLike object, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than or equal to the maximum valid start index: the string
         size minus the size of a \ref toy::StringLike object.

    \note The search is case-sensitive.
    \note If a \ref toy::StringLike object is empty, returns \a position if within bounds, otherwise returns the string
          size.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr size_t rfind(const stringType & string, size_t position = npos) const noexcept;

  /*!
    \brief Finds the last occurrence of the C \a string in the string.

    This method searches for the last occurrence of the C \a string within this string, starting from the given \a
    position and searching backwards.

    \param string   The source C string to search for.
    \param position The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of the C \a string, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than or equal to the maximum valid start index: the string
         size minus the size of the C \a string.
    \pre The \a string must not be null.

    \note The search is case-sensitive.
    \note If the C \a string is empty, returns \a position if within bounds, otherwise returns the string size.
  */
  [[nodiscard]] constexpr size_t rfind(const char * string, size_t position = npos) const noexcept;

  /*!
    \brief Finds the last occurrence of a \a character in the string.

    This method searches for the last occurrence of the specified \a character within this string, starting from the
    given \a position and searching backwards.

    \param character The character to search for.
    \param position  The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of the \a character, or \ref npos if not found.

    \pre The \a position must be less than the string size.

    \note The search is case-sensitive.
  */
  [[nodiscard]] constexpr size_t rfind(char character, size_t position = npos) const noexcept;

  /*!
    \brief Finds the first occurrence of any character from the specified \a string.

    This method searches for the first occurrence of any character from the specified \a string within this string,
    starting from the given \a position.

    \param string   The string containing characters to search for.
    \param position The position to start searching from (default: \c 0).

    \return The position of the first occurrence of any character from \a string, or \ref npos if not found.

    \pre The \a position must be less than the string size.

    \note The search is case-sensitive.
    \note If \a string is empty, this method returns \ref npos.
  */
  [[nodiscard]] constexpr size_t find_first_of(const FixedString<allocatedSize> & string,
                                               size_t position = 0) const noexcept;

  /*!
    \brief Finds the first occurrence of any character from a \ref toy::StringLike object.

    This method searches for the first occurrence of any character from a \ref toy::StringLike object within this
    string, starting from the given \a position.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string   The \ref toy::StringLike object containing characters to search for.
    \param position The position to start searching from (default: \c 0).

    \return The position of the first occurrence of any character from a \ref toy::StringLike object, or \ref npos if
            not found.

    \pre The \a position must be less than the string size.

    \note The search is case-sensitive.
    \note If a \ref toy::StringLike object is empty, this method returns \ref npos.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr size_t find_first_of(const stringType & string, size_t position = 0) const noexcept;

  /*!
    \brief Finds the first occurrence of any character from the C \a string.

    This method searches for the first occurrence of any character from the C \a string within this string, starting
    from the given \a position.

    \param string   The C string containing characters to search for.
    \param position The position to start searching from (default: \c 0).

    \return The position of the first occurrence of any character from the C \a string, or \ref npos if not found.

    \pre The \a position must be less than the string size.
    \pre The \a string must not be null.

    \note The search is case-sensitive.
    \note If the C \a string is empty, this method returns \ref npos.
  */
  [[nodiscard]] constexpr size_t find_first_of(const char * string, size_t position = 0) const noexcept;

  /*!
    \brief Finds the first occurrence of the specified \a character.

    This method searches for the first occurrence of the specified \a character within this string, starting from the
    given \a position.

    \param character The character to search for.
    \param position  The position to start searching from (default: \c 0).

    \return The position of the first occurrence of the \a character, or \ref npos if not found.

    \pre The \a position must be less than the string size.

    \note The search is case-sensitive.
    \note This method is equivalent to find(character, position).
  */
  [[nodiscard]] constexpr size_t find_first_of(char character, size_t position = 0) const noexcept;

  /*!
    \brief Finds the first occurrence of any character not from the specified \a string.

    This method searches for the first occurrence of any character that is not present in the specified \a string within
    this string, starting from the given \a position.

    \param string   The string containing characters to exclude from search.
    \param position The position to start searching from (default: \c 0).

    \return The position of the first occurrence of any character not from \a string, or \ref npos if not found.

    \pre The \a position must be less than the string size.

    \note The search is case-sensitive.
    \note If \a string is empty, returns \a position if within bounds, otherwise returns \ref npos.
  */
  [[nodiscard]] constexpr size_t find_first_not_of(const FixedString<allocatedSize> & string,
                                                   size_t position = 0) const noexcept;

  /*!
    \brief Finds the first occurrence of any character not from a \ref toy::StringLike object.

    This method searches for the first occurrence of any character that is not present in a \ref toy::StringLike object
    within this string, starting from the given \a position.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string   The \ref toy::StringLike object containing characters to exclude from search.
    \param position The position to start searching from (default: \c 0).

    \return The position of the first occurrence of any character not from a \ref toy::StringLike object, or \ref npos
            if not found.

    \pre The \a position must be less than the string size.

    \note The search is case-sensitive.
    \note If a \ref toy::StringLike object is empty, returns \a position if within bounds, otherwise returns \ref npos.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr size_t find_first_not_of(const stringType & string, size_t position = 0) const noexcept;

  /*!
    \brief Finds the first occurrence of any character not from the C \a string.

    This method searches for the first occurrence of any character that is not present in the C \a string within this
    string, starting from the given \a position.

    \param string   The C string containing characters to exclude from search.
    \param position The position to start searching from (default: \c 0).

    \return The position of the first occurrence of any character not from the C \a string, or \ref npos if not found.

    \pre The \a position must be less than the string size.
    \pre The \a string must not be null.

    \note The search is case-sensitive.
    \note If the C \a string is empty, returns \a position if within bounds, otherwise returns \ref npos.
  */
  [[nodiscard]] constexpr size_t find_first_not_of(const char * string, size_t position = 0) const noexcept;

  /*!
    \brief Finds the first occurrence of any character not equal to the specified \a character.

    This method searches for the first occurrence of any character that is not equal to the specified \a character
    within this string, starting from the given \a position.

    \param character The character to exclude from search.
    \param position  The position to start searching from (default: \c 0).

    \return The position of the first occurrence of any character not equal to \a character, or \ref npos if not found.

    \pre The \a position must be less than the string size.

    \note The search is case-sensitive.
  */
  [[nodiscard]] constexpr size_t find_first_not_of(char character, size_t position = 0) const noexcept;

  /*!
    \brief Finds the last occurrence of any character from the specified \a string.

    This method searches for the last occurrence of any character from the specified \a string within this string,
    starting from the given \a position and searching backwards.

    \param string   The string containing characters to search for.
    \param position The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of any character from \a string, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than the string size.

    \note The search is case-sensitive.
    \note If \a string is empty, this method returns \ref npos.
  */
  [[nodiscard]] constexpr size_t find_last_of(const FixedString<allocatedSize> & string,
                                              size_t position = npos) const noexcept;

  /*!
    \brief Finds the last occurrence of any character from a \ref toy::StringLike object.

    This method searches for the last occurrence of any character from a \ref toy::StringLike object within this string,
    starting from the given \a position and searching backwards.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string   The \ref toy::StringLike object containing characters to search for.
    \param position The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of any character from a \ref toy::StringLike object, or \ref npos if not
            found.

    \pre If \a position is not \ref npos, it must be less than the string size.

    \note The search is case-sensitive.
    \note If a \ref toy::StringLike object is empty, this method returns \ref npos.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr size_t find_last_of(const stringType & string, size_t position = npos) const noexcept;

  /*!
    \brief Finds the last occurrence of any character from the C \a string.

    This method searches for the last occurrence of any character from the C \a string within this string, starting from
    the given \a position and searching backwards.

    \param string   The C string containing characters to search for.
    \param position The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of any character from the C \a string, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than the string size.
    \pre The \a string must not be null.

    \note The search is case-sensitive.
    \note If the C \a string is empty, this method returns \ref npos.
  */
  [[nodiscard]] constexpr size_t find_last_of(const char * string, size_t position = npos) const noexcept;

  /*!
    \brief Finds the last occurrence of the specified \a character.

    This method searches for the last occurrence of the specified \a character within this string, starting from the
    given \a position and searching backwards.

    \param character The character to search for.
    \param position  The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of the \a character, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than the string size.

    \note The search is case-sensitive.
    \note This method is equivalent to rfind(char, size_t) const.

    \sa rfind(char, size_t) const
  */
  [[nodiscard]] constexpr size_t find_last_of(char character, size_t position = npos) const noexcept;

  /*!
    \brief Finds the last occurrence of any character not from the specified \a string.

    This method searches for the last occurrence of any character that is not present in the specified \a string within
    this string, starting from the given \a position and searching backwards.

    \param string   The string containing characters to exclude from search.
    \param position The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of any character not from \a string, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than the string size.

    \note The search is case-sensitive.
    \note If \a string is empty, returns \a position if within bounds, otherwise returns \ref npos.
  */
  [[nodiscard]] constexpr size_t find_last_not_of(const FixedString<allocatedSize> & string,
                                                  size_t position = npos) const noexcept;

  /*!
    \brief Finds the last occurrence of any character not from a \ref toy::StringLike object.

    This method searches for the last occurrence of any character that is not present in a \ref toy::StringLike object
    within this string, starting from the given \a position and searching backwards.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string   The \ref toy::StringLike object containing characters to exclude from search.
    \param position The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of any character not from a \ref toy::StringLike object, or \ref npos if
            not found.

    \pre If \a position is not \ref npos, it must be less than the string size.

    \note The search is case-sensitive.
    \note If a \ref toy::StringLike object is empty, returns \a position if within bounds, otherwise returns \ref npos.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr size_t find_last_not_of(const stringType & string, size_t position = npos) const noexcept;

  /*!
    \brief Finds the last occurrence of any character not from the C \a string.

    This method searches for the last occurrence of any character that is not present in the C \a string within this
    string, starting from the given \a position and searching backwards.

    \param string   The C string containing characters to exclude from search.
    \param position The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of any character not from the C \a string, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than the string size.
    \pre The \a string must not be null.

    \note The search is case-sensitive.
    \note If the C \a string is empty, returns \a position if within bounds, otherwise returns \ref npos.
  */
  [[nodiscard]] constexpr size_t find_last_not_of(const char * string, size_t position = npos) const noexcept;

  /*!
    \brief Finds the last occurrence of any character not equal to the specified \a character.

    This method searches for the last occurrence of any character that is not equal to the specified \a character within
    this string, starting from the given \a position and searching backwards.

    \param character The character to exclude from search.
    \param position  The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of any character not equal to \a character, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than the string size.

    \note The search is case-sensitive.
  */
  [[nodiscard]] constexpr size_t find_last_not_of(char character, size_t position = npos) const noexcept;

  /*!
    \brief Compares this string with another \a string lexicographically.

    This method performs a lexicographic comparison between this string and another \a string. The comparison is
    performed character by character using the character's numeric value.

    \param string The string to compare with this string.

    \return A negative value if this string is lexicographically less than \a string, zero if they are equal, or a
            positive value if this string is lexicographically greater than \a string.

    \note The comparison is case-sensitive.
    \note The comparison stops at the first character that differs between the strings.
    \note If one string is a prefix of another, the shorter is considered lexicographically smaller.
  */
  [[nodiscard]] constexpr int compare(const FixedString<allocatedSize> & string) const noexcept;

  /*!
    \brief Compares this string with a \ref toy::StringLike object lexicographically.

    This method performs a lexicographic comparison between this string and a \ref toy::StringLike object. The
    comparison is performed character by character using the character's numeric value.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string The \ref toy::StringLike object to compare with this string.

    \return A negative value if this string is lexicographically less than a \ref toy::StringLike object, zero if they
            are equal, or a positive value if this string is lexicographically greater than a \ref toy::StringLike
            object.

    \note The comparison is case-sensitive.
    \note The comparison stops at the first character that differs between the strings.
    \note If one string is a prefix of another, the shorter is considered lexicographically smaller.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr int compare(const stringType & string) const noexcept;

  /*!
    \brief Compares this string with the C \a string lexicographically.

    This method performs a lexicographic comparison between this string and the C \a string. The comparison is
    performed character by character using the character's numeric value.

    \param string The C string to compare with this string.

    \return A negative value if this string is lexicographically less than C \a string, zero if they are equal, or a
            positive value if this string is lexicographically greater than C \a string.

    \pre The C \a string must not be null.

    \note The comparison is case-sensitive.
    \note The comparison stops at the first character that differs between the strings.
    \note If one string is a prefix of another, the shorter is considered lexicographically smaller.
  */
  [[nodiscard]] constexpr int compare(const char * string) const noexcept;

  /*!
    \brief Checks if the string starts with the specified \a string.

    This method checks if the current string starts with the specified \a string. The comparison is performed character
    by character from the beginning of the string.

    \param string The string to check if this string starts with.

    \return \c true if this string starts with the specified \a string, \c false otherwise.

    \note The comparison is case-sensitive.
    \note If the specified \a string is empty, this method returns \c true.
    \note If the specified \a string is longer than this string, returns \c false.
  */
  [[nodiscard]] constexpr bool starts_with(const FixedString<allocatedSize> & string) const noexcept;

  /*!
    \brief Checks if the string starts with a \ref toy::StringLike object.

    This method checks if the current string starts with a \ref toy::StringLike object. The comparison is performed
    character by character from the beginning of the string.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string The \ref toy::StringLike object to check if this string starts with.

    \return \c true if this string starts with a \ref toy::StringLike object, \c false otherwise.

    \note The comparison is case-sensitive.
    \note If a \ref toy::StringLike object is empty, this method returns \c true.
    \note If a \ref toy::StringLike object is longer than this string, returns \c false.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr bool starts_with(const stringType & string) const noexcept;

  /*!
    \brief Checks if the string starts with the specified C \a string.

    This method checks if the current string starts with the C \a string. The comparison is performed character by
    character from the beginning of the string.

    \param string The C string to check if this string starts with.

    \return \c true if this string starts with the C \a string, \c false otherwise.

    \pre The C \a string must not be null.

    \note The comparison is case-sensitive.
    \note If the C \a string is empty, this method returns \c true.
    \note If the C \a string is longer than this string, this method returns \c false.
  */
  [[nodiscard]] constexpr bool starts_with(const char * string) const noexcept;

  /*!
    \brief Checks if the string starts with the specified \a character.

    This method checks if the current string starts with the specified \a character. The comparison is performed on the
    first character of the string.

    \param character The character to check if this string starts with.

    \return \c true if this string starts with the specified \a character, \c false otherwise.

    \note The comparison is case-sensitive.
    \note If the string is empty, this method returns \c false.
  */
  [[nodiscard]] constexpr bool starts_with(char character) const noexcept;

  /*!
    \brief Checks if the string ends with the specified \a string.

    This method checks if the current string ends with the specified \a string. The comparison is performed character by
    character from the end of the string.

    \param string The string to check if this string ends with.

    \return \c true if this string ends with the specified \a string, \c false otherwise.

    \note The comparison is case-sensitive.
    \note If the specified \a string is empty, this method returns \c true.
    \note If the specified \a string is longer than this string, returns \c false.
  */
  [[nodiscard]] constexpr bool ends_with(const FixedString<allocatedSize> & string) const noexcept;

  /*!
    \brief Checks if the string ends with a \ref toy::StringLike object.

    This method checks if the current string ends with a \ref toy::StringLike object. The comparison is performed
    character by character from the end of the string.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string The \ref toy::StringLike object to check if this string ends with.

    \return \c true if this string ends with a \ref toy::StringLike object, \c false otherwise.

    \note The comparison is case-sensitive.
    \note If a \ref toy::StringLike object is empty, this method returns \c true.
    \note If a \ref toy::StringLike object is longer than this string, returns \c false.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr bool ends_with(const stringType & string) const noexcept;

  /*!
    \brief Checks if the string ends with the C \a string.

    This method checks if the current string ends with the C \a string. The comparison is performed character by
    character from the end of the string.

    \param string The C string to check if this string ends with.

    \return \c true if this string ends with the C \a string, \c false otherwise.

    \pre The C \a string must not be null.

    \note The comparison is case-sensitive.
    \note If the C \a string is empty, this method returns \c true.
    \note If the C \a string is longer than this string, this method returns \c false.
  */
  [[nodiscard]] constexpr bool ends_with(const char * string) const noexcept;

  /*!
    \brief Checks if the string ends with the specified \a character.

    This method checks if the current string ends with the specified \a character. The comparison is performed on the
    last character of the string.

    \param character The character to check if this string ends with.

    \return \c true if this string ends with the specified \a character, \c false otherwise.

    \note The comparison is case-sensitive.
    \note If the string is empty, this method returns \c false.
  */
  [[nodiscard]] constexpr bool ends_with(char character) const noexcept;

  /*!
    \brief Checks if the string contains the specified \a string.

    This method checks if the current string contains the specified \a string anywhere within it.

    \param string The string to search for within this string.

    \return \c true if this string contains the specified \a string, \c false otherwise.

    \note The search is case-sensitive.
    \note If the specified \a string is empty, this method returns \c true.
    \note If the specified \a string is longer than this string, returns \c false.
  */
  [[nodiscard]] constexpr bool contains(const FixedString<allocatedSize> & string) const noexcept;

  /*!
    \brief Checks if the string contains a \ref toy::StringLike object.

    This method checks if the current string contains a \ref toy::StringLike object anywhere within it.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string The \ref toy::StringLike object to search for within this string.

    \return \c true if this string contains a \ref toy::StringLike object, \c false otherwise.

    \note The search is case-sensitive.
    \note If a \ref toy::StringLike object is empty, this method returns \c true.
    \note If a \ref toy::StringLike object is longer than this string, returns \c false.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr bool contains(const stringType & string) const noexcept;

  /*!
    \brief Checks if the string contains the C \a string.

    This method checks if the current string contains the C \a string anywhere within it.

    \param string The C string to search for within this string.

    \return \c true if this string contains the C \a string, \c false otherwise.

    \pre The C \a string must not be null.

    \note The search is case-sensitive.
    \note If the C \a string is empty, this method returns \c true.
    \note If the C \a string is longer than this string, this method returns \c false.
  */
  [[nodiscard]] constexpr bool contains(const char * string) const noexcept;

  /*!
    \brief Checks if the string contains the specified \a character.

    This method checks if the current string contains the specified \a character anywhere within it.

    \param character The character to search for within this string.

    \return \c true if this string contains the specified \a character, \c false otherwise.

    \note The search is case-sensitive.
    \note If the string is empty, this method returns \c false.
  */
  [[nodiscard]] constexpr bool contains(char character) const noexcept;

  /*!
    \brief Returns a substring of this string.

    This method creates and returns a new FixedString object containing a substring of this string, starting at the
    specified \a position and containing up to \a count characters.

    \param position The starting position of the substring (default: \c 0).
    \param count    The maximum number of characters to include in the substring (default: \ref npos). If \ref npos or
                    exceeds the remaining characters, all characters from \a position to the end are included.

    \return A new FixedString object containing the specified substring.

    \pre The \a position must be less than or equal to the string size.

    \note If \a position is equal to the string size, an empty string is returned.
    \note If \a count is \ref npos or exceeds remaining characters from \a position, all remaining characters are
    included.
  */
  [[nodiscard]] constexpr FixedString<allocatedSize> substr(size_t position = 0, size_t count = npos) const noexcept;

  /// The special value, its exact meaning depends on the context
  static constexpr size_t npos = size_t(-1);

private:
  /*!
    \brief Helper method for inserting \a data at a specific \a position.

    This private method performs the common insertion logic used by all insert methods. It shifts existing data to the
    right and inserts new \a data at the specified position.

    \param position The position where to insert the data.
    \param data     The data to insert.
    \param dataSize The size of the data to insert.

    \pre The \a position must be within the bounds of the current string.
    \pre The \a data must not be null.
    \pre The combined length after insertion must not exceed the allocated capacity.
  */
  constexpr void _insert_raw(size_t position, const char * data, size_t dataSize) noexcept;

  /*!
    \brief Helper method for appending \a data to the end of the string.

    This private method performs the common append logic used by all append methods. It copies new \a data to the end of
    the string and updates the size accordingly.

    \param data     The data to append.
    \param dataSize The size of the data to append.

    \pre The \a data must not be null.
    \pre The combined length after appending must not exceed the allocated capacity.
  */
  constexpr void _append_raw(const char * data, size_t dataSize) noexcept;

  /*!
    \brief Helper method for replacing data at a specific \a position.

    This private method performs the common replacement logic used by all replace methods. It handles the shifting of
    existing data and insertion of new \a data at the specified \a position.

    \param position The position where to replace the data.
    \param oldCount The number of characters to replace.
    \param data     The new data to insert.
    \param dataSize The size of the new data.

    \pre The \a position must be within the bounds of the current string.
    \pre The \a data must not be null.
    \pre The replacement range must be within string bounds.
    \pre The resulting string size must not exceed the allocated capacity.
  */
  constexpr void _replace_raw(size_t position, size_t oldCount, const char * data, size_t dataSize) noexcept;

  /*!
    \brief Helper method for finding \a data in the string.

    This private method performs the common search logic used by all find methods. It searches for the specified \a data
    starting from the given \a position.

    \param position The position to start searching from.
    \param data     The data to search for.
    \param dataSize The size of the data to search for.

    \return The position of the first occurrence of \a data, or \ref npos if not found.

    \pre The \a position must be less than the string size.
    \pre The \a data must not be null.
  */
  constexpr size_t _find_raw(size_t position, const char * data, size_t dataSize) const noexcept;

  /*!
    \brief Helper method for finding \a data in the string backwards.

    This private method performs the common reverse search logic used by all rfind methods. It searches for the
    specified \a data starting from the given \a position and searching backwards.

    \param position The position to start searching from.
    \param data     The data to search for.
    \param dataSize The size of the data to search for.

    \return The position of the last occurrence of \a data, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than or equal to the string size.
    \pre The \a data must not be null.
  */
  constexpr size_t _rfind_raw(size_t position, const char * data, size_t dataSize) const noexcept;

  /*!
    \brief Helper method for finding the first occurrence of any character from \a data.

    This private method performs the common search logic used by all find_first_of methods. It searches for the first
    occurrence of any character from the specified \a data starting from the given \a position.

    \param position The position to start searching from.
    \param data     The data containing characters to search for.
    \param dataSize The size of the data containing characters to search for.

    \return The position of the first occurrence of any character from \a data, or \ref npos if not found.

    \pre The \a position must be less than the string size.
    \pre The \a data must not be null.
  */
  constexpr size_t _find_first_of_raw(size_t position, const char * data, size_t dataSize) const noexcept;

  /*!
    \brief Helper method for finding the first occurrence of any character not from \a data.

    This private method performs the common search logic used by all find_first_not_of methods. It searches for the
    first occurrence of any character that is not present in the specified \a data starting from the given \a position.

    \param position The position to start searching from.
    \param data     The data containing characters to exclude from search.
    \param dataSize The size of the data containing characters to exclude from search.

    \return The position of the first occurrence of any character not from \a data, or \ref npos if not found.

    \pre The \a position must be less than the string size.
    \pre The \a data must not be null.
  */
  constexpr size_t _find_first_not_of_raw(size_t position, const char * data, size_t dataSize) const noexcept;

  /*!
    \brief Helper method for finding the last occurrence of any character from \a data.

    This private method performs the common reverse search logic used by all find_last_of methods. It searches for the
    last occurrence of any character from the specified \a data starting from the given \a position and searching
    backwards.

    \param position The position to start searching from. If \ref npos, searches from the end.
    \param data     The data containing characters to search for.
    \param dataSize The size of the data containing characters to search for.

    \return The position of the last occurrence of any character from \a data, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than the string size.
    \pre The \a data must not be null.
  */
  constexpr size_t _find_last_of_raw(size_t position, const char * data, size_t dataSize) const noexcept;

  /*!
    \brief Helper method for finding the last occurrence of any character not from \a data.

    This private method performs the common reverse search logic used by all find_last_not_of methods. It searches for
    the last occurrence of any character that is not present in the specified \a data starting from the given \a
    position and searching backwards.

    \param position The position to start searching from. If \ref npos, searches from the end.
    \param data     The data containing characters to exclude from search.
    \param dataSize The size of the data containing characters to exclude from search.

    \return The position of the last occurrence of any character not from \a data, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than the string size.
    \pre The \a data must not be null.
  */
  constexpr size_t _find_last_not_of_raw(size_t position, const char * data, size_t dataSize) const noexcept;

  /// Internal character buffer storing the string data
  FixedStringStorage<allocatedSize> _storage;
};

/*!
  \brief Concatenation operator for two FixedString objects.

  This operator creates a new FixedString object by concatenating the contents of two FixedString objects. The result
  will contain the characters from the left-hand side followed by the characters from the right-hand side.

  \tparam allocatedSize1 The size of the first FixedString's internal buffer.
  \tparam allocatedSize2 The size of the second FixedString's internal buffer.

  \param lhs The left-hand side FixedString object.
  \param rhs The right-hand side FixedString object.

  \return A new FixedString object containing the concatenated result.

  \note Result size is sum of both input sizes, must not exceed allocated size.
*/
template <size_t allocatedSize1, size_t allocatedSize2>
[[nodiscard]] constexpr FixedString<allocatedSize1> operator+(const FixedString<allocatedSize1> & lhs,
                                                              const FixedString<allocatedSize2> & rhs) noexcept;

/*!
  \brief Concatenation operator for FixedString and \ref toy::StringLike object.

  This operator creates a new FixedString object by concatenating a FixedString with any \ref toy::StringLike object.
  The result will contain the characters from the left-hand side followed by the characters from the right-hand side.

  \tparam allocatedSize The size of the FixedString's internal buffer.
  \tparam stringType The type of the \ref toy::StringLike object. Must satisfy the \ref toy::StringLike concept.

  \param lhs The left-hand side FixedString object.
  \param rhs The right-hand side \ref toy::StringLike object.

  \return A new FixedString object containing the concatenated result.

  \note Result size is sum of both input sizes, must not exceed allocated size.
*/
template <size_t allocatedSize, StringLike stringType>
[[nodiscard]] constexpr FixedString<allocatedSize> operator+(const FixedString<allocatedSize> & lhs,
                                                             const stringType & rhs) noexcept;

/*!
  \brief Concatenation operator for \ref toy::StringLike object and FixedString.

  This operator creates a new FixedString object by concatenating any \ref toy::StringLike object with a FixedString.
  The result will contain the characters from the left-hand side followed by the characters from the right-hand side.

  \tparam stringType The type of the \ref toy::StringLike object. Must satisfy the \ref toy::StringLike concept.
  \tparam allocatedSize The size of the FixedString's internal buffer.

  \param lhs The left-hand side \ref toy::StringLike object.
  \param rhs The right-hand side FixedString object.

  \return A new FixedString object containing the concatenated result.

  \note Result size is sum of both input sizes, must not exceed allocated size.
*/
template <size_t allocatedSize, StringLike stringType>
[[nodiscard]] constexpr FixedString<allocatedSize> operator+(const stringType & lhs,
                                                             const FixedString<allocatedSize> & rhs) noexcept;

/*!
  \brief Concatenation operator for FixedString and C-string.

  This operator creates a new FixedString object by concatenating a FixedString with a C-string. The result will contain
  the characters from the left-hand side followed by the characters from the right-hand side.

  \tparam allocatedSize The size of the FixedString's internal buffer.

  \param lhs The left-hand side FixedString object.
  \param rhs The right-hand side C-string.

  \return A new FixedString object containing the concatenated result.

  \pre The \a rhs C-string must not be null.

  \note Result size is sum of both input sizes, must not exceed allocated size.
*/
template <size_t allocatedSize>
[[nodiscard]] constexpr FixedString<allocatedSize> operator+(const FixedString<allocatedSize> & lhs,
                                                             const char * rhs) noexcept;

/*!
  \brief Concatenation operator for C-string and FixedString.

  This operator creates a new FixedString object by concatenating a C-string with a FixedString. The result will contain
  the characters from the left-hand side followed by the characters from the right-hand side.

  \tparam allocatedSize The size of the FixedString's internal buffer.

  \param lhs The left-hand side C-string.
  \param rhs The right-hand side FixedString object.

  \pre The \a lhs C-string must not be null.

  \return A new FixedString object containing the concatenated result.

  \note Result size is sum of both input sizes, must not exceed allocated size.
*/
template <size_t allocatedSize>
[[nodiscard]] constexpr FixedString<allocatedSize> operator+(const char * lhs,
                                                             const FixedString<allocatedSize> & rhs) noexcept;

/*!
  \brief Concatenation operator for FixedString and character.

  This operator creates a new FixedString object by concatenating a FixedString with a single character. The result will
  contain the characters from the left-hand side followed by the character from the right-hand side.

  \param lhs The left-hand side FixedString object.
  \param rhs The right-hand side character.

  \return A new FixedString object containing the concatenated result.

  \note Result size is sum of both input sizes, must not exceed allocated size.
*/
template <size_t allocatedSize>
[[nodiscard]] constexpr FixedString<allocatedSize> operator+(const FixedString<allocatedSize> & lhs, char rhs) noexcept;

/*!
  \brief Concatenation operator for character and FixedString.

  This operator creates a new FixedString object by concatenating a single character with a FixedString. The result will
  contain the character from the left-hand side followed by the characters from the right-hand side.

  \param lhs The character to prepend to the FixedString.
  \param rhs The right-hand side FixedString object.

  \return A new FixedString object containing the concatenated result.

  \note Result size is sum of both input sizes, must not exceed allocated size.
*/
template <size_t allocatedSize>
[[nodiscard]] constexpr FixedString<allocatedSize> operator+(char lhs, const FixedString<allocatedSize> & rhs) noexcept;

/*!
  \brief Equality comparison operator for two FixedString objects.

  This operator compares two FixedString objects for equality. The comparison is performed character by character.

  \tparam allocatedSize1 The size of the first FixedString's internal buffer.
  \tparam allocatedSize2 The size of the second FixedString's internal buffer.

  \param lhs The left-hand side FixedString object.
  \param rhs The right-hand side FixedString object.

  \return \c true if both strings have the same content, \c false otherwise.

  \note The comparison is case-sensitive.
  \note Empty strings are considered equal.

  \sa operator<=>(const FixedString<allocatedSize1> &, const FixedString<allocatedSize2> &)
*/
template <size_t allocatedSize1, size_t allocatedSize2>
[[nodiscard]] constexpr bool operator==(const FixedString<allocatedSize1> & lhs,
                                        const FixedString<allocatedSize2> & rhs) noexcept;

/*!
  \brief Equality comparison operator for FixedString and \ref toy::StringLike object.

  This operator compares a FixedString object with a \ref toy::StringLike object for equality.

  \tparam allocatedSize The size of the FixedString's internal buffer.
  \tparam stringType The type of the \ref toy::StringLike object. Must satisfy the \ref toy::StringLike concept.

  \param lhs The FixedString object.
  \param rhs The \ref toy::StringLike object.

  \return \c true if both strings have the same content, \c false otherwise.

  \note The comparison is case-sensitive.
  \note Empty strings are considered equal.

  \sa operator<=>(const FixedString<allocatedSize> &, const stringType &)
*/
template <size_t allocatedSize, StringLike stringType>
[[nodiscard]] constexpr bool operator==(const FixedString<allocatedSize> & lhs, const stringType & rhs) noexcept;

/*!
  \brief Equality comparison operator for \ref toy::StringLike object and FixedString.

  This operator compares a \ref toy::StringLike object with a FixedString object for equality.

  \tparam stringType The type of the \ref toy::StringLike object. Must satisfy the \ref toy::StringLike concept.
  \tparam allocatedSize The size of the FixedString's internal buffer.

  \param lhs The \ref toy::StringLike object.
  \param rhs The FixedString object.

  \return \c true if both strings have the same content, \c false otherwise.

  \note The comparison is case-sensitive.
  \note Empty strings are considered equal.

  \sa operator<=>(const stringType &, const FixedString<allocatedSize> &)
*/
template <size_t allocatedSize, StringLike stringType>
[[nodiscard]] constexpr bool operator==(const stringType & lhs, const FixedString<allocatedSize> & rhs) noexcept;

/*!
  \brief Equality comparison operator for FixedString and C string.

  This operator compares a FixedString object with a C string for equality.

  \tparam allocatedSize The size of the FixedString's internal buffer.

  \param lhs The FixedString object.
  \param rhs The C string.

  \return \c true if both strings have the same content, \c false otherwise.

  \pre The \a rhs pointer must not be null.

  \note The comparison is case-sensitive.
  \note Empty strings are considered equal.

  \sa operator<=>(const FixedString<allocatedSize> &, const char *)
*/
template <size_t allocatedSize>
[[nodiscard]] constexpr bool operator==(const FixedString<allocatedSize> & lhs, const char * rhs) noexcept;

/*!
  \brief Equality comparison operator for C string and FixedString.

  This operator compares a C string with a FixedString object for equality.

  \tparam allocatedSize The size of the FixedString's internal buffer.

  \param lhs The C string.
  \param rhs The FixedString object.

  \return \c true if both strings have the same content, \c false otherwise.

  \pre The \a lhs pointer must not be null.

  \note The comparison is case-sensitive.
  \note Empty strings are considered equal.

  \sa operator<=>(const char *, const FixedString<allocatedSize> &)
*/
template <size_t allocatedSize>
[[nodiscard]] constexpr bool operator==(const char * lhs, const FixedString<allocatedSize> & rhs) noexcept;

/*!
  \brief Three-way comparison operator for FixedString objects.

  This operator provides a three-way comparison between two FixedString objects. It returns a strong_ordering value that
  indicates the relationship between the strings.

  \tparam allocatedSize1 The size of the first FixedString's internal buffer.
  \tparam allocatedSize2 The size of the second FixedString's internal buffer.

  \param lhs The left-hand side FixedString object to compare.
  \param rhs The right-hand side FixedString object to compare.

  \return \c strong_ordering::less if \a lhs is lexicographically less than \a rhs, \c strong_ordering::equal if they
          are equal, or \c strong_ordering::greater if \a lhs is lexicographically greater than \a rhs.

  \note The comparison is case-sensitive.
  \note The comparison is performed lexicographically character by character.
  \note Empty strings are considered equal.

  \sa operator==(const FixedString<allocatedSize1> &, const FixedString<allocatedSize2> &)
*/
template <size_t allocatedSize1, size_t allocatedSize2>
[[nodiscard]] constexpr strong_ordering operator<=>(const FixedString<allocatedSize1> & lhs,
                                                    const FixedString<allocatedSize2> & rhs) noexcept;

/*!
  \brief Three-way comparison operator for FixedString and \ref toy::StringLike object.

  This operator provides a three-way comparison between a FixedString object and a \ref toy::StringLike object. It
  returns a strong_ordering value that indicates the relationship between the strings.

  \tparam allocatedSize The size of the FixedString's internal buffer.
  \tparam stringType The type of the \ref toy::StringLike object. Must satisfy the \ref toy::StringLike concept.

  \param lhs The FixedString object to compare.
  \param rhs The \ref toy::StringLike object to compare.

  \return \c strong_ordering::less if \a lhs is lexicographically less than \a rhs, \c strong_ordering::equal if they
          are equal, or \c strong_ordering::greater if \a lhs is lexicographically greater than \a rhs.

  \note The comparison is case-sensitive.
  \note The comparison is performed lexicographically character by character.
  \note Empty strings are considered equal.

  \sa operator==(const FixedString<allocatedSize> &, const stringType &)
*/
template <size_t allocatedSize, StringLike stringType>
[[nodiscard]] constexpr strong_ordering operator<=>(const FixedString<allocatedSize> & lhs,
                                                    const stringType & rhs) noexcept;

/*!
  \brief Three-way comparison operator for \ref toy::StringLike object and FixedString.

  This operator provides a three-way comparison between a \ref toy::StringLike object and a FixedString object. It
  returns a strong_ordering value that indicates the relationship between the strings.

  \tparam stringType The type of the \ref toy::StringLike object. Must satisfy the \ref toy::StringLike concept.
  \tparam allocatedSize The size of the FixedString's internal buffer.

  \param lhs The \ref toy::StringLike object to compare.
  \param rhs The FixedString object to compare.

  \return \c strong_ordering::less if \a lhs is lexicographically less than \a rhs, \c strong_ordering::equal if they
          are equal, or \c strong_ordering::greater if \a lhs is lexicographically greater than \a rhs.

  \note The comparison is case-sensitive.
  \note The comparison is performed lexicographically character by character.
  \note Empty strings are considered equal.

  \sa operator==(const stringType &, const FixedString<allocatedSize> &)
*/
template <size_t allocatedSize, StringLike stringType>
[[nodiscard]] constexpr strong_ordering operator<=>(const stringType & lhs,
                                                    const FixedString<allocatedSize> & rhs) noexcept;

/*!
  \brief Three-way comparison operator for FixedString and C string.

  This operator provides a three-way comparison between a FixedString object and a C string. It returns a
  strong_ordering value that indicates the relationship between the strings.

  \tparam allocatedSize The size of the FixedString's internal buffer.

  \param lhs The FixedString object to compare.
  \param rhs The C string to compare.

  \return \c strong_ordering::less if \a lhs is lexicographically less than \a rhs, \c strong_ordering::equal if they
          are equal, or \c strong_ordering::greater if \a lhs is lexicographically greater than \a rhs.

  \pre The \a rhs pointer must not be null.

  \note The comparison is case-sensitive.
  \note The comparison is performed lexicographically character by character.
  \note Empty strings are considered equal.

  \sa operator==(const FixedString<allocatedSize> &, const char *)
*/
template <size_t allocatedSize>
[[nodiscard]] constexpr strong_ordering operator<=>(const FixedString<allocatedSize> & lhs, const char * rhs) noexcept;

/*!
  \brief Three-way comparison operator for C string and FixedString.

  This operator provides a three-way comparison between a C string and a FixedString object. It returns a
  strong_ordering value that indicates the relationship between the strings.

  \tparam allocatedSize The size of the FixedString's internal buffer.

  \param lhs The C string to compare.
  \param rhs The FixedString object to compare.

  \return \c strong_ordering::less if \a lhs is lexicographically less than \a rhs, \c strong_ordering::equal if they
          are equal, or \c strong_ordering::greater if \a lhs is lexicographically greater than \a rhs.

  \pre The \a lhs pointer must not be null.

  \note The comparison is case-sensitive.
  \note The comparison is performed lexicographically character by character.
  \note Empty strings are considered equal.

  \sa operator==(const char *, const FixedString<allocatedSize> &)
*/
template <size_t allocatedSize>
[[nodiscard]] constexpr strong_ordering operator<=>(const char * lhs, const FixedString<allocatedSize> & rhs) noexcept;

} // namespace toy

#endif // INCLUDE_CORE_FIXED_STRING_HPP_
