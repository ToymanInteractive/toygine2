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

  Defines \ref toy::OStringStream: an allocator-free output stream that appends formatted values into any storage
  satisfying \ref toy::OStringStreamBackend. Used when building diagnostic strings, log entries, or formatted output
  with bounded or fixed storage (e.g. \ref toy::FixedString) without heap allocation.

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
concept OStringStreamBackend = std::default_initializable<T> && std::assignable_from<T &, const T &>
                               && std::is_nothrow_swappable_v<T>
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
  \a BackendType (e.g. \ref toy::FixedString). Integral, floating-point, boolean, pointer, character, C-string,
  string-like, \ref toy::chrono::Duration, \ref toy::chrono::TimePoint, and \ref toy::chrono::CalendarTime inserts are
  supported.

  \tparam BackendType The type of the underlying storage. Must satisfy the \ref toy::OStringStreamBackend concept.

  \section features Key Features

  - **Storage policy**: No separate allocator; behavior and limits come from \a BackendType.
  - **constexpr**: Construction and many inserts are usable in constant evaluation when \a BackendType allows it.
  - **noexcept**: Members are \c noexcept; failed capacity is handled per \a BackendType (typically debug assertions).
  - **Floating-point precision**: \c precision() / \c setPrecision() affect subsequent floating inserts.
  - **Chrono types**: \ref toy::chrono::Duration formats as a decimal second count with an \c 's' suffix
    (e.g. \c "0.016000000s") or, via \ref toy::chrono::DurationFormat, as a clock-style string using a pattern
    (e.g. \c "hh:mm:ss.zzz" → \c "00:00:01.042"); \ref toy::chrono::TimePoint delegates to \ref toy::chrono::Duration;
    \ref toy::chrono::CalendarTime outputs ISO 8601 (e.g. \c "2026-05-20 14:30:45.123").

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
    \brief Constructs an OStringStream with initial content copied from \a string.

    \tparam SourceStringType Source string type. Must satisfy the \ref toy::StringLike concept.

    \param string The source string-like object to copy content from.

    \pre The \a string must be valid and properly initialized.

    \post The stream contains a copy of the source string content.

    \note \a SourceStringType may differ from \a BackendType provided both satisfy \ref toy::StringLike.
  */
  template <StringLike SourceStringType>
  explicit constexpr OStringStream(const SourceStringType & string) noexcept;

  /*!
    \brief Swaps the contents of this stream with \a other.

    \param other The stream to swap contents with.

    \post The contents of this stream and \a other are exchanged.

    \note Self-swap is safe and has no effect.
  */
  constexpr void swap(OStringStream & other) noexcept;

  /*!
    \brief Inserts a boolean value into the stream.

    Appends \c "true" or \c "false" according to \a value.

    \param value The boolean value to insert.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by 4 (\c true) or 5 (\c false) characters.

    \sa tellp()
  */
  constexpr OStringStream & operator<<(bool value) noexcept;

  /*!
    \brief Inserts a signed long integer value into the stream.

    \param value The signed long integer value to insert.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the decimal representation.

    \sa operator<<(unsigned long)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(long value) noexcept;

  /*!
    \brief Inserts an unsigned long integer value into the stream.

    \param value The unsigned long integer value to insert.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the decimal representation.

    \sa operator<<(long)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(unsigned long value) noexcept;

  /*!
    \brief Inserts a signed long long integer value into the stream.

    \param value The signed long long integer value to insert.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the decimal representation.

    \sa operator<<(unsigned long long)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(long long value) noexcept;

  /*!
    \brief Inserts an unsigned long long integer value into the stream.

    \param value The unsigned long long integer value to insert.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the decimal representation.

    \sa operator<<(long long)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(unsigned long long value) noexcept;

  /*!
    \brief Inserts a double-precision floating-point value into the stream.

    The number of digits is controlled by the current precision setting.

    \param value The double-precision floating-point value to insert.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the formatted representation.

    \note The precision defaults to 6 digits; use setPrecision() to change it.

    \sa precision() const
    \sa setPrecision(size_t)
    \sa operator<<(float)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(double value) noexcept;

  /*!
    \brief Inserts a pointer value into the stream as a zero-padded hexadecimal address.

    The output has a \c "0x" prefix and is zero-padded to the pointer width: 8 hex digits on 32-bit platforms, 16 on
    64-bit (e.g. \c "0x00000000" or \c "0x0000000000000000").

    \param value The pointer value to insert.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by \c 2 + sizeof(void*) * 2 characters.

    \note Null pointers are redirected to operator<<(nullptr_t) which outputs \c "nullptr".

    \sa operator<<(nullptr_t)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(const void * value) noexcept;

  /*!
    \brief Inserts the string \c "nullptr" into the stream.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by 7 characters.

    \note Prefer this overload over operator<<(const void*) when the intent is to indicate a null pointer.

    \sa operator<<(const void *)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(nullptr_t) noexcept;

  /*!
    \brief Inserts a signed short integer value into the stream.

    \param value The signed short integer value to insert.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the decimal representation.

    \sa operator<<(unsigned short)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(short value) noexcept;

  /*!
    \brief Inserts a signed integer value into the stream.

    \param value The signed integer value to insert.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the decimal representation.

    \sa operator<<(unsigned int)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(int value) noexcept;

  /*!
    \brief Inserts an unsigned short integer value into the stream.

    \param value The unsigned short integer value to insert.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the decimal representation.

    \sa operator<<(short)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(unsigned short value) noexcept;

  /*!
    \brief Inserts an unsigned integer value into the stream.

    \param value The unsigned integer value to insert.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the decimal representation.

    \sa operator<<(int)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(unsigned int value) noexcept;

  /*!
    \brief Inserts a single-precision floating-point value into the stream.

    The number of digits is controlled by the current precision setting.

    \param value The single-precision floating-point value to insert.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the formatted representation.

    \note The precision defaults to 6 digits; use setPrecision() to change it.

    \sa precision() const
    \sa setPrecision(size_t)
    \sa operator<<(double)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(float value) noexcept;

  /*!
    \brief Inserts a signed 8-bit integer value into the stream.

    \param value The signed 8-bit integer value to insert.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the decimal representation.

    \sa operator<<(unsigned char)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(signed char value) noexcept;

  /*!
    \brief Inserts an unsigned 8-bit integer value into the stream.

    \param value The unsigned 8-bit integer value to insert.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the decimal representation.

    \sa operator<<(signed char)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(unsigned char value) noexcept;

  /*!
    \brief Inserts a single character into the stream.

    The character is appended as-is; no numeric conversion is applied. To insert the numeric value of a byte, use
    operator<<(signed char) or operator<<(unsigned char).

    \param value The character to insert.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by one character.

    \sa operator<<(signed char)
    \sa operator<<(unsigned char)
    \sa operator<<(const char_type *)
    \sa operator<<(const SourceStringType &)
    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(char_type value) noexcept;

  /*!
    \brief Inserts a string-like object into the stream.

    \tparam SourceStringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param value The string-like object to insert.

    \return A reference to this OStringStream, allowing operator chaining.

    \pre The \a value must be valid and properly initialized.

    \post The write position is advanced by the length of the appended string.

    \note \a SourceStringType may differ from \a BackendType provided both satisfy \ref toy::StringLike.

    \sa operator<<(const char_type *)
    \sa operator<<(char_type)
    \sa put(char_type)
    \sa write(const char_type *, size_t)
    \sa tellp()
  */
  template <StringLike SourceStringType>
  constexpr OStringStream & operator<<(const SourceStringType & value) noexcept;

  /*!
    \brief Inserts a null-terminated C string into the stream.

    \param value The C string to insert. Must not be null.

    \return A reference to this OStringStream, allowing operator chaining.

    \pre The \a value must not be null.

    \post The write position is advanced by the string length (excluding the null terminator).

    \sa operator<<(const SourceStringType &)
    \sa operator<<(char_type)
    \sa put(char_type)
    \sa write(const char_type *, size_t)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(const char_type * value) noexcept;

  /*!
    \brief Inserts a \ref toy::chrono::Duration as decimal seconds into the stream.

    Converts \a value to seconds using integer-only arithmetic and appends it with an \c 's' suffix. The fractional part
    width is determined at compile time from \c Period::den (e.g. 9 digits for nanoseconds, 3 for milliseconds).
    Negative durations are prefixed with \c '-'.

    \tparam Rep    Arithmetic representation type of the duration tick count.
    \tparam Period \c std::ratio specifying the tick period relative to one second.

    \param value The duration to insert.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note For sub-second periods (\c Period::num == 1 && \c Period::den > 1), the output includes a decimal point
          followed by \c ceil(log10(Period::den)) fractional digits (e.g. \c "0.016000000s" for nanoseconds).
    \note For coarser-than-second periods (e.g. hours, minutes), the output is the total number of whole seconds
          (e.g. \c "3600s" for one hour).

    \sa operator<<(chrono::DurationFormat<Rep, Period>)
    \sa operator<<(chrono::TimePoint<Clock, Dur>)
    \sa operator<<(chrono::CalendarTime)
    \sa tellp()
  */
  template <typename Rep, typename Period>
    requires std::signed_integral<Rep>
  constexpr OStringStream & operator<<(const chrono::Duration<Rep, Period> & value) noexcept;

  /*!
    \brief Inserts a \ref toy::chrono::Duration formatted using the clock-style pattern in \a value.

    Decomposes the duration into hours, minutes, seconds, and milliseconds via a single \c duration_cast to
    milliseconds. The \c pattern field is then iterated character-by-character: recognised tokens are replaced with the
    corresponding time component; all other characters are emitted verbatim. Negative durations are prefixed with
    \c '-'.

    \tparam Rep    Arithmetic representation type of the duration tick count.
    \tparam Period \c std::ratio specifying the tick period relative to one second.

    \param value Wrapper carrying the duration and the null-terminated display pattern; see
                 \ref toy::chrono::DurationFormat for the full token reference.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note Recognised tokens: \c h / \c hh (hours), \c m / \c mm (minutes), \c s / \c ss (seconds),
          \c z / \c zzz (milliseconds); all other pattern characters are emitted verbatim.

    \sa operator<<(chrono::Duration<Rep, Period>)
    \sa operator<<(chrono::TimePointFormat<Clock, Dur>)
    \sa tellp()
  */
  template <typename Rep, typename Period>
    requires std::signed_integral<Rep>
  constexpr OStringStream & operator<<(const chrono::DurationFormat<Rep, Period> & value) noexcept;

  /*!
    \brief Inserts a \ref toy::chrono::TimePoint as decimal seconds since the clock epoch into the stream.

    Formats \a value by delegating to operator<<(chrono::Duration) with \c value.time_since_epoch(). The output
    represents the duration from the clock epoch to \a value in seconds with a fractional width determined by the
    clock's tick period.

    \tparam Clock The clock type that defines the epoch.
    \tparam Dur   Duration type used to store the offset from the epoch.

    \param value The time point to insert.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the appended string.

    \note The epoch is implementation-defined and may differ across processes and reboots; only differences between
          two time points produced by the same \ref toy::chrono::ClockSource are meaningful.

    \sa operator<<(chrono::TimePointFormat<Clock, Dur>)
    \sa operator<<(chrono::Duration<Rep, Period>)
    \sa operator<<(chrono::CalendarTime)
    \sa tellp()
  */
  template <typename Clock, typename Dur>
    requires std::signed_integral<typename Dur::rep>
  constexpr OStringStream & operator<<(chrono::TimePoint<Clock, Dur> value) noexcept;

  /*!
    \brief Inserts a \ref toy::chrono::TimePoint formatted using the clock-style pattern in \a value.

    Delegates to operator<<(chrono::DurationFormat) with \c value.timePoint.time_since_epoch(), so the output represents
    elapsed time since the clock epoch using the same token language as \ref toy::chrono::DurationFormat.

    \tparam Clock The clock type that defines the epoch.
    \tparam Dur   Duration type of the time point; its \c rep must be an signed integral type.

    \param value Wrapper carrying the time point and the null-terminated display pattern; see
                 \ref toy::chrono::DurationFormat for the full token reference.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by the length of the formatted string.

    \note The epoch is implementation-defined; only differences between time points from the same
          \ref toy::chrono::ClockSource are meaningful.
    \note No floating-point arithmetic is performed; safe on FPU-less targets (GBA, NDS).

    \sa operator<<(chrono::DurationFormat<Rep, Period>)
    \sa operator<<(chrono::TimePoint<Clock, Dur>)
    \sa tellp()
  */
  template <typename Clock, typename Dur>
    requires std::signed_integral<typename Dur::rep>
  constexpr OStringStream & operator<<(chrono::TimePointFormat<Clock, Dur> value) noexcept;

  /*!
    \brief Inserts a \ref toy::chrono::CalendarTime as an ISO 8601 date-time string into the stream.

    Formats \a value as \c "YYYY-MM-DD HH:MM:SS.ZZZ" using UTC field values from \a value directly. All numeric fields
    are zero-padded to their canonical width. When \a value equals \ref toy::chrono::CalendarTime::invalid() (all fields
    zero) the output is \c "0000-00-00 00:00:00.000".

    \param value The calendar date and time to insert.

    \return A reference to this OStringStream, allowing operator chaining.

    \post The write position is advanced by 23 characters (the fixed width of the formatted string).

    \note No locale, time zone, or DST conversion is applied; fields are taken from \a value as-is.

    \sa operator<<(chrono::Duration<Rep, Period>)
    \sa operator<<(chrono::TimePoint<Clock, Dur>)
    \sa tellp()
  */
  constexpr OStringStream & operator<<(chrono::CalendarTime value) noexcept;

  /*!
    \brief Returns a const reference to the underlying string storage.

    \return A const reference to the underlying \a BackendType string.

    \note The returned reference is valid for the lifetime of the OStringStream object.

    \sa view()
  */
  [[nodiscard]] constexpr const BackendType & str() const noexcept;

  /*!
    \brief Replaces the stream content with a copy of \a string.

    \tparam SourceStringType The type of the source string. Must satisfy the \ref toy::StringLike concept.

    \param string The source string-like object to copy content from.

    \pre The \a string must be valid and properly initialized.

    \post The stream contains a copy of \a string; the previous content is discarded.

    \note \a SourceStringType may differ from \a BackendType provided both satisfy \ref toy::StringLike.

    \sa str() const
  */
  template <StringLike SourceStringType>
  constexpr void str(const SourceStringType & string) noexcept;

  /*!
    \brief Returns a non-owning view of the underlying string.

    \return A \ref toy::CStringView over the current stream content.

    \note The view is valid for the lifetime of the OStringStream object; do not use it after the stream is destroyed.
    \note The view reflects the state at the time of the call; subsequent writes are not visible through an existing
          view.

    \sa str() const
    \sa \ref toy::CStringView
  */
  [[nodiscard]] constexpr CStringView view() const noexcept;

  /*!
    \brief Appends a single character to the stream.

    \param character The character to append.

    \return A reference to this OStringStream, allowing method chaining.

    \pre The \a character must not be the null character (\c '\0').

    \post The \a character is appended to the end of the stream content.
    \post The write position is advanced by one character.

    \sa write(const char_type *, size_t)
    \sa tellp()
  */
  constexpr OStringStream & put(char_type character) noexcept;

  /*!
    \brief Writes \a count characters from \a string to the stream.

    Unlike operator<<(const char_type *), the input does not need to be null-terminated.

    \param string The pointer to the character buffer to write from. Must not be null when \a count > 0.
    \param count  The number of characters to write.

    \return A reference to this OStringStream, allowing method chaining.

    \pre If \a count > 0, \a string must not be null and must point to a valid buffer of at least \a count characters.

    \post \a count characters from \a string are appended to the stream.
    \post The write position is advanced by \a count characters.

    \note When \a count is zero, the method returns without modifying the stream.

    \sa put(char_type)
    \sa tellp()
  */
  constexpr OStringStream & write(const char_type * string, size_t count) noexcept;

  /*!
    \brief Returns the current write position, equal to the number of characters appended so far.

    Because all writes go to the end, this value always equals \c str().size().

    \return The current write position.

    \sa str() const
    \sa put(char_type)
    \sa write(const char_type *, size_t)
  */
  [[nodiscard]] constexpr pos_type tellp() const noexcept;

  /*!
    \brief Returns the current floating-point precision.

    \return The number of digits used when formatting floating-point values.

    \sa setPrecision(size_t)
  */
  [[nodiscard]] constexpr size_t precision() const noexcept;

  /*!
    \brief Sets the floating-point precision and returns the previous value.

    \param newPrecision The new number of digits to use when formatting floating-point values.

    \return The previous precision value.

    \pre The \a newPrecision must not exceed the maximum supported digits for \c long \c double.

    \sa precision() const
  */
  constexpr size_t setPrecision(size_t newPrecision) noexcept;

private:
  /// Writes \a value zero-padded to exactly \a width decimal digits.
  void _writeZeroPadded(int64_t value, size_t width) noexcept;

  /// Writes \a value zero-padded to exactly \a width decimal digits.
  void _writeZeroPadded(int32_t value, size_t width) noexcept;

  /// Internal string storage for the stream content.
  BackendType _string;

  /// Floating-point precision value used for number formatting.
  size_t _precision = 6;
};

} // namespace toy

#endif // INCLUDE_CORE_O_STRING_STREAM_HPP_
