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
  \file   c_string_view.hpp
  \brief  Non-owning string view class for C-style strings.
*/

#ifndef INCLUDE_CORE_C_STRING_VIEW_HPP_
#define INCLUDE_CORE_C_STRING_VIEW_HPP_

#include "string_like.hpp"

namespace toy {

/*!
  \class CStringView
  \ingroup String
  \brief Non-owning string view class for C-style strings.

  CStringView is a lightweight, non-owning wrapper around C-style strings that provides a safe and efficient way to work
  with null-terminated character arrays. It offers a std::string_view-like interface while being specifically designed
  for constexpr contexts and compile-time string manipulation.

  Unlike std::string_view, CStringView is optimized for constexpr usage and provides additional functionality for
  compile-time string operations. It does not own the underlying string data and simply holds a pointer to it, making
  it extremely lightweight and efficient.

  \section features Key Features

  - ⚙️ **Zero Allocation**: No dynamic memory allocation, only holds a pointer
  - 🔧 **ConstExpr Support**: Most operations can be evaluated at compile time
  - 🛡️ **Exception Safety**: All operations are noexcept
  - 🔗 **STL Compatibility**: Provides std::string_view/std::string like interface
  - 🌍 **UTF-8 Support**: Built-in UTF-8 character counting and manipulation
  - 🧬 **Type Safety**: Uses C++20 concepts for type safety
  - 📏 **Non-owning**: Does not manage memory, safe for temporary strings

  \section usage Usage Example

  \code
  #include "c_string_view.hpp"

  // Create a string view from a C string
  toy::CStringView str("Hello, World!");

  // Use in constexpr context
  constexpr auto greeting = toy::CStringView("Hello, World!");
  constexpr auto world = toy::CStringView("World");
  constexpr auto position = greeting.find(world);

  // Safe string operations
  if (str.starts_with("Hello")) {
    // Process the string
  }
  \endcode

  \section performance Performance Characteristics

  - ⚙️ **Construction**: O(1) - just stores a pointer
  - 📝 **Assignment**: O(1) - just copies a pointer
  - 📏 **Size/Length**: O(n) - requires traversing the string to find the null terminator
  - 🔍 **Search Operations**: O(n*m) where n is the string length and m is the pattern length
  - 💾 **Memory Usage**: Minimal - only stores a single pointer
  - ⚡ **Cache Performance**: Excellent due to pointer-only storage
  - 📋 **Copy Performance**: Extremely fast - just pointer copy
  - 🎯 **String Operations**: Optimized for common cases

  \section safety Safety Guarantees

  - 🛡️ **Contracts & Debug Checks**: Bounds are asserted in debug; in production, violating preconditions is UB
  - 🔒 **Null Pointer Safety**: All C-string operations validate pointers via assertions in debug
  - 📐 **Type Safety**: Concepts prevent invalid usage
  - ⚠️ **Exception Safety**: All operations are noexcept, no exceptions thrown
  - ⏰ **Lifetime Safety**: User must ensure underlying string remains valid

  \section compatibility Compatibility

  - 🆕 **C++23**: Requires C++23 or later for full functionality
  - 🔗 **STL Integration**: Compatible with STL algorithms and containers
  - 🌐 **Cross-Platform**: Works on all platforms supported by the compiler
  - 🔧 **Embedded Systems**: Suitable for resource-constrained environments

  \warning The underlying C string must remain valid for the lifetime of the CStringView object.
           CStringView does not own the string data and will not manage its lifetime.

  \sa std::string_view
  \sa toy::StringLike
  \sa toy::FixedString
*/
class CStringView {
public:
  /*!
    \brief Default constructor.

    Creates an empty string view that points to an empty string.

    \post The string view is empty and ready for use.
  */
  constexpr CStringView() noexcept;

  /*!
    \brief Constructs a copy of \a string view.

    This constructor initializes a string view by copying the C string from another \a string view.

    \param string The source string view to copy C string from.

    \pre The source \a string view must be valid and properly initialized.
  */
  constexpr CStringView(const CStringView & string) noexcept = default;

  /*!
    \brief Constructs a string view from a C \a string.

    Creates a new string view that wraps the provided C \a string pointer.

    \param string The C string to wrap.

    \pre The source C \a string must not be null.
  */
  constexpr explicit CStringView(const char * string) noexcept;

  /*!
    \brief Destructor for the string view.

    \note Since the CStringView does not manage dynamic memory, no special cleanup is required.
  */
  constexpr ~CStringView() noexcept = default;

