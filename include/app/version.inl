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
  \file   version.inl
  \brief  Implementation of application version management utilities.
*/

#ifndef INCLUDE_APP_VERSION_INL_
#define INCLUDE_APP_VERSION_INL_

namespace toy::app {

constexpr bool operator==(const Version & lhs, const Version & rhs) noexcept {
  return lhs.major == rhs.major && lhs.minor == rhs.minor && lhs.maintenance == rhs.maintenance
         && lhs.revision == rhs.revision;
}

constexpr strong_ordering operator<=>(const Version & lhs, const Version & rhs) noexcept {
  if (auto cmp = lhs.major <=> rhs.major; cmp != strong_ordering::equal) {
    return cmp;
  }

  if (auto cmp = lhs.minor <=> rhs.minor; cmp != strong_ordering::equal) {
    return cmp;
  }

  if (auto cmp = lhs.maintenance <=> rhs.maintenance; cmp != strong_ordering::equal) {
    return cmp;
  }

  return lhs.revision <=> rhs.revision;
}

} // namespace toy::app

#endif // INCLUDE_APP_VERSION_INL_
