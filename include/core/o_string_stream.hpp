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
  /*!
    \brief Default constructor.

    Constructs an empty OStringStream with default-initialized string storage.

    \post The stream contains an empty string.
  */
  constexpr OStringStream() noexcept = default;

  /*!
    \brief Constructs an OStringStream from a string-like object.

    This constructor initializes the stream by copying the content from a string-like object. The source object must
    satisfy the \ref toy::StringLike concept.

    \tparam SourceStringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string The source string-like object to copy content from.

    \pre The \a string must be valid and properly initialized.
    \pre The \a string size must not exceed the capacity of StringType.

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
  */
  [[nodiscard]] constexpr const StringType & str() const noexcept;

private:
  StringType _string;
};

} // namespace toy

#endif // INCLUDE_CORE_O_STRING_STREAM_HPP_