  /*!
    \brief Copy assigns other \a string view to this string view.

    This operator assigns the C string of another \a string view to this string view.

    \param string The source string view to copy C string from.

    \return A reference to this string view after assignment.
  */
  constexpr CStringView & operator=(const CStringView & string) noexcept = default;

  /*!
    \brief Copy assigns the C \a string to this string view.

    This operator assigns a new C \a string to this string view.

    \param string The C string to assign.

    \return A reference to this string view after assignment.

    \pre The source C \a string must not be null.
  */
  constexpr CStringView & operator=(const char * string) noexcept;

  /*!
    \brief Copy assigns other \a string view to this string view.

    This method assigns the C string of another \a string view to this string view.

    \param string The source string view to copy C string from.

    \return A reference to this string view after assignment.

    \note This method is equivalent to the copy assignment operator.

    \sa operator=(const CStringView & string)
  */
  constexpr CStringView & assign(const CStringView & string) noexcept;

  /*!
    \brief Copy assigns the C \a string to this string view.

    This method assigns a new C \a string to this string view.

    \param string The C string to assign.

    \return A reference to this string view after assignment.

    \pre The source C \a string must not be null.

    \note This method is equivalent to the copy assignment operator.

    \sa operator=(const char * string)
  */
  constexpr CStringView & assign(const char * string) noexcept;

  /*!
    \brief Access a character in the string view at a given \a offset.

    This method provides read-only access to a character at the specified \a offset within the string view.

    \param offset The offset of the character to access in the string view.

    \return A const reference to the character at the specified \a offset.

    \pre The \a offset must be less than the current string view size.

    \note The returned reference is read-only and cannot modify the character.
    \note This method is equivalent to the subscript operator.

    \sa operator[](size_t offset) const
  */
  [[nodiscard]] constexpr const char & at(size_t offset) const noexcept;

  /*!
    \brief Access a character in the string view at a given \a offset.

    This operator provides read-only access to a character at the specified \a offset within the string view.

    \param offset The offset of the character to access in the string view.

    \return A const reference to the character at the specified \a offset.

    \pre The \a offset must be less than the current string view size.

    \note The returned reference is read-only and cannot modify the character.
  */
  [[nodiscard]] constexpr const char & operator[](size_t offset) const noexcept;

  /*!
    \brief Returns a const reference to the first character of the string view.

    This method provides read-only access to the first character of the string view.

    \return A const reference to the first character of the string view.

    \pre The string view must not be empty.

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

    \note The returned pointer points to a C string.
    \note The returned pointer is read-only and cannot modify the string view contents.
    \note This method is equivalent to data().

    \sa data()
  */
  [[nodiscard]] constexpr const char * c_str() const noexcept;

  /*!
    \brief Checks if the string view is empty.

    This method checks if the string view is empty, i.e. its size is zero. An empty string view contains no characters
    and has a length of zero.

    \return \c true if the string view is empty, \c false otherwise.

    \note An empty string view has size zero.
    \note An empty string view still contains a null terminator.
    \note This method is equivalent to the expression: `size() == 0`.

    \sa size()
  */
  [[nodiscard]] constexpr bool empty() const noexcept;

  /*!
    \brief Returns the size of the string view.

    This method returns the current number of characters in the string view, excluding the terminating null character.
    The size represents the actual length of the string view content.

    \return The number of characters in the string view, excluding the terminating null character.

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

    \return The number of Unicode characters in the string view, excluding the terminating null character.

    \note For ASCII strings, utf8_size() equals size().
    \note For UTF-8 strings, utf8_size() may be less than size().
    \note Invalid UTF-8 sequences are handled gracefully and may affect the count.
    \note This method is useful for internationalization and text processing.
  */
  [[nodiscard]] size_t utf8_size() const noexcept;

  /*!
    \brief Returns the size of the string view.

    This method returns the current number of characters in the string view, excluding the terminating null character.
    The length represents the actual length of the string view content.

    \return The number of characters in the string view, excluding the terminating null character.

    \note This method is equivalent to size().

    \sa size()
  */
  [[nodiscard]] constexpr size_t length() const noexcept;

  /*!
    \brief Returns the maximum observable size of this string view.

    This method returns the maximum possible size for the string view. The maximum size represents the size of the
    allocated buffer.

    \return The maximum number of characters in the string view, excluding the terminating null character.

    \note CStringView is non-owning and has no capacity; this is an alias of size().
  */
  [[nodiscard]] constexpr size_t max_size() const noexcept;

  /*!
    \brief Returns the capacity of the string view.

    This method returns the capacity of the allocated buffer for the string view.

    \return The capacity of the string view in characters, excluding the terminating null character.

    \note CStringView is non-owning and has no capacity; this is an alias of size().
  */
  [[nodiscard]] constexpr size_t capacity() const noexcept;

