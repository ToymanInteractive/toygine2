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
  \file   format_argument.hpp
  \brief  Type-erased format argument for the runtime formatting path.

  Defines \ref toy::FormatArgument: a two-pointer aggregate that pairs a value pointer with a stateless formatter
  callback. Used internally by toy::makeVFormatArguments() and toy::makeVFormatArgs() to build argument arrays and
  \ref toy::FormatArguments collections consumed by toy::vformat() and toy::vformatTo().

  \note Included by core.hpp; do not include this file directly.
*/

#ifndef INCLUDE_CORE_FORMAT_ARGUMENT_HPP_
#define INCLUDE_CORE_FORMAT_ARGUMENT_HPP_

#include "format_context.hpp"

namespace toy {

/*!
  \struct FormatArgument

  \ingroup String

  \brief Single type-erased format argument carrying a value pointer and a formatter callback.

  Stores a pointer to the original argument value alongside a stateless formatter that knows the concrete type and can
  write a formatted representation to any \ref toy::FormatContext. Created internally by toy::makeVFormatArguments() and
  toy::makeVFormatArgs(); users rarely construct \c FormatArgument directly.

  \section features Key Features

  - **Type erasure**: Holds any formattable value without templates at the call site.
  - **No allocation**: Two-pointer aggregate stored inline in \ref toy::FormatArguments or \c std::array.
  - **noexcept formatter**: The formatFn must not throw.
  - **Lifetime coupling**: \a value must remain valid as long as the \c FormatArgument is used.

  \section usage Usage Example

  \code
  #include "core.hpp"

  toy::FixedString<32> buf;
  toy::FormatContext out{
    static_cast<void *>(&buf),
    [](void * ctx, const char * data, size_t n) noexcept {
      static_cast<toy::FixedString<32> *>(ctx)->append(data, n);
    }
  };
  int x = 42;
  toy::FormatArgument arg{
    static_cast<const void *>(&x),
    [](const void * v, toy::FormatContext & o) noexcept {
      toy::OStringStream<toy::FixedString<32>> s;
      s << *static_cast<const int *>(v);
      const auto & str = s.str();
      o.write(str.c_str(), str.size());
    }
  };
  arg.formatFn(arg.value, out);
  \endcode

  \section performance Performance Characteristics

  - **Construction**: O(1) constant time.
  - **formatFn call**: One indirect call; formatting cost depends on the argument type.
  - **Memory usage**: Two pointers (typically 16 bytes on 64-bit platforms).

  \section safety Safety Guarantees

  - **Exception safety**: \a formatFn is required to be \c noexcept.
  - **Lifetime**: \a value must remain valid for the duration of the format operation.
  - **Null safety**: Neither \a value nor \a formatFn may be null when the argument is dispatched.

  \sa FormatContext, FormatArguments
*/
struct FormatArgument {
  /// Pointer to the original argument value.
  const void * value;

  /// Callback that formats the value and writes it to \a output.
  void (*formatFn)(const void * value, FormatContext & output) noexcept;
};

} // namespace toy

#endif // INCLUDE_CORE_FORMAT_ARGUMENT_HPP_
