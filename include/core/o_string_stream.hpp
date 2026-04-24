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
  \file   o_string_stream.hpp
  \brief  Allocator-free output string stream that appends into string-like storage.

  Defines \ref toy::OStringStream.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_O_STRING_STREAM_HPP_
#define INCLUDE_CORE_O_STRING_STREAM_HPP_

#include "string_like.hpp"

namespace toy {

/*!
  \concept OStringStreamBackend

  \ingroup StringStream

  \brief Concept satisfied when \a T can serve as the backing storage for \ref toy::OStringStream.

  Expresses the mutable string interface that \ref toy::OStringStream requires from its template argument. Types
  satisfying this concept support character-by-character append, block appends with and without a repetition count,
  null-terminated read access, and swap. \ref toy::FixedString is the canonical representative.

  \section requirements Requirements

  A type \a T satisfies OStringStreamBackend if and only if:
  - \a T is default-initializable.
  - \a T is assignable from \c const \c T \c &.
  - \c T::size() returns \c size_t.
  - \c T::c_str() returns a value convertible to \c const \c char*.
  - \c T::push_back(char) is a valid expression.
  - \c T::append(const char *) is a valid expression.
  - \c T::append(const char *, size_t) is a valid expression.
  - \c T::append(size_t, char) is a valid expression.
  - \a T is nothrow-swappable.

  \sa \ref toy::OStringStream
*/
template <typename T>
concept OStringStreamBackend
  = std::default_initializable<T> && std::assignable_from<T &, const T &> && std::is_nothrow_swappable_v<T>
    && requires(T & str, const T & constStr, const char * cstr, size_t count, char ch) {
         { constStr.size() } noexcept -> std::same_as<size_t>;
         { constStr.c_str() } noexcept -> std::convertible_to<const char *>;
         { str.push_back(ch) } noexcept;
         { str.append(cstr) } noexcept;
         { str.append(cstr, count) } noexcept;
         { str.append(count, ch) } noexcept;
       };

/*!
  \class OStringStream

  \ingroup StringStream

  \brief Output stream that appends formatted values into a \ref toy::OStringStreamBackend backend.

  OStringStream provides a \c std::ostringstream-like \c operator<< surface; allocation and capacity follow
  \a BackendType (e.g. \ref toy::FixedString). Integral, floating-point, boolean, pointer, character, C-string, and
  string-like inserts are supported where exposed by the template API.

  \tparam BackendType The type of the underlying storage. Must satisfy the \ref toy::OStringStreamBackend concept.

  \section features Key Features

  - **Storage policy**: No separate allocator; behavior and limits come from \a BackendType.
  - **constexpr**: Construction and many inserts are usable in constant evaluation when \a BackendType allows it.
  - **noexcept**: Members are \c noexcept; failed capacity is handled per \a BackendType (typically debug assertions).
  - **Floating-point precision**: \c precision() / \c setPrecision() affect subsequent floating inserts.

  \section usage Usage Example

  \code
  #include "core.hpp"

  toy::OStringStream<toy::FixedString<64>> stream;
  stream << "Value: " << 42 << ", Pi: " << 3.14159;
  stream << '!' << toy::CStringView(" ok: ") << true;
  const auto result = stream.str();
  (void)result;
  \endcode

  \section performance Performance Characteristics

  - **Construction / swap**: O(1) relative to string state.
  - **Append**: Depends on \a BackendType growth and content; each \c operator<< forwards to string append or formatting
    helpers.
  - **str()**: Returns \c const \a BackendType \c &; O(1) constant time (no copy).

  \section safety Safety Guarantees

  - **Bounds**: Overflow is governed by \a BackendType; violating capacity is undefined if assertions are disabled.
  - **Exception safety**: No exceptions; all operations are \c noexcept.

  \section compatibility Compatibility

  - **C++ standard**: C++20 or later (concepts, \c constexpr usage as implemented).
  - **Embedded**: Suitable when \a BackendType uses fixed storage (e.g. \ref toy::FixedString).

  \note Only end-of-string appends are supported; there is no seek or insert-at-offset API.
  \note The effective write position matches the end of the underlying string after each successful append.

  \sa \ref toy::OStringStreamBackend, \ref toy::StringLike
*/
template <OStringStreamBackend BackendType>
class OStringStream {
public:
  /// Type of characters stored in the string.
  using char_type = char;

  /// Type of positions in the string.
  using pos_type = size_t;

  /// Default constructor.
  constexpr OStringStream() noexcept = default;