  /*!
    \brief Clears the contents by detaching from the current C string.

    This method resets this view to point at the internal empty sentinel. The previously referenced character buffer is
    not modified or freed.

    \post The string view is empty (size is \c 0).
    \post The string view points to an empty sentinel string.

    \note No allocation or deallocation occurs.
  */
  constexpr void clear() noexcept;

  /*!
    \brief Swaps the contents of this string view with another \a string view.

    This method exchanges the contents of this string view with another \a string view of the same type. The operation
    is performed efficiently by swapping the internal data information.

    \param string The string view to swap contents with.

    \post This string view contains the contents that were in other \a string.
    \post The other \a string view contains the contents that were in this string.

    \note Self-swap is handled correctly and safely (no-op).
  */
  constexpr void swap(CStringView & string) noexcept;

  /*!
    \brief Finds the first occurrence of a \ref toy::StringLike object in the string view.

    This method searches for the first occurrence of a \ref toy::StringLike object within this string view, starting
    from the given \a position.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string   The source \ref toy::StringLike object to search for.
    \param position The position to start searching from (default: \c 0).

    \return The position of the first occurrence of a \ref toy::StringLike object, or \ref npos if not found.

    \pre The \a position must be less than the string view size.

    \note The search is case-sensitive.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr size_t find(const stringType & string, size_t position = 0) const noexcept;

  /*!
    \brief Finds the first occurrence of the C \a string in the string view.

    This method searches for the first occurrence of the C \a string within this string view, starting from the given \a
    position.

    \param string   The source C string to search for.
    \param position The position to start searching from (default: \c 0).

    \return The position of the first occurrence of the C \a string, or \ref npos if not found.

    \pre The \a position must be less than the string view size.
    \pre The \a string must not be null.

    \note The search is case-sensitive.
  */
  [[nodiscard]] constexpr size_t find(const char * string, size_t position = 0) const noexcept;

  /*!
    \brief Finds the first occurrence of a \a character in the string view.

    This method searches for the first occurrence of the specified \a character within this string view, starting from
    the given \a position.

    \param character The character to search for.
    \param position  The position to start searching from (default: \c 0).

    \return The position of the first occurrence of the \a character, or \ref npos if not found.

    \pre The \a position must be less than the string view size.

    \note The search is case-sensitive.
  */
  [[nodiscard]] constexpr size_t find(char character, size_t position = 0) const noexcept;

  /*!
    \brief Finds the last occurrence of a \ref toy::StringLike object in the string view.

    This method searches for the last occurrence of a \ref toy::StringLike object within this string view, starting from
    the given \a position and searching backwards.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string   The source \ref toy::StringLike object to search for.
    \param position The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of a \ref toy::StringLike object, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than or equal to the maximum valid start index: the string
    view size minus the size of a \ref toy::StringLike object.

    \note The search is case-sensitive.
    \note If a \ref toy::StringLike object is empty, returns \a position if within bounds, otherwise returns the string
    view size.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr size_t rfind(const stringType & string, size_t position = npos) const noexcept;

  /*!
    \brief Finds the last occurrence of the C \a string in the string view.

    This method searches for the last occurrence of the C \a string within this string view, starting from the given \a
    position and searching backwards.

    \param string   The source C string to search for.
    \param position The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of the C \a string, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than or equal to the maximum valid start index: the string
         view size minus the size of the C \a string.
    \pre The \a string must not be null.

    \note The search is case-sensitive.
    \note If the C \a string is empty, returns \a position if within bounds, otherwise returns the string view size.
  */
  [[nodiscard]] constexpr size_t rfind(const char * string, size_t position = npos) const noexcept;

  /*!
    \brief Finds the last occurrence of a \a character in the string view.

    This method searches for the last occurrence of the specified \a character within this string view, starting from
    the given \a position and searching backwards.

    \param character The character to search for.
    \param position  The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of the \a character, or \ref npos if not found.

    \pre The \a position must be less than the string view size.

    \note The search is case-sensitive.
  */
  [[nodiscard]] constexpr size_t rfind(char character, size_t position = npos) const noexcept;

  /// The special value, its exact meaning depends on the context
  static constexpr size_t npos = size_t(-1);

