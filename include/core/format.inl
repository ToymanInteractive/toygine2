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
  \file   format.inl
  \brief  Inline implementations for toy::format() and toy::vformat().

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_INL_
#define INCLUDE_CORE_FORMAT_INL_

#include "detail/format_arg.hpp"
#include "detail/format_fixed_sink.hpp"
#include "detail/format_string.hpp"
#include "detail/format_to.hpp"

namespace toy {

template <size_t BufferSize, typename... Args>
constexpr FixedString<BufferSize> format(std::type_identity_t<detail::FormatString<Args...>> fmt,
                                         const Args &... args) noexcept
  requires(BufferSize >= 1) {
  FixedString<BufferSize> result;
  result.resize_and_overwrite(result.capacity(), [&](char * out, size_t capacity) {
    detail::FormatFixedSink sink{out, capacity, 0, false};

    if constexpr (sizeof...(Args) == 0)
      static_cast<void>(detail::walk(sink, fmt.get(), span<const detail::FormatArg>{}));
    else {
      const detail::FormatArg storage[sizeof...(Args)] = {detail::packArg(args)...};
      static_cast<void>(detail::walk(sink, fmt.get(), span<const detail::FormatArg>{storage}));
    }

    assert_message(!sink.overflow, "formatted output exceeds the buffer capacity");
    return sink.size;
  });

  return result;
}

template <size_t BufferSize, typename... Args>
constexpr FixedString<BufferSize> vformat(StringView pattern, const Args &... args) noexcept
  requires(BufferSize >= 1) {
  FixedString<BufferSize> result;
  result.resize_and_overwrite(result.capacity(), [&](char * out, size_t capacity) {
    detail::FormatFixedSink sink{out, capacity, 0, false};

    if constexpr (sizeof...(Args) == 0)
      static_cast<void>(detail::walk(sink, pattern, span<const detail::FormatArg>{}));
    else {
      const detail::FormatArg storage[sizeof...(Args)] = {detail::packArg(args)...};
      static_cast<void>(detail::walk(sink, pattern, span<const detail::FormatArg>{storage}));
    }

    assert_message(!sink.overflow, "formatted output exceeds the buffer capacity");
    return sink.size;
  });

  return result;
}

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_INL_