  /*!
    \brief Constructs an OStringStream from a string-like object.

    This constructor initializes the stream by copying the content from a string-like object.

    \tparam SourceStringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string The source string-like object to copy content from.

    \pre The \a string must be valid and properly initialized.

    \post The stream contains a copy of the source string content.

    \note The constructor performs a deep copy of the string content.
    \note The source string type can be different from \a BackendType as long as both satisfy \ref toy::StringLike.
  */
  template <StringLike SourceStringType>
  explicit constexpr OStringStream(const SourceStringType & string) noexcept;

  /*!
    \brief Swaps the contents of this stream with another stream.

    This method exchanges the underlying string storage between this stream and the \a other stream. Both streams must
    have the same \a BackendType template parameter.

    \param other The stream to swap contents with.

    \post The contents of this stream and \a other are exchanged.
    \post Both streams remain valid after the swap operation.

    \note Self-swap is safe and has no effect.
    \note The operation is noexcept and provides strong exception guarantee.
    \note This method is useful for efficient content exchange without copying.
  */
  constexpr void swap(OStringStream & other) noexcept;

  /*!
    \brief Inserts a boolean value into the stream.

    This operator converts the boolean \a value to its string representation and appends it to the stream. The value
    \c true is converted to the string "true", and the value \c false is converted to the string "false".

    \param value The boolean value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \note This operator follows the same pattern as \c std::ostringstream::operator<<(bool).

    \post The write position is advanced by the length of the appended string (4 for "true", 5 for "false").

    \sa put(char_type)
    \sa write(const char_type *, size_t)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(bool value) noexcept;

  /*!
    \brief Inserts a signed long integer value into the stream.

    This operator converts the signed long integer \a value to its decimal string representation and appends it to the
    stream. Negative values are prefixed with a minus sign.

    \param value The signed long integer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as \c std::ostringstream::operator<<(long).

    \sa operator<<(unsigned long)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(long value) noexcept;

  /*!
    \brief Inserts an unsigned long integer value into the stream.

    This operator converts the unsigned long integer \a value to its decimal string representation and appends it to
    the stream.

    \param value The unsigned long integer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as \c std::ostringstream::operator<<(unsigned long).

    \sa operator<<(long)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(unsigned long value) noexcept;

  /*!
    \brief Inserts a signed long long integer value into the stream.

    This operator converts the signed long long integer \a value to its decimal string representation and appends it to
    the stream. Negative values are prefixed with a minus sign.

    \param value The signed long long integer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as \c std::ostringstream::operator<<(long long).

    \sa operator<<(unsigned long long)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(long long value) noexcept;

  /*!
    \brief Inserts an unsigned long long integer value into the stream.

    This operator converts the unsigned long long integer \a value to its decimal string representation and appends it
    to the stream.

    \param value The unsigned long long integer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as \c std::ostringstream::operator<<(unsigned long long).

    \sa operator<<(long long)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(unsigned long long value) noexcept;

  /*!
    \brief Inserts a double-precision floating-point value into the stream.

    This operator converts the double \a value to its decimal string representation with the current precision setting
    and appends it to the stream. The precision controls the number of digits displayed.

    \param value The double-precision floating-point value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as \c std::ostringstream::operator<<(double).
    \note The precision is controlled by setPrecision(size_t) and defaults to 6 digits.

    \sa precision() const
    \sa setPrecision(size_t)
    \sa operator<<(float)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(double value) noexcept;

  /*!
    \brief Inserts a pointer value into the stream.

    This operator converts the pointer \a value to its hexadecimal string representation with "0x" prefix and appends it
    to the stream. The value is zero-padded to the pointer width: 8 hex digits for 32-bit pointers, 16 for 64-bit,
    so the output length is fixed for a given platform (e.g. "0x00000000" or "0x0000000000000000").

    \param value The pointer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string (2 + sizeof(void*) * 2 characters).

    \note This operator follows the same pattern as \c std::ostringstream::operator<<(const void*).
    \note For null pointers, use operator<<(nullptr_t) for consistent "nullptr" output.

    \sa operator<<(nullptr_t)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(const void * value) noexcept;

  /*!
    \brief Inserts a null pointer literal into the stream.

    This operator inserts the string "nullptr" into the stream when a null pointer literal is provided. This provides
    a consistent and readable representation for null pointers.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by 7 characters (length of "nullptr").

    \note This operator follows the same pattern as \c std::ostringstream::operator<<(nullptr_t).
    \note The output is always the string "nullptr", regardless of the actual null pointer value.
    \note This operator is preferred over operator<<(const void*) for null pointer literals.

    \sa operator<<(const void *)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(nullptr_t) noexcept;

  /*!
    \brief Inserts a signed short integer value into the stream.

    This operator converts the signed short integer \a value to its decimal string representation and appends it to the
    stream. Negative values are prefixed with a minus sign.

    \param value The signed short integer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as \c std::ostringstream::operator<<(short).

    \sa operator<<(unsigned short)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(short value) noexcept;

  /*!
    \brief Inserts a signed integer value into the stream.

    This operator converts the signed integer \a value to its decimal string representation and appends it to the
    stream. Negative values are prefixed with a minus sign.

    \param value The signed integer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as \c std::ostringstream::operator<<(int).

    \sa operator<<(unsigned int)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(int value) noexcept;

  /*!
    \brief Inserts an unsigned short integer value into the stream.

    This operator converts the unsigned short integer \a value to its decimal string representation and appends it to
    the stream.

    \param value The unsigned short integer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as \c std::ostringstream::operator<<(unsigned short).

    \sa operator<<(short)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(unsigned short value) noexcept;

  /*!
    \brief Inserts an unsigned integer value into the stream.

    This operator converts the unsigned integer \a value to its decimal string representation and appends it to the
    stream.

    \param value The unsigned integer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as \c std::ostringstream::operator<<(unsigned int).

    \sa operator<<(int)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(unsigned int value) noexcept;

  /*!
    \brief Inserts a single-precision floating-point value into the stream.

    This operator converts the float \a value to its decimal string representation with the current precision setting
    and appends it to the stream. The precision controls the number of digits displayed.

    \param value The single-precision floating-point value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as \c std::ostringstream::operator<<(float).
    \note The precision is controlled by setPrecision() and defaults to 6 digits.

    \sa precision() const
    \sa setPrecision(size_t)
    \sa operator<<(double)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(float value) noexcept;

  /*!
    \brief Inserts a signed 8-bit integer value into the stream.

    This operator converts the signed 8-bit integer \a value to its decimal string representation and appends it to the
    stream. Negative values are prefixed with a minus sign.

    \param value The signed 8-bit integer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as \c std::ostringstream::operator<<(signed char).

    \sa operator<<(unsigned char)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(signed char value) noexcept;

  /*!
    \brief Inserts an unsigned 8-bit integer value into the stream.

    This operator converts the unsigned 8-bit integer \a value to its decimal string representation and appends it to
    the stream.

    \param value The unsigned 8-bit integer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as \c std::ostringstream::operator<<(unsigned char).

    \sa operator<<(signed char)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(unsigned char value) noexcept;

  /*!
    \brief Inserts a single character into the stream.

    This operator appends the specified \a value character to the end of the stream. The character is added directly
    without any conversion or formatting.

    \param value The character to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by one character.

    \note This operator follows the same pattern as \c std::ostringstream::operator<<(char_type).

    \sa operator<<(const char_type *)
    \sa operator<<(const SourceStringType &)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(char_type value) noexcept;

  /*!
    \brief Inserts a string-like object into the stream.

    This operator appends the content of a string-like object to the end of the stream. The string content is copied
    and appended to the current stream content.

    \tparam SourceStringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param value The string-like object to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \pre The \a value must be valid and properly initialized.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as \c std::ostringstream::operator<<(const std::string&).
    \note The source string type can be different from \a BackendType as long as both satisfy \ref toy::StringLike.
    \note This operator is useful for inserting string literals, \ref toy::FixedString objects, and other string-like
    types.

    \sa operator<<(const char_type *)
    \sa operator<<(char_type)
    \sa put(char_type)
    \sa write(const char_type *, size_t)
    \sa tellp()
  */
  template <StringLike SourceStringType>
  constexpr OStringStream & operator<<(const SourceStringType & value) noexcept;

