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
  \file   format.inl
  \brief  Inline implementations for toy::format() and toy::formatTo().

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_INL_
#define INCLUDE_CORE_FORMAT_INL_

namespace toy {

namespace {

/*!
  \brief Writes the argument at position \a index in the pack to \a stream.

  Peels arguments one at a time via recursion, decrementing \a index at each step. When \a index reaches zero the
  current \a first argument is written. The recursion terminates either when the matching argument is found or when the
  pack is exhausted (which cannot happen on valid input because \ref toy::FormatString validates index bounds at compile
  time).

  \tparam StringType Type of the stream's backing storage.
  \tparam First      Type of the leading argument in the current recursion step.
  \tparam Rest       Types of the remaining arguments.

  \param index  Zero-based position of the argument to write.
  \param stream Destination stream.
  \param first  Leading argument in the current recursion step.
  \param rest   Remaining arguments passed through unchanged.

  \pre \a index is less than \c 1 + sizeof...(Rest); guaranteed by compile-time pattern validation.

  \note Internal helper for toy::formatTo(); not part of the public API.
*/
template <typename StringType, typename First, typename... Rest>
constexpr void dispatchFormatArg(size_t index, OStringStream<StringType> & stream, const First & first,
                                 const Rest &... rest) noexcept {
  if (index == 0) {
    stream << first;

    return;
  }

  if constexpr (sizeof...(Rest) > 0)
    dispatchFormatArg(index - 1, stream, rest...);
}

} // namespace

template <size_t BufferSize, typename... Args>
[[nodiscard]] constexpr FixedString<BufferSize> format(type_identity_t<FormatString<Args...>> fmt,
                                                       const Args &... args) noexcept {
  FixedString<BufferSize> result;

  formatTo(result, fmt, args...);

  return result;
}

template <StringLike StringType, typename... Args>
constexpr void formatTo(StringType & output, type_identity_t<FormatString<Args...>> fmt,
                        const Args &... args) noexcept {
  OStringStream<StringType> stream;

  const auto pattern = fmt.get();
  const auto length  = pattern.size();
  size_t position    = 0;
  size_t autoIndex   = 0;

  while (position < length) {
    const char c = pattern.at(position);
    if (c == '{') {
      if (position + 1 < length && pattern.at(position + 1) == '{') {
        stream.put('{');
        position += 2;

        continue;
      }

      // Find the closing brace and extract the content between { and }.
      const size_t start = position + 1;
      auto end           = start;
      while (end < length && pattern.at(end) != '}')
        ++end;

      size_t argIndex;
      if (start == end) {
        argIndex = autoIndex++;
      } else {
        argIndex = 0;

        for (size_t i = start; i < end; ++i) {
          argIndex = argIndex * 10U + static_cast<size_t>(pattern.at(i) - '0');
        }
      }

      if constexpr (sizeof...(Args) > 0)
        dispatchFormatArg(argIndex, stream, args...);

      position = end + 1;

      continue;
    } else if (c == '}' && position + 1 < length && pattern.at(position + 1) == '}') {
      stream.put('}');
      position += 2;

      continue;
    }

    stream.put(c);
    ++position;
  }

  output = stream.str();
}

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_INL_
