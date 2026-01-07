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
  \brief  Output string stream class for building strings from various data types.
*/

#ifndef INCLUDE_CORE_O_STRING_STREAM_HPP_
#define INCLUDE_CORE_O_STRING_STREAM_HPP_

#include "string_like.hpp"

namespace toy {

template <typename StringType>
class OStringStream {
public:
  using char_type = char; //!< Type of characters stored in the string.

  using pos_type = size_t; //!< Type of positions in the string.

  /*!
    \brief Default constructor.

    Constructs an empty OStringStream with default-initialized string storage.

    \post The stream contains an empty string.
  */
  constexpr OStringStream() noexcept = default;

  /*!
    \brief Constructs an OStringStream from a string-like object.

    This constructor initializes the stream by copying the content from a string-like object.

    \tparam SourceStringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string The source string-like object to copy content from.

    \pre The \a string must be valid and properly initialized.

    \post The stream contains a copy of the source string content.

    \note The constructor performs a deep copy of the string content.
    \note The source string type can be different from StringType as long as both satisfy \ref toy::StringLike.
  */
  template <StringLike SourceStringType>
  explicit constexpr OStringStream(const SourceStringType & string) noexcept;

  /*!
    \brief Copy assignment operator.

    Assigns the contents of another OStringStream to this stream. The assignment performs a deep copy of the underlying
    string.

    \param other The source OStringStream to copy from.

    \return A reference to this OStringStream after assignment.

    \pre The \a other stream must be valid and properly initialized.

    \post This stream contains the same content as the source \a other stream.
    \post The underlying string is a copy of the source string.

    \note Self-assignment is handled correctly and safely.
  */
  constexpr OStringStream & operator=(const OStringStream & other) noexcept = default;

  /*!
    \brief Move assignment operator.

    Assigns the contents of another OStringStream to this stream using move semantics. The source stream is left in a
    valid but unspecified state.

    \param other The source OStringStream to move from.

    \return A reference to this OStringStream after assignment.

    \pre The \a other stream must be valid and properly initialized.

    \post This stream contains the same content as the source \a other stream.
    \post The source \a other stream is left in a valid but unspecified state.

    \note Self-assignment is handled correctly and safely.
  */
  constexpr OStringStream & operator=(OStringStream && other) noexcept = default;

