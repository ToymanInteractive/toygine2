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
  \brief  TODO
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
  inline OStringStream & put(char_type character) noexcept;

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
  inline OStringStream & write(const char_type * string, size_t count) noexcept;

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

private:
  StringType _string;
};

} // namespace toy

#endif // INCLUDE_CORE_O_STRING_STREAM_HPP_
