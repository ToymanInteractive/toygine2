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
  \brief  Implementation of toy::application::ConsoleApplication::writeToSystemConsole() for Windows, and the helper
          that IMPLEMENT_CONSOLE_APP calls to convert the wide-character command line to UTF-8.
*/

#include "application.hpp"

// The console API and wmain() hold UTF-16 in wchar_t, so it bit_casts to the char16_t the core utilities take.
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
    std::array<char16_t, 512> buffer;
    while (!text.empty()) {
      auto length = std::min(text.size(), buffer.size() - 1);
      while (length < text.size() && length > 1 && (bit_cast<uint8_t>(text[length]) & 0xC0) == 0x80)
        --length;

      // A '\0' or an ill-formed sequence cuts this chunk's output short, and utf8ToUtf16() does not report it.
      utf8ToUtf16(buffer.data(), buffer.size(), text.data(), length);
      const auto units = std::char_traits<char16_t>::length(buffer.data());
      ::WriteConsoleW(handle, bit_cast<const wchar_t *>(buffer.data()), static_cast<DWORD>(units), nullptr, nullptr);

      text.remove_prefix(length);
    }

    if (newLine)
      ::WriteConsoleW(handle, L"\n", 1, nullptr, nullptr);
  } else {
    DWORD written = 0;
    if (!text.empty())
      ::WriteFile(handle, text.data(), static_cast<DWORD>(text.size()), &written, nullptr);
    if (newLine)
      ::WriteFile(handle, "\n", 1, &written, nullptr);
  }

  ::ReleaseSRWLockExclusive(&lock);
}

} // namespace toy::application

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

  \note A converted argument ends before its first lone surrogate, and toy::utf16ToUtf8() does not report the cut.
*/
bool convertEntryPointArgs(int argc, wchar_t ** argvw, char *** argv) {
  assert_message(argv != nullptr, "argv must point to the variable that receives the converted array");
  *argv = nullptr;
  if (argc <= 0)
    return true;

  assert_message(argvw != nullptr, "argvw must not be null when argc is positive");

  // A string's slot holds its widest UTF-8 form and the terminator.
  constexpr auto slotSize = [](size_t units) noexcept {
    return units * toy::c_utf16InUtf8MaxSize + 1;
  };

  const auto wideArgs  = std::ranges::subrange(argvw, argvw + argc);
  const auto arraySize = (wideArgs.size() + 1) * sizeof(char *);

  // One block: the pointer array with its nullptr entry, then a slot for every string.
  auto blockSize = arraySize;
  for (const wchar_t * wideArg : wideArgs)
    blockSize += slotSize(std::char_traits<wchar_t>::length(wideArg));

  void * const block = MALLOC(blockSize);
  if (block == nullptr)
    return false;

  auto ** slot = static_cast<char **>(block);
  char *  text = static_cast<char *>(block) + arraySize;
  for (const wchar_t * wideArg : wideArgs) {
    const auto units = std::char_traits<wchar_t>::length(wideArg);
    const auto size  = slotSize(units);

    *slot++  = toy::utf16ToUtf8(text, size, toy::bit_cast<const char16_t *>(wideArg), units);
    text    += size;
  }
  *slot = nullptr;

  *argv = static_cast<char **>(block);

  return true;
}