  /*!
    \brief Swaps the contents of this stream with another stream.

    This method exchanges the underlying string storage between this stream and the \a other stream. Both streams must
    have the same StringType template parameter.

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

    \note This operator follows the same pattern as std::ostringstream::operator<<(bool).

    \post The write position is advanced by the length of the appended string (4 for "true", 5 for "false").

    \sa put(char_type)
    \sa write(const char_type *, size_t)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(bool value) noexcept;

  /*!
    \brief Inserts a signed 8-bit integer value into the stream.

    This operator converts the signed 8-bit integer \a value to its decimal string representation and appends it to the
    stream. Negative values are prefixed with a minus sign.

    \param value The signed 8-bit integer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as std::ostringstream::operator<<(int8_t).

    \sa operator<<(uint8_t)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(int8_t value) noexcept;

  /*!
    \brief Inserts a signed 16-bit integer value into the stream.

    This operator converts the signed 16-bit integer \a value to its decimal string representation and appends it to the
    stream. Negative values are prefixed with a minus sign.

    \param value The signed 16-bit integer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as std::ostringstream::operator<<(int16_t).

    \sa operator<<(uint16_t)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(int16_t value) noexcept;

  /*!
    \brief Inserts a signed 32-bit integer value into the stream.

    This operator converts the signed 32-bit integer \a value to its decimal string representation and appends it to the
    stream. Negative values are prefixed with a minus sign.

    \param value The signed 32-bit integer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as std::ostringstream::operator<<(int32_t).

    \sa operator<<(uint32_t)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(int32_t value) noexcept;

  /*!
    \brief Inserts a signed 64-bit integer value into the stream.

    This operator converts the signed 64-bit integer \a value to its decimal string representation and appends it to the
    stream. Negative values are prefixed with a minus sign.

    \param value The signed 64-bit integer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as std::ostringstream::operator<<(int64_t).

    \sa operator<<(uint64_t)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(int64_t value) noexcept;

  /*!
    \brief Inserts an unsigned 8-bit integer value into the stream.

    This operator converts the unsigned 8-bit integer \a value to its decimal string representation and appends it to
    the stream.

    \param value The unsigned 8-bit integer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as std::ostringstream::operator<<(uint8_t).

    \sa operator<<(int8_t)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(uint8_t value) noexcept;

  /*!
    \brief Inserts an unsigned 16-bit integer value into the stream.

    This operator converts the unsigned 16-bit integer \a value to its decimal string representation and appends it to
    the stream.

    \param value The unsigned 16-bit integer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as std::ostringstream::operator<<(uint16_t).

    \sa operator<<(int16_t)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(uint16_t value) noexcept;

  /*!
    \brief Inserts an unsigned 32-bit integer value into the stream.

    This operator converts the unsigned 32-bit integer \a value to its decimal string representation and appends it to
    the stream.

    \param value The unsigned 32-bit integer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as std::ostringstream::operator<<(uint32_t).

    \sa operator<<(int32_t)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(uint32_t value) noexcept;

  /*!
    \brief Inserts an unsigned 64-bit integer value into the stream.

    This operator converts the unsigned 64-bit integer \a value to its decimal string representation and appends it to
    the stream.

    \param value The unsigned 64-bit integer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as std::ostringstream::operator<<(uint64_t).

    \sa operator<<(int64_t)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(uint64_t value) noexcept;

  /*!
    \brief Inserts a single-precision floating-point value into the stream.

    This operator converts the float \a value to its decimal string representation with the current precision setting
    and appends it to the stream. The precision controls the number of digits displayed.

    \param value The single-precision floating-point value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as std::ostringstream::operator<<(float).
    \note The precision is controlled by precision() and defaults to 6 digits.

    \sa precision() const
    \sa precision(int)
    \sa operator<<(double)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(float value) noexcept;

  /*!
    \brief Inserts a double-precision floating-point value into the stream.

    This operator converts the double \a value to its decimal string representation with the current precision setting
    and appends it to the stream. The precision controls the number of digits displayed.

    \param value The double-precision floating-point value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as std::ostringstream::operator<<(double).
    \note The precision is controlled by precision(int) and defaults to 6 digits.

    \sa precision() const
    \sa precision(int)
    \sa operator<<(float)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(double value) noexcept;

  /*!
    \brief Inserts a pointer value into the stream.

    This operator converts the pointer \a value to its hexadecimal string representation with "0x" prefix and appends it
    to the stream. The pointer is formatted as a hexadecimal number, matching the standard C++ pointer output format.

    \param value The pointer value to insert into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note This operator follows the same pattern as std::ostringstream::operator<<(const void*).
    \note For null pointers, consider using operator<<(nullptr_t) for consistent "nullptr" output.

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

    \note This operator follows the same pattern as std::ostringstream::operator<<(std::nullptr_t).
    \note The output is always the string "nullptr", regardless of the actual null pointer value.
    \note This operator is preferred over operator<<(const void*) for null pointer literals.

    \sa operator<<(const void *)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(nullptr_t) noexcept;

  /*!
    \brief Returns a const reference to the underlying string.

    This method provides read-only access to the internal string storage.

    \return A const reference to the underlying string.

    \note The returned reference is valid for the lifetime of the OStringStream object.

    \sa view()
  */
  [[nodiscard]] constexpr const StringType & str() const noexcept;

  /*!
    \brief Sets the content of the stream from a string-like object.

    This method replaces the current content of the stream with the content from a string-like object.

    \tparam SourceStringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string The source string-like object to set the content from.

    \pre The \a string must be valid and properly initialized.

    \post The stream contains a copy of the source string content.
    \post The previous content of the stream is replaced.

    \note The method performs a deep copy of the string content.
    \note The source string type can be different from StringType as long as both satisfy \ref toy::StringLike.
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
    \sa toy::CStringView
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

    \sa put(char_type)
    \sa tellp()
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

    \sa str() const
    \sa put(char_type)
    \sa write(const char_type *, size_t)
  */
  [[nodiscard]] constexpr pos_type tellp() const noexcept;

  /*!
    \brief Returns the current floating-point precision setting.

    This method returns the precision value that will be used for formatting floating-point numbers when writing to the
    stream. The precision specifies the number of digits to display.

    \return The current precision value.

    \note The precision value affects how floating-point numbers are formatted when written to the stream.

    \sa precision(int)
  */
  [[nodiscard]] constexpr int precision() const noexcept;

  /*!
    \brief Sets the floating-point precision and returns the previous value.

    This method sets the precision value that will be used for formatting floating-point numbers when writing to the
    stream. The precision specifies the number of digits to display.

    \param newPrecision The new precision value to set.

    \return The previous precision value before the change.

    \pre The \a newPrecision must be non-negative.

    \note The precision value affects how floating-point numbers are formatted when written to the stream.

    \sa precision() const
  */
  constexpr int precision(int newPrecision) noexcept;

private:
  StringType _string; //!< Internal string storage for the stream content.
  int _precision = 6; //!< Floating-point precision value used for number formatting.
};

} // namespace toy

#endif // INCLUDE_CORE_O_STRING_STREAM_HPP_
