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
  \file   core_application.inl
  \brief  Inline implementations for \ref toy::application::CoreApplication accessors.

  \note Included by application.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_APPLICATION_CORE_APPLICATION_INL_
#define INCLUDE_APPLICATION_CORE_APPLICATION_INL_

namespace toy::application {

inline const Version & CoreApplication::version() const noexcept {
  return _version;
}

inline void CoreApplication::setVersion(Version version) noexcept {
  _version = version;
}

inline const char * CoreApplication::argument(size_t index) const noexcept {
  assert_message(index < _argumentsCount, "Invalid argument index");

  if (index >= _argumentsCount || _argumentsVector == nullptr)
    return nullptr;

  return _argumentsVector[index];
}

inline char ** CoreApplication::arguments() const noexcept {
  return _argumentsVector;
}

inline size_t CoreApplication::argumentsCount() const noexcept {
  return _argumentsCount;
}

inline void CoreApplication::setArguments(int argc, char ** argv) noexcept {
  if (argc > 0)
    _argumentsCount = static_cast<size_t>(argc);

  _argumentsVector = argv;
}

} // namespace toy::application

#endif // INCLUDE_APPLICATION_CORE_APPLICATION_INL_
