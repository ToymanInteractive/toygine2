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
  \file   console_application_winapi.cpp
  \brief  Implementation of toy::application::ConsoleApplication::writeToSystemConsole() for Windows.

  Also defines convertEntryPointArgs(), which IMPLEMENT_CONSOLE_APP calls to convert the wide-character command line
  to UTF-8.
*/

#include "application.hpp"

// The console API and wmain() hold UTF-16 in wchar_t, so a unit-by-unit copy to char16_t keeps the text intact.
static_assert(sizeof(wchar_t) == sizeof(char16_t), "wchar_t must hold UTF-16 code units on Windows");

namespace toy::application {

void ConsoleApplication::writeToSystemConsole(StringView text, bool isError, bool newLine) noexcept {
  if (text.empty() && !newLine)
    return;

  const auto handle    = ::GetStdHandle(isError ? STD_ERROR_HANDLE : STD_OUTPUT_HANDLE);
  DWORD      mode      = 0;
  const bool isConsole = ::GetConsoleMode(handle, &mode) != 0;

  static constinit SRWLOCK lock = SRWLOCK_INIT;

  ::AcquireSRWLockExclusive(&lock);

  if (isConsole) {
    constexpr size_t                 chunkUnits = 512;
    std::array<char16_t, chunkUnits> buffer;
    std::array<wchar_t, chunkUnits>  wideBuffer;
    while (!text.empty()) {
      auto length = std::min(text.size(), buffer.size() - 1);
      while (length < text.size() && length > 1 && (bit_cast<uint8_t>(text[length]) & 0xC0) == 0x80)
        --length;

      utf8ToUtf16(buffer.data(), buffer.size(), text.data(), length);
      auto units = std::char_traits<char16_t>::length(buffer.data());
      // At a decoded '\0' or an ill-formed byte the chunk ends; that byte goes out as U+0000 or U+FFFD.
      if (const auto consumed = utf16InUtf8Size(std::span<const char16_t>{buffer.data(), units}); consumed < length) {
        buffer[units++] = text[consumed] == '\0' ? u'\0' : u'\uFFFD';
        length          = consumed + 1;
      }
      std::ranges::transform(buffer.data(), buffer.data() + units, wideBuffer.data(), [](char16_t unit) noexcept {
        return static_cast<wchar_t>(unit);
      });
      ::WriteConsoleW(handle, wideBuffer.data(), static_cast<DWORD>(units), nullptr, nullptr);

      text.remove_prefix(length);
    }

    if (newLine)
      ::WriteConsoleW(handle, L"\n", 1, nullptr, nullptr);
  } else {
    DWORD written = 0;
    for (; !text.empty(); text.remove_prefix(written))
      if (!::WriteFile(handle, text.data(), static_cast<DWORD>(std::min<size_t>(text.size(), MAXDWORD)), &written,
                       nullptr)
          || written == 0)
        break;

    if (newLine && text.empty())
      ::WriteFile(handle, "\n", 1, &written, nullptr);
  }

  ::ReleaseSRWLockExclusive(&lock);
}

} // namespace toy::application

namespace {

/*!
  \brief Replaces every surrogate outside a pair in a range of UTF-16 units with U+FFFD.

  \param units First unit of the range.
  \param count Number of units in the range.

  \post Each surrogate in the range is half of a pair or has become U+FFFD; every other unit keeps its value.
*/
void replaceLoneSurrogates(char16_t * units, size_t count) noexcept {
  constexpr auto isHigh = [](char16_t unit) noexcept {
    return unit >= 0xD800 && unit <= 0xDBFF;
  };
  constexpr auto isLow = [](char16_t unit) noexcept {
    return unit >= 0xDC00 && unit <= 0xDFFF;
  };

  for (size_t index = 0; index < count; ++index)
    if (isHigh(units[index]) && index + 1 < count && isLow(units[index + 1]))
      ++index;
    else if (isHigh(units[index]) || isLow(units[index]))
      units[index] = 0xFFFD;
}

} // namespace

/*!
  \brief Converts the UTF-16 arguments wmain() receives into a null-terminated array of UTF-8 strings.

  Allocates the array and all strings as one \c MALLOC block, which the caller releases with \c FREE.

  \param argc  Number of arguments in \a argvw.
  \param argvw UTF-16 arguments as wmain() receives them.
  \param argv  Pointer that receives the converted array.

  \return \c true when \a argv receives the result, \c false when the allocation fails.

  \pre \a argv is non-null, as is \a argvw when \a argc is positive; assert_message checks both in debug builds.
  \pre \a argvw holds at least \a argc non-null strings.

  \post \a argv holds \a argc UTF-8 strings followed by a \c nullptr entry, or \c nullptr when \a argc is not positive
        or the allocation fails.

  \note A surrogate outside a pair arrives as U+FFFD, so an argument keeps every character after it.
*/
bool convertEntryPointArgs(int argc, wchar_t ** argvw, char *** argv) {
  assert_message(argv != nullptr, "argv must point to the variable that receives the converted array");
  *argv = nullptr;
  if (argc <= 0)
    return true;

  assert_message(argvw != nullptr, "argvw must not be null when argc is positive");

  // A string's slot holds its widest UTF-8 form and the terminator.
  constexpr auto slotSize = [](size_t units) noexcept {
    return units * toy::utf16InUtf8MaxSize + 1;
  };

  const auto wideArgs  = std::ranges::subrange(argvw, argvw + argc);
  const auto arraySize = (wideArgs.size() + 1) * sizeof(char *);

  // One block: the pointer array with its nullptr entry, a char16_t copy of the longest string, then a slot per string.
  size_t longestUnits = 0;
  auto   blockSize    = arraySize;
  for (const wchar_t * wideArg : wideArgs) {
    const auto units = std::char_traits<wchar_t>::length(wideArg);

    longestUnits  = std::max(longestUnits, units);
    blockSize    += slotSize(units);
  }
  const auto copySize  = longestUnits * sizeof(char16_t);
  blockSize           += copySize;

  void * const block = MALLOC(blockSize);
  if (block == nullptr)
    return false;

  auto ** slot     = static_cast<char **>(block);
  auto *  wideCopy = static_cast<char16_t *>(static_cast<void *>(static_cast<char *>(block) + arraySize));
  char *  text     = static_cast<char *>(block) + arraySize + copySize;
  for (const wchar_t * wideArg : wideArgs) {
    const auto units = std::char_traits<wchar_t>::length(wideArg);
    const auto size  = slotSize(units);

    std::ranges::transform(wideArg, wideArg + units, wideCopy, [](wchar_t unit) noexcept {
      return static_cast<char16_t>(unit);
    });
    replaceLoneSurrogates(wideCopy, units);
    *slot++  = toy::utf16ToUtf8(text, size, wideCopy, units);
    text    += size;
  }
  *slot = nullptr;

  *argv = static_cast<char **>(block);

  return true;
}