  /*!
    \brief Finds the first occurrence of any character from a \ref toy::StringLike object.

    This method searches for the first occurrence of any character from a \ref toy::StringLike object within this string
    view, starting from the given \a position.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string   The \ref toy::StringLike object containing characters to search for.
    \param position The position to start searching from (default: \c 0).

    \return The position of the first occurrence of any character from a \ref toy::StringLike object, or \ref npos if
            not found.

    \pre The \a position must be less than the string view size.

    \note The search is case-sensitive.
    \note If a \ref toy::StringLike object is empty, this method returns \ref npos.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr size_t find_first_of(const stringType & string, size_t position = 0) const noexcept;

  /*!
    \brief Finds the first occurrence of any character from the C \a string.

    This method searches for the first occurrence of any character from the C \a string within this string view,
    starting from the given \a position.

    \param string   The C string containing characters to search for.
    \param position The position to start searching from (default: \c 0).

    \return The position of the first occurrence of any character from the C \a string, or \ref npos if not found.

    \pre The \a position must be less than the string view size.
    \pre The \a string must not be null.

    \note The search is case-sensitive.
    \note If the C \a string is empty, this method returns \ref npos.
  */
  [[nodiscard]] constexpr size_t find_first_of(const char * string, size_t position = 0) const noexcept;

  /*!
    \brief Finds the first occurrence of the specified \a character.

    This method searches for the first occurrence of the specified \a character within this string view, starting from
    the given \a position.

    \param character The character to search for.
    \param position  The position to start searching from (default: \c 0).

    \return The position of the first occurrence of the \a character, or \ref npos if not found.

    \pre The \a position must be less than the string view size.

    \note The search is case-sensitive.
    \note This method is equivalent to find(char, size_t) const.

    \sa find(char, size_t) const
  */
  [[nodiscard]] constexpr size_t find_first_of(char character, size_t position = 0) const noexcept;

  /*!
    \brief Finds the first occurrence of any character not from a \ref toy::StringLike object.

    This method searches for the first occurrence of any character that is not present in a \ref toy::StringLike object
    within this string view, starting from the given \a position.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string   The \ref toy::StringLike object containing characters to exclude from search.
    \param position The position to start searching from (default: \c 0).

    \return The position of the first occurrence of any character not from a \ref toy::StringLike object, or \ref npos
            if not found.

    \pre The \a position must be less than the string view size.

    \note The search is case-sensitive.
    \note If a \ref toy::StringLike object is empty, returns \a position if within bounds, otherwise returns \ref npos.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr size_t find_first_not_of(const stringType & string, size_t position = 0) const noexcept;

  /*!
    \brief Finds the first occurrence of any character not from the C \a string.

    This method searches for the first occurrence of any character that is not present in the C \a string within this
    string view, starting from the given \a position.

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
    within this string view, starting from the given \a position.

    \param character The character to exclude from search.
    \param position  The position to start searching from (default: \c 0).

    \return The position of the first occurrence of any character not equal to \a character, or \ref npos if not found.

    \pre The \a position must be less than the string view size.

    \note The search is case-sensitive.
  */
  [[nodiscard]] constexpr size_t find_first_not_of(char character, size_t position = 0) const noexcept;

  /*!
    \brief Finds the last occurrence of any character from a \ref toy::StringLike object.

    This method searches for the last occurrence of any character from a \ref toy::StringLike object within this string
    view, starting from the given \a position and searching backwards.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string   The \ref toy::StringLike object containing characters to search for.
    \param position The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of any character from a \ref toy::StringLike object, or \ref npos if not
            found.

    \pre If \a position is not \ref npos, it must be less than the string view size.

    \note The search is case-sensitive.
    \note If a \ref toy::StringLike object is empty, this method returns \ref npos.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr size_t find_last_of(const stringType & string, size_t position = npos) const noexcept;

  /*!
    \brief Finds the last occurrence of any character from the C \a string.

    This method searches for the last occurrence of any character from the C \a string within this string view, starting
    from the given \a position and searching backwards.

    \param string   The C string containing characters to search for.
    \param position The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of any character from the C \a string, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than the string view size.
    \pre The \a string must not be null.

    \note The search is case-sensitive.
    \note If the C \a string is empty, this method returns \ref npos.
  */
  [[nodiscard]] constexpr size_t find_last_of(const char * string, size_t position = npos) const noexcept;

  /*!
    \brief Finds the last occurrence of the specified \a character.

    This method searches for the last occurrence of the specified \a character within this string view, starting from
    the given \a position and searching backwards.

    \param character The character to search for.
    \param position  The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of the \a character, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than the string view size.

    \note The search is case-sensitive.
    \note This method is equivalent to rfind(char, size_t) const.

    \sa rfind(char, size_t) const
  */
  [[nodiscard]] constexpr size_t find_last_of(char character, size_t position = npos) const noexcept;