  /*!
    \brief Inserts a C string into the stream.

    This operator appends the content of a C string to the end of the stream. The string is copied and appended to the
    current stream content.

    \param value The C string to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \pre The \a value must not be null.

    \post The write position is advanced by the length of the appended string (excluding the null terminator).

    \note This operator follows the same pattern as \c std::ostringstream::operator<<(const char*).
    \note The string is appended up to but not including the null terminator.
    \note This operator is useful for inserting C-style string literals and null-terminated character arrays.

    \sa operator<<(const SourceStringType &)
    \sa operator<<(char_type)
    \sa put(char_type)
    \sa write(const char_type *, size_t)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(const char_type * value) noexcept;

  /*!
    \brief Returns a const reference to the underlying string.

    This method provides read-only access to the internal string storage.

    \return A const reference to the underlying string.

    \note The returned reference is valid for the lifetime of the OStringStream object.

    \sa view()
  */
  [[nodiscard]] constexpr const BackendType & str() const noexcept;

  /*!
    \brief Sets the content of the stream from a string-like object.

    This method replaces the current content of the stream with the content from a string-like object.

    \tparam SourceStringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string The source string-like object to set the content from.

    \pre The \a string must be valid and properly initialized.

    \post The stream contains a copy of the source string content.
    \post The previous content of the stream is replaced.

    \note The method performs a deep copy of the string content.
    \note The source string type can be different from \a BackendType as long as both satisfy \ref toy::StringLike.
    \note This method provides a convenient way to update the stream content after construction.

    \sa str() const
  */
  template <StringLike SourceStringType>
  constexpr void str(const SourceStringType & string) noexcept;

