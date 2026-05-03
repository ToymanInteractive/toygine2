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
  \file   console_application_windows.cpp
  \brief  Windows console entry-point argument conversion from UTF-16 to UTF-8.
*/

#include "application.hpp"

namespace {

/// Frees each string in a null-terminated \c char** array, then frees the array itself.
void freeStringArray(char ** array) noexcept {
  assert_message(array != nullptr, "The array must not be null.");

  for (char ** it = array; *it != nullptr; ++it)
    free(*it);

  free(array);
}

/*!
  \brief Converts a null-terminated wide-character array of \a count strings to a heap-allocated UTF-8 array.

  On allocation failure for any individual string, frees all previously allocated strings and the array, then returns
  \c nullptr.

  \param wide  Source wide-character string array.
  \param count Number of strings in \a wide.

  \return Null-terminated \c char** array of UTF-8 strings on success, \c nullptr on allocation failure.
*/
[[nodiscard]] char ** convertWideStringArray(wchar_t ** wide, std::size_t count) noexcept {
  auto array = static_cast<char **>(malloc(sizeof(char *) * (count + 1)));
  if (array == nullptr)
    return nullptr;

  for (size_t index = 0; index < count; ++index) {
    const size_t bufferSize = wcslen(wide[index]) * toy::WCHAR_IN_UTF8_MAX_SIZE + 1;
    array[index]            = static_cast<char *>(malloc(bufferSize));
    if (array[index] == nullptr) {
      array[index] = nullptr;
      freeStringArray(array);
      return nullptr;
    }

    toy::wcharToUtf8(array[index], bufferSize, wide[index]);
  }

  array[count] = nullptr;

  return array;
}

} // namespace

/*!
  \brief Converts wide-character (UTF-16) command-line arguments and environment strings to UTF-8.

  Allocates heap memory for the converted arrays. The caller is responsible for releasing all allocations via
  clearEntryPointArgs().

  \param argc  Number of command-line arguments.
  \param argvw Wide-character argument array of \a argc entries, or \c nullptr.
  \param argv  Receives a newly allocated null-terminated UTF-8 argument array, or \c nullptr if \a argvw is \c nullptr.
  \param envpw Null-terminated wide-character environment array, or \c nullptr.
  \param envp  Receives a newly allocated null-terminated UTF-8 environment array, or \c nullptr if \a envpw is
               \c nullptr.

  \return \c true on success; \c false if any heap allocation fails.

  \post On success, \a *argv and \a *envp point to null-terminated arrays of heap-allocated UTF-8 strings.

  \warning On failure, any partially allocated output must still be released via clearEntryPointArgs() to avoid leaks.
*/
[[nodiscard]] bool convertEntryPointArgs(int argc, wchar_t ** argvw, char *** argv, wchar_t ** envpw,
                                         char *** envp) noexcept {
  if (argv != nullptr && argvw != nullptr) {
    if (argc <= 0) {
      *argv = nullptr;
    } else {
      *argv = convertWideStringArray(argvw, static_cast<size_t>(argc));
      if (*argv == nullptr)
        return false;
    }
  }

  if (envp != nullptr && envpw != nullptr) {
    std::size_t envc = 0;
    while (envpw[envc] != nullptr)
      ++envc;

    if (envc == 0) {
      *envp = nullptr;
    } else {
      *envp = convertWideStringArray(envpw, envc);
      if (*envp == nullptr)
        return false;
    }
  }

  return true;
}

/*!
  \brief Releases memory allocated by convertEntryPointArgs().

  Frees each string in \a argv and \a env, then frees the arrays themselves. Safe to call with \c nullptr arguments.

  \param argv UTF-8 argument array returned by convertEntryPointArgs(), or \c nullptr.
  \param env  UTF-8 environment array returned by convertEntryPointArgs(), or \c nullptr.
*/
void clearEntryPointArgs(char ** argv, char ** env) noexcept {
  if (argv != nullptr)
    freeStringArray(argv);

  if (env != nullptr)
    freeStringArray(env);
}