  /*!
    \brief Finds the last occurrence of any character not from a \ref toy::StringLike object.

    This method searches for the last occurrence of any character that is not present in a \ref toy::StringLike object
    within this string view, starting from the given \a position and searching backwards.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string   The \ref toy::StringLike object containing characters to exclude from search.
    \param position The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of any character not from a \ref toy::StringLike object, or \ref npos if
            not found.

    \pre If \a position is not \ref npos, it must be less than the string view size.

    \note The search is case-sensitive.
    \note If a \ref toy::StringLike object is empty, returns \a position if within bounds, otherwise returns \ref npos.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr size_t find_last_not_of(const stringType & string, size_t position = npos) const noexcept;

  /*!
    \brief Finds the last occurrence of any character not from the C \a string.

    This method searches for the last occurrence of any character that is not present in the C \a string within this
    string view, starting from the given \a position and searching backwards.

    \param string   The C string containing characters to exclude from search.
    \param position The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of any character not from the C \a string, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than the string view size.
    \pre The \a string must not be null.

    \note The search is case-sensitive.
    \note If the C \a string is empty, returns \a position if within bounds, otherwise returns \ref npos.
  */
  [[nodiscard]] constexpr size_t find_last_not_of(const char * string, size_t position = npos) const noexcept;

  /*!
    \brief Finds the last occurrence of any character not equal to the specified \a character.

    This method searches for the last occurrence of any character that is not equal to the specified \a character within
    this string view, starting from the given \a position and searching backwards.

    \param character The character to exclude from search.
    \param position  The position to start searching from (default: \ref npos). If \ref npos, searches from the end.

    \return The position of the last occurrence of any character not equal to \a character, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than the string view size.

    \note The search is case-sensitive.
  */
  [[nodiscard]] constexpr size_t find_last_not_of(char character, size_t position = npos) const noexcept;

  /*!
    \brief Compares this string view with a \ref toy::StringLike object lexicographically.

    This method performs a lexicographic comparison between this string view and a \ref toy::StringLike object. The
    comparison is performed character by character using the character's numeric value.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string The \ref toy::StringLike object to compare with this string.

    \return A negative value if this string view is lexicographically less than a \ref toy::StringLike object, zero if
            they are equal, or a positive value if this string view is lexicographically greater than a
            \ref toy::StringLike object.

    \note The comparison is case-sensitive.
    \note The comparison stops at the first character that differs between the strings.
    \note If one string is a prefix of another, the shorter is considered lexicographically smaller.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr int compare(const stringType & string) const noexcept;

  /*!
    \brief Compares this string view with the C \a string lexicographically.

    This method performs a lexicographic comparison between this string view and the C \a string. The comparison is
    performed character by character using the character's numeric value.

    \param string The C string to compare with this string.

    \return A negative value if this string view is lexicographically less than C \a string, zero if they are equal, or
    a positive value if this string view is lexicographically greater than C \a string.

    \pre The C \a string must not be null.

    \note The comparison is case-sensitive.
    \note The comparison stops at the first character that differs between the strings.
    \note If one string is a prefix of another, the shorter is considered lexicographically smaller.
  */
  [[nodiscard]] constexpr int compare(const char * string) const noexcept;

  /*!
    \brief Checks if the string view starts with a \ref toy::StringLike object.

    This method checks if the current string view starts with a \ref toy::StringLike object. The comparison is performed
    character by character from the beginning of the string view.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string The \ref toy::StringLike object to check if this string starts with.

    \return \c true if this string view starts with a \ref toy::StringLike object, \c false otherwise.

    \note The comparison is case-sensitive.
    \note If a \ref toy::StringLike object is empty, this method returns \c true.
    \note If a \ref toy::StringLike object is longer than this string view, returns \c false.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr bool starts_with(const stringType & string) const noexcept;

  /*!
    \brief Checks if the string view starts with the specified C \a string.

    This method checks if the current string view starts with the C \a string. The comparison is performed character by
    character from the beginning of the string view.

    \param string The C string to check if this string starts with.

    \return \c true if this string view starts with the C \a string, \c false otherwise.

    \pre The C \a string must not be null.

    \note The comparison is case-sensitive.
    \note If the C \a string is empty, this method returns \c true.
    \note If the C \a string is longer than this string view, returns \c false.
  */
  [[nodiscard]] constexpr bool starts_with(const char * string) const noexcept;

