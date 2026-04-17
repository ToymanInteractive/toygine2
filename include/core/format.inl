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
  \brief  Inline implementations for toy::format(), toy::formatTo(), toy::vformat(), and toy::vformatTo().

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

/*!
  \brief Resolves the argument index for a single `\{ ... \}` placeholder body.

  An empty body (`{}`) selects the next automatic index. Otherwise the body is read as a decimal numeral and returned as
  the zero-based argument index.

  \tparam PatternType Type that provides \c at(size_t) for indexed access to the pattern's code units.

  \param pattern   Format pattern string.
  \param start     Inclusive index of the first code unit inside the opening brace.
  \param end       Exclusive index of the closing brace; the examined range is \c [start, end).
  \param autoIndex Next index to use for an empty placeholder; updated when \a start equals \a end.

  \return Zero-based argument index to pass to the formatting dispatch.

  \pre \a start <= \a end. Both indices refer to valid positions in \a pattern for the range \c [start, end). When
       \a start < \a end, code units in that range are decimal digits; this holds for patterns accepted by
       \ref toy::FormatString.

  \post If \a start == \a end, \a autoIndex is incremented by \c 1. Otherwise \a autoIndex is unchanged.

  \note Internal helper for toy::formatTo(); not part of the public API.
*/
template <typename PatternType>
[[nodiscard]] constexpr size_t parseArgIndex(const PatternType & pattern, size_t start, size_t end,
                                             size_t & autoIndex) noexcept {
  if (start == end)
    return autoIndex++;

  size_t argIndex = 0;
  for (size_t i = start; i < end; ++i)
    argIndex = argIndex * 10U + static_cast<size_t>(pattern.at(i) - '0');

  return argIndex;
}

} // namespace

template <size_t BufferSize, typename... Args>
[[nodiscard]] constexpr FixedString<BufferSize> format(type_identity_t<FormatString<Args...>> fmt,
                                                       const Args &... args) noexcept {
  FixedString<BufferSize> result;

  formatTo(result, fmt, args...);

  return result;
}

template <OStringStreamBackend BackendType, typename... Args>
constexpr void formatTo(BackendType & output, type_identity_t<FormatString<Args...>> fmt,
                        const Args &... args) noexcept {
  OStringStream<BackendType> stream;

  const auto pattern   = fmt.get();
  const auto length    = pattern.size();
  size_t     position  = 0;
  size_t     autoIndex = 0;

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
      auto         end   = start;
      while (end < length && pattern.at(end) != '}')
        ++end;

      const auto argIndex = parseArgIndex(pattern, start, end, autoIndex);

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

template <size_t BufferSize, size_t MaximumArgs>
FixedString<BufferSize> vformat(CStringView pattern, const array<FormatArgument, MaximumArgs> & args) noexcept {
  FixedString<BufferSize> result;

  vformatTo(result, pattern, args);

  return result;
}

template <OStringStreamBackend BackendType, typename... Args>
void vformatTo(BackendType & output, CStringView pattern, const Args &... args) noexcept {
  vformatTo(output, pattern, makeVFormatArguments(args...));
}

template <OStringStreamBackend BackendType, size_t MaximumArgs>
void vformatTo(BackendType & output, CStringView pattern, const array<FormatArgument, MaximumArgs> & args) noexcept {
  assert_message(validateFormatPattern(pattern, MaximumArgs) == FormatPatternValidationError::none,
                 "vformatTo: pattern is invalid or inconsistent with the argument count");

  FormatContext out{static_cast<void *>(&output), [](void * ctx, const char * data, size_t count) noexcept {
                      static_cast<BackendType *>(ctx)->append(data, count);
                    }};

  output = BackendType{};

  const char * const data      = pattern.c_str();
  const auto         length    = pattern.size();
  size_t             position  = 0;
  size_t             autoIndex = 0;
  size_t             litStart  = 0;

  while (position < length) {
    if (const char c = data[position]; c == '{') {
      if (position + 1 < length && data[position + 1] == '{') {
        out.write(data + litStart, position - litStart);
        out.put('{');
        position += 2;
        litStart  = position;
        continue;
      }

      out.write(data + litStart, position - litStart);

      const size_t start = position + 1;
      auto         end   = start;
      while (end < length && data[end] != '}')
        ++end;

      if (const auto argIndex = parseArgIndex(pattern, start, end, autoIndex); argIndex < MaximumArgs) {
        const auto & argument = args[argIndex];
        assert_message(argument.formatFn != nullptr, "vformatTo: format callback must not be null");
        argument.formatFn(argument.value, out);
      }

      position = end + 1;
      litStart = position;

      continue;
    } else if (c == '}' && position + 1 < length && data[position + 1] == '}') {
      out.write(data + litStart, position - litStart);
      out.put('}');
      position += 2;
      litStart  = position;

      continue;
    }

    ++position;
  }

  if (position > litStart)
    out.write(data + litStart, position - litStart);
}

template <typename... Args>
[[nodiscard]] array<FormatArgument, sizeof...(Args)> makeVFormatArguments(const Args &... args) noexcept {
  return {[]<typename T>(const T & value) noexcept {
    return FormatArgument{static_cast<const void *>(&value), [](const void * v, FormatContext & out) noexcept {
                            const T & arg = *static_cast<const T *>(v);
                            if constexpr (StringLike<T>) {
                              out.write(arg.c_str(), arg.size());
                            } else if constexpr (std::is_pointer_v<T>
                                                 && std::is_same_v<std::remove_cv_t<std::remove_pointer_t<T>>, char>) {
                              if (arg != nullptr)
                                out.write(arg, char_traits<char>::length(arg));
                            } else {
                              OStringStream<FixedString<128>> stream;
                              stream << arg;
                              const auto & str = stream.str();
                              out.write(str.c_str(), str.size());
                            }
                          }};
  }(args)...};
}

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_INL_