  /*!
    \brief Returns a non-owning view of the underlying string.

    This method creates and returns a \ref toy::CStringView that provides read-only access to the underlying string
    storage without copying the content. The view is lightweight and does not own the string data.

    \return A \ref toy::CStringView representing the current content of the stream.

    \note The returned view is valid for the lifetime of the OStringStream object.
    \note The view does not own the string data and should not be used after the OStringStream is destroyed.
    \note This method is useful for passing the stream content to functions that accept \ref toy::CStringView without
          creating a copy.
    \note The view reflects the current state of the stream at the time of the call.

    \sa str() const
    \sa \ref toy::CStringView
  */
  [[nodiscard]] constexpr CStringView view() const noexcept;

  /*!
    \brief Appends a single character to the end of the stream.

    This method appends the specified \a character to the end of the underlying string storage. The character is added
    to the current content without replacing it.

    \param character The character to append to the stream.

    \return A reference to this OStringStream, allowing method chaining.

    \pre The \a character must not be the null character ('\0').

    \post The \a character is appended to the end of the stream content.
    \post The write position is advanced by one character.

    \sa write(const char_type *, size_t)
    \sa tellp()
  */
  constexpr OStringStream & put(char_type character) noexcept;

  /*!
    \brief Writes a specified number of characters from a buffer to the stream.

    This method appends \a count characters from the buffer pointed to by \a string to the end of the stream. The
    characters are copied directly from the buffer without requiring null termination.

    \param string The pointer to the character buffer to write from. Must not be null if \a count is greater than zero.
    \param count  The number of characters to write from the buffer.

    \return A reference to this OStringStream, allowing method chaining.

    \pre If \a count is greater than zero, \a string must not be null and must point to a valid buffer of at least
         \a count characters.
    \pre All characters in the buffer must not be null characters ('\0').

    \post \a count characters from the buffer are appended to the end of the stream content.
    \post The write position is advanced by \a count characters.

    \note This method is useful for writing binary data or partial strings that may not be null-terminated.
    \note If \a count is zero, the method returns without modifying the stream.

    \sa put(char_type), tellp()
  */
  constexpr OStringStream & write(const char_type * string, size_t count) noexcept;

  /*!
    \brief Returns the current write position in the stream.

    This method returns the position where the next write operation will append data. Since OStringStream only supports
    appending operations (all writes go to the end), the write position always equals the number of characters currently
    in the stream, which is the same as the size of the underlying string.

    \return The current write position, representing the index where the next character will be appended. This value is
            always equal to the number of characters written so far and to \c str().size().

    \note Since all operations append to the end, the write position is always equal to the stream size.

    \sa str() const, put(char_type), write(const char_type *, size_t)
  */
  [[nodiscard]] constexpr pos_type tellp() const noexcept;

  /*!
    \brief Returns the current floating-point precision setting.

    This method returns the precision value that will be used for formatting floating-point numbers when writing to the
    stream. The precision specifies the number of digits to display.

    \return The current precision value.

    \note The precision value affects how floating-point numbers are formatted when written to the stream.

    \sa setPrecision(size_t)
  */
  [[nodiscard]] constexpr size_t precision() const noexcept;

  /*!
    \brief Sets the floating-point precision and returns the previous value.

    This method sets the precision value that will be used for formatting floating-point numbers when writing to the
    stream. The precision specifies the number of digits to display.

    \param newPrecision The new precision value to set.

    \return The previous precision value before the change.

    \pre The \a newPrecision must be lower than or equal to the maximum supported digits for long double.

    \note The precision value affects how floating-point numbers are formatted when written to the stream.

    \sa precision() const
  */
  constexpr size_t setPrecision(size_t newPrecision) noexcept;

private:
  /// Internal string storage for the stream content.
  BackendType _string;

  /// Floating-point precision value used for number formatting.
  size_t _precision = 6;
};

} // namespace toy

#endif // INCLUDE_CORE_O_STRING_STREAM_HPP_