  /*!
    \brief Checks if the string view starts with the specified \a character.

    This method checks if the current string view starts with the specified \a character. The comparison is performed on
    the first character of the string view.

    \param character The character to check if this string view starts with.

    \return \c true if this string view starts with the specified \a character, \c false otherwise.

    \note The comparison is case-sensitive.
    \note If the string view is empty, this method returns \c false.
  */
  [[nodiscard]] constexpr bool starts_with(char character) const noexcept;

  /*!
    \brief Checks if the string view ends with a \ref toy::StringLike object.

    This method checks if the current string view ends with a \ref toy::StringLike object. The comparison is performed
    character by character from the end of the string view.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string The \ref toy::StringLike object to check if this string view ends with.

    \return \c true if this string view ends with a \ref toy::StringLike object, \c false otherwise.

    \note The comparison is case-sensitive.
    \note If a \ref toy::StringLike object is empty, this method returns \c true.
    \note If a \ref toy::StringLike object is longer than this string view, returns \c false.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr bool ends_with(const stringType & string) const noexcept;

  /*!
    \brief Checks if the string view ends with the C \a string.

    This method checks if the current string view ends with the C \a string. The comparison is performed character by
    character from the end of the string view.

    \param string The C string to check if this string view ends with.

    \return \c true if this string view ends with the C \a string, \c false otherwise.

    \pre The C \a string must not be null.

    \note The comparison is case-sensitive.
    \note If the C \a string is empty, this method returns \c true.
    \note If the C \a string is longer than this string view, returns \c false.
  */
  [[nodiscard]] constexpr bool ends_with(const char * string) const noexcept;

  /*!
    \brief Checks if the string view ends with the specified \a character.

    This method checks if the current string view ends with the specified \a character. The comparison is performed on
    the last character of the string view.

    \param character The character to check if this string view ends with.

    \return \c true if this string view ends with the specified \a character, \c false otherwise.

    \note The comparison is case-sensitive.
    \note If the string view is empty, this method returns \c false.
  */
  [[nodiscard]] constexpr bool ends_with(char character) const noexcept;

  /*!
    \brief Checks if the string view contains a \ref toy::StringLike object.

    This method checks if the current string view contains a \ref toy::StringLike object anywhere within it.

    \tparam stringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string The \ref toy::StringLike object to search for within this string.

    \return \c true if this string view contains a \ref toy::StringLike object, \c false otherwise.

    \note The search is case-sensitive.
    \note If a \ref toy::StringLike object is empty, this method returns \c true.
    \note If a \ref toy::StringLike object is longer than this string view, returns \c false.
  */
  template <StringLike stringType>
  [[nodiscard]] constexpr bool contains(const stringType & string) const noexcept;

  /*!
    \brief Checks if the string view contains the C \a string.

    This method checks if the current string view contains the C \a string anywhere within it.

    \param string The C string to search for within this string.

    \return \c true if this string view contains the C \a string, \c false otherwise.

    \pre The C \a string must not be null.

    \note The search is case-sensitive.
    \note If the C \a string is empty, this method returns \c true.
    \note If the C \a string is longer than this string view, returns \c false.
  */
  [[nodiscard]] constexpr bool contains(const char * string) const noexcept;

  /*!
    \brief Checks if the string view contains the specified \a character.

    This method checks if the current string view contains the specified \a character anywhere within it.

    \param character The character to search for within this string view.

    \return \c true if this string view contains the specified \a character, \c false otherwise.

    \note The search is case-sensitive.
    \note If the string view is empty, this method returns \c false.
  */
  [[nodiscard]] constexpr bool contains(char character) const noexcept;

  /*!
    \brief Equality comparison operator for two CStringView objects.

    This operator compares two CStringView objects for equality. The comparison is performed character by character.

    \param lhs The left-hand side CStringView object.
    \param rhs The right-hand side CStringView object.

    \return \c true if both strings view have the same content, \c false otherwise.

    \note The comparison is case-sensitive.
    \note Empty string views are considered equal.

    \sa operator<=>(const CStringView &, const CStringView &)
  */
  friend constexpr bool operator==(const CStringView & lhs, const CStringView & rhs) noexcept;

  /*!
    \brief Equality comparison operator for CStringView and \ref toy::StringLike object.

    This operator compares a CStringView object with a \ref toy::StringLike object for equality.

    \tparam stringType The type of the \ref toy::StringLike object. Must satisfy the \ref toy::StringLike concept.

    \param lhs The CStringView object.
    \param rhs The \ref toy::StringLike object.

    \return \c true if both strings have the same content, \c false otherwise.

    \note The comparison is case-sensitive.
    \note Empty strings are considered equal.

    \sa operator<=>(const CStringView &, const stringType &)
  */
  template <StringLike stringType>
  friend constexpr bool operator==(const CStringView & lhs, const stringType & rhs) noexcept;

