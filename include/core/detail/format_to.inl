//
// Copyright (c) 2026 Toyman Interactive
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
  \file   format_to.inl
  \brief  Inline implementations for toy::detail::formatTo() and its formatters.
*/

#ifndef INCLUDE_CORE_DETAIL_FORMAT_TO_INL_
#define INCLUDE_CORE_DETAIL_FORMAT_TO_INL_

namespace toy::detail {

template <typename SinkT>
[[nodiscard]] constexpr size_t formatBoolTo(SinkT & sink, bool value) noexcept {
  const auto text = value ? span<const char>{"true", 4} : span<const char>{"false", 5};

  sink.write(text);

  return text.size();
}

template <typename SinkT>
[[nodiscard]] constexpr size_t formatCharTo(SinkT & sink, char character) noexcept {
  sink.put(character);

  return 1;
}

template <typename SinkT, std::signed_integral T>
[[nodiscard]] constexpr size_t formatIntTo(SinkT & sink, T value) noexcept {
  char         buffer[std::numeric_limits<T>::digits10 + 3]; // The sign, the widest magnitude, and the terminator.
  char * const first = itoa(span<char>{buffer}, value);
  const auto   count = static_cast<size_t>(buffer + sizeof(buffer) - 1 - first);

  sink.write(span<const char>{first, count});

  return count;
}

template <typename SinkT, std::unsigned_integral T>
[[nodiscard]] constexpr size_t formatUIntTo(SinkT & sink, T value) noexcept {
  char         buffer[std::numeric_limits<T>::digits10 + 2]; // The widest magnitude and the terminator.
  char * const first = utoa(span<char>{buffer}, value);
  const auto   count = static_cast<size_t>(buffer + sizeof(buffer) - 1 - first);

  sink.write(span<const char>{first, count});

  return count;
}

template <typename SinkT>
[[nodiscard]] constexpr size_t dispatchArgument(SinkT & sink, const FormatArg & arg) noexcept {
  switch (arg.type) {
    case FormatArgType::charValue:
      return formatCharTo(sink, arg.character);
    case FormatArgType::boolValue:
      return formatBoolTo(sink, arg.boolean);
    case FormatArgType::int8Value:
      return formatIntTo(sink, arg.int8);
    case FormatArgType::int16Value:
      return formatIntTo(sink, arg.int16);
    case FormatArgType::int32Value:
      return formatIntTo(sink, arg.int32);
    case FormatArgType::int64Value:
      return formatIntTo(sink, arg.int64);
    case FormatArgType::uint8Value:
      return formatUIntTo(sink, arg.uint8);
    case FormatArgType::uint16Value:
      return formatUIntTo(sink, arg.uint16);
    case FormatArgType::uint32Value:
      return formatUIntTo(sink, arg.uint32);
    case FormatArgType::uint64Value:
      return formatUIntTo(sink, arg.uint64);
    case FormatArgType::none:
      break;
  }

  return 0;
}

template <typename SinkT>
constexpr size_t walk(SinkT & sink, StringView pattern, span<const FormatArg> args) noexcept {
  assert_message(validateFormatPattern(pattern, args.size()) == FormatPatternValidationError::none,
                 "formatTo requires a pattern that is valid for the argument count");

  size_t       writtenBytes = 0;
  size_t       position     = 0;
  size_t       nextAuto     = 0;
  const size_t length       = pattern.size();

  while (position < length) {
    const char c = pattern.at(position);

    if (c != '{' && c != '}') {
      const auto start = position;
      do {
        ++position;
      } while (position < length && pattern.at(position) != '{' && pattern.at(position) != '}');

      sink.write(span<const char>{pattern.data() + start, position - start});
      writtenBytes += position - start;

      continue;
    }

    if (c == '}') {
      if (!consumeEscapedBrace(pattern, length, position, '}'))
        ++position;

      sink.put('}');
      ++writtenBytes;

      continue;
    }

    if (consumeEscapedBrace(pattern, length, position, '{')) {
      sink.put('{');
      ++writtenBytes;

      continue;
    }

    PlaceholderMode mode      = PlaceholderMode::none;
    size_t          autoCount = 0;
    size_t          index     = 0;
    const size_t    brace     = position;
    if (parseOpeningBrace(pattern, length, position, autoCount, mode, args.size(), &index)
        != FormatPatternValidationError::none) {
      position = brace + 1;
      if (position == length) {
        sink.put('{');
        ++writtenBytes;
        continue;
      }

      while (position < length && pattern.at(position) != '}')
        ++position;
      if (position < length)
        ++position;

      continue;
    }

    const size_t argIndex = mode == PlaceholderMode::autoIndex ? nextAuto++ : index;
    if (argIndex < args.size())
      writtenBytes += dispatchArgument(sink, args[argIndex]);
  }

  return writtenBytes;
}

constexpr void formatChar(FormatContext sink, char character) noexcept {
  static_cast<void>(formatCharTo(sink, character));
}

constexpr void formatBool(FormatContext sink, bool value) noexcept {
  static_cast<void>(formatBoolTo(sink, value));
}

constexpr size_t formatTo(FormatContext sink, StringView pattern, span<const FormatArg> args) noexcept {
  return walk(sink, pattern, args);
}

} // namespace toy::detail

#endif // INCLUDE_CORE_DETAIL_FORMAT_TO_INL_
