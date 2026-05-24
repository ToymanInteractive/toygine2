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
  \file   log_backend.inl
  \brief  Inline implementation for \ref toy::log::Backend.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_LOG_BACKEND_INL_
#define INCLUDE_CORE_LOG_BACKEND_INL_

namespace toy::log {

inline void Backend::setSink(ISink * sink) noexcept {
  _sink = sink;
}

inline ISink * Backend::sink() const noexcept {
  return _sink;
}

inline void Backend::setTimestampPolicy(timestamp_fn_type fn) noexcept {
  _timestampFn = (fn != nullptr) ? fn : &Backend::defaultTimestamp;
}

inline Backend::timestamp_fn_type Backend::timestampPolicy() const noexcept {
  return _timestampFn;
}

template <typename... Args>
inline void Backend::push(const Metadata * meta, const Args &... args) noexcept {
  assert_message(meta != nullptr, "Backend::push: meta must be non-null");
  if (_sink == nullptr)
    return;

  Record record;
  record.meta      = meta;
  record.timestamp = _timestampFn();
  vformatTo(record.message, CStringView(meta->format), args...);
  _sink->write(record);
}

} // namespace toy::log

#endif // INCLUDE_CORE_LOG_BACKEND_INL_