  /*!
    \brief Equality comparison operator for \ref toy::StringLike object and CStringView.

    This operator compares a \ref toy::StringLike object with a CStringView object for equality.

    \tparam stringType The type of the \ref toy::StringLike object. Must satisfy the \ref toy::StringLike concept.

    \param lhs The \ref toy::StringLike object.
    \param rhs The CStringView object.

    \return \c true if both strings have the same content, \c false otherwise.

    \note The comparison is case-sensitive.
    \note Empty strings are considered equal.

    \sa operator<=>(const stringType &, const CStringView &)
  */
  template <StringLike stringType>
  friend constexpr bool operator==(const stringType & lhs, const CStringView & rhs) noexcept;

  /*!
    \brief Equality comparison operator for CStringView and C string.

    This operator compares a CStringView object with a C string for equality.

    \param lhs The CStringView object.
    \param rhs The C string.

    \return \c true if both strings have the same content, \c false otherwise.

    \pre The \a rhs pointer must not be null.

    \note The comparison is case-sensitive.
    \note Empty strings are considered equal.

    \sa operator<=>(const CStringView &, const char *)
  */
  friend constexpr bool operator==(const CStringView & lhs, const char * rhs) noexcept;

  /*!
    \brief Equality comparison operator for C string and CStringView.

    This operator compares a C string with a CStringView object for equality.

    \param lhs The C string.
    \param rhs The CStringView object.

    \return \c true if both strings have the same content, \c false otherwise.

    \pre The \a lhs pointer must not be null.

    \note The comparison is case-sensitive.
    \note Empty strings are considered equal.

    \sa operator<=>(const char *, const CStringView &)
  */
  friend constexpr bool operator==(const char * lhs, const CStringView & rhs) noexcept;

  /*!
    \brief Three-way comparison operator for two CStringView objects.

    This operator provides a three-way comparison between two CStringView objects. It returns a strong_ordering value
    that indicates the relationship between the string views.

    \param lhs The left-hand side CStringView object to compare.
    \param rhs The right-hand side CStringView object to compare.

    \return \c strong_ordering::less if \a lhs is lexicographically less than \a rhs, \c strong_ordering::equal if they
            are equal, or \c strong_ordering::greater if \a lhs is lexicographically greater than \a rhs.

    \note The comparison is case-sensitive.
    \note The comparison is performed lexicographically character by character.
    \note Empty string views are considered equal.

    \sa operator==(const CStringView &, const CStringView &)
  */
  friend constexpr strong_ordering operator<=>(const CStringView & lhs, const CStringView & rhs) noexcept;

  /*!
    \brief Three-way comparison operator for CStringView and \ref toy::StringLike object.

    This operator provides a three-way comparison between a CStringView object and a \ref toy::StringLike object. It
    returns a strong_ordering value that indicates the relationship between the strings.

    \tparam stringType The type of the \ref toy::StringLike object. Must satisfy the \ref toy::StringLike concept.

    \param lhs The CStringView object to compare.
    \param rhs The \ref toy::StringLike object to compare.

    \return \c strong_ordering::less if \a lhs is lexicographically less than \a rhs, \c strong_ordering::equal if they
            are equal, or \c strong_ordering::greater if \a lhs is lexicographically greater than \a rhs.

    \note The comparison is case-sensitive.
    \note The comparison is performed lexicographically character by character.
    \note Empty strings are considered equal.

    \sa operator==(const CStringView &, const stringType &)
  */
  template <StringLike stringType>
  friend constexpr strong_ordering operator<=>(const CStringView & lhs, const stringType & rhs) noexcept;

  /*!
    \brief Three-way comparison operator for \ref toy::StringLike object and CStringView.

    This operator provides a three-way comparison between a \ref toy::StringLike object and a CStringView object. It
    returns a strong_ordering value that indicates the relationship between the strings.

    \tparam stringType The type of the \ref toy::StringLike object. Must satisfy the \ref toy::StringLike concept.

    \param lhs The \ref toy::StringLike object to compare.
    \param rhs The CStringView object to compare.

    \return \c strong_ordering::less if \a lhs is lexicographically less than \a rhs, \c strong_ordering::equal if they
            are equal, or \c strong_ordering::greater if \a lhs is lexicographically greater than \a rhs.

    \note The comparison is case-sensitive.
    \note The comparison is performed lexicographically character by character.
    \note Empty strings are considered equal.

    \sa operator==(const stringType &, const CStringView &)
  */
  template <StringLike stringType>
  friend constexpr strong_ordering operator<=>(const stringType & lhs, const CStringView & rhs) noexcept;

