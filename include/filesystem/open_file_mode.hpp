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
  \file   open_file_mode.hpp
  \brief  File open mode enumeration for the filesystem module.

  Defines \ref toy::filesystem::OpenFileMode: how a file is opened (read-only, write, append, or read-write variants).
  Used by file open APIs to select semantics and platform flags.

  \note Included by filesystem.hpp; do not include this file directly.
*/

#ifndef INCLUDE_FILESYSTEM_OPEN_FILE_MODE_HPP_
#define INCLUDE_FILESYSTEM_OPEN_FILE_MODE_HPP_

namespace toy::filesystem {

/*!
  \enum OpenFileMode
  \brief Mode in which a file is opened: read, write, append, or read-write.

  Each value maps to platform-specific flags (e.g. \c O_RDONLY, \c O_WRONLY, \c O_APPEND).
  \c WriteEmpty and \c ReadWriteEmpty typically create or truncate the file; \c WriteAppend
  and \c ReadWriteAppend append; \c Read is read-only.
*/
enum class OpenFileMode {
  /// Open for reading only. File must exist.
  Read,
  /// Create or truncate and open for writing only.
  WriteEmpty,
  /// Open for writing at end of file; create if not present.
  WriteAppend,
  /// Open for reading and writing. File must exist.
  ReadWrite,
  /// Create or truncate and open for reading and writing.
  ReadWriteEmpty,
  /// Open for reading and writing at end of file; create if not present.
  ReadWriteAppend,
};

} // namespace toy::filesystem

#endif // INCLUDE_FILESYSTEM_OPEN_FILE_MODE_HPP_