  /*!
    \brief Three-way comparison operator for CStringView and C string.

    This operator provides a three-way comparison between a CStringView object and a C string. It returns a
    strong_ordering value that indicates the relationship between the strings.

    \param lhs The CStringView object to compare.
    \param rhs The C string to compare.

    \return \c strong_ordering::less if \a lhs is lexicographically less than \a rhs, \c strong_ordering::equal if they
            are equal, or \c strong_ordering::greater if \a lhs is lexicographically greater than \a rhs.

    \pre The \a rhs pointer must not be null.

    \note The comparison is case-sensitive.
    \note The comparison is performed lexicographically character by character.
    \note Empty strings are considered equal.

    \sa operator==(const CStringView &, const char *)
  */
  friend constexpr strong_ordering operator<=>(const CStringView & lhs, const char * rhs) noexcept;

  /*!
    \brief Three-way comparison operator for C string and CStringView.

    This operator provides a three-way comparison between a C string and a CStringView object. It returns a
    strong_ordering value that indicates the relationship between the strings.

    \param lhs The C string to compare.
    \param rhs The CStringView object to compare.

    \return \c strong_ordering::less if \a lhs is lexicographically less than \a rhs, \c strong_ordering::equal if they
            are equal, or \c strong_ordering::greater if \a lhs is lexicographically greater than \a rhs.

    \pre The \a lhs pointer must not be null.

    \note The comparison is case-sensitive.
    \note The comparison is performed lexicographically character by character.
    \note Empty strings are considered equal.

    \sa operator==(const char *, const CStringView &)
  */
  friend constexpr strong_ordering operator<=>(const char * lhs, const CStringView & rhs) noexcept;

private:
  /// Pointer to the wrapped C string
  const char * _data;

  /// Static empty C string used as default value for null pointers
  static constexpr char _emptyString[]{'\0'};

  /*!
    \brief Helper method for finding \a data in the string view.

    This private method performs the common search logic used by all find methods. It searches for the specified \a data
    starting from the given \a position.

    \param position The position to start searching from.
    \param data     The data to search for.
    \param dataSize The size of the data to search for.

    \return The position of the first occurrence of \a data, or \ref npos if not found.

    \pre The \a position must be less than the string view size.
    \pre The \a data must not be null.
  */
  constexpr size_t _find_raw(size_t position, const char * data, size_t dataSize) const noexcept;

  /*!
    \brief Helper method for finding \a data in the string view backwards.

    This private method performs the common reverse search logic used by all rfind methods. It searches for the
    specified \a data starting from the given \a position and searching backwards.

    \param position The position to start searching from.
    \param data     The data to search for.
    \param dataSize The size of the data to search for.

    \return The position of the last occurrence of \a data, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than or equal to the string view size.
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

    \pre The \a position must be less than the string view size.
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

    \pre The \a position must be less than the string view size.
    \pre The \a data must not be null.
  */
  constexpr size_t _find_first_not_of_raw(size_t position, const char * data, size_t dataSize) const noexcept;

  /*!
    \brief Helper method for finding the last occurrence of any character from \a data.

    This private method performs the common reverse search logic used by all find_last_of methods. It searches for the
    last occurrence of any character from the specified \a data starting from the given \a position and searching
    backwards.

    \param position The position to start searching from (default: \ref npos). If \ref npos, searches from the end.
    \param data     The data containing characters to search for.
    \param dataSize The size of the data containing characters to search for.

    \return The position of the last occurrence of any character from \a data, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than the string view size.
    \pre The \a data must not be null.
  */
  constexpr size_t _find_last_of_raw(size_t position, const char * data, size_t dataSize) const noexcept;

  /*!
    \brief Helper method for finding the last occurrence of any character not from \a data.

    This private method performs the common reverse search logic used by all find_last_not_of methods. It searches for
    the last occurrence of any character that is not present in the specified \a data starting from the given \a
    position and searching backwards.

    \param position The position to start searching from (default: \ref npos). If \ref npos, searches from the end.
    \param data     The data containing characters to exclude from search.
    \param dataSize The size of the data containing characters to exclude from search.

    \return The position of the last occurrence of any character not from \a data, or \ref npos if not found.

    \pre If \a position is not \ref npos, it must be less than the string view size.
    \pre The \a data must not be null.
  */
  constexpr size_t _find_last_not_of_raw(size_t position, const char * data, size_t dataSize) const noexcept;
};

} // namespace toy

#endif // INCLUDE_CORE_C_STRING_VIEW_HPP_
