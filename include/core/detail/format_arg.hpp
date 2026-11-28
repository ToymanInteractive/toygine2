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
  \file   format_arg.hpp
  \brief  Type-erased format arguments and the packers that build them.

  Defines \ref toy::detail::FormatArg, one tagged value per packable argument, and the toy::detail::packArg() overloads
  that pack a character or a truth value into it. Used by any caller that assembles an argument pack for a later format
  walk by hand.
*/

#ifndef INCLUDE_CORE_DETAIL_FORMAT_ARG_HPP_
#define INCLUDE_CORE_DETAIL_FORMAT_ARG_HPP_

#include "format_arg_type.hpp"

namespace toy::detail {

/*!
  \struct FormatArg
  \brief One format argument as a tag plus the scalar value the packer reduced it to.

  The toy::detail::packArg() overloads build the value at the call site; the tag names the union member that holds it.
  The packers reduce argument types to tags as follows:

  | Argument type | Tag                                        | Rule           |
  |---------------|--------------------------------------------|----------------|
  | \c char       | \ref toy::detail::FormatArgType::charValue | exact overload |
  | \c bool       | \ref toy::detail::FormatArgType::boolValue | exact overload |

  \section format_arg_features Key Features

  * **Tagged value**: one \ref toy::detail::FormatArgType selects the union member that holds the value.
  * **Trivially copyable**: a tag and a plain scalar, so a pack copies by \c memcpy and rides the stack.
  * **Fixed footprint**: two bytes, the tag and the widest union member.
  * **Constexpr support**: packing a value evaluates in a constant expression.

  \section format_arg_usage Usage Example

  \code
  #include "core.hpp"

  constexpr toy::detail::FormatArg packed = toy::detail::packArg(true);
  static_assert(packed.type == toy::detail::FormatArgType::boolValue && packed.boolean,
                "a truth value packs to its slot");
  \endcode

  \section format_arg_performance Performance Characteristics

  * **Construction**: one tag store plus one scalar store, constant time.
  * **Memory usage**: two bytes for the whole value.

  \section format_arg_safety Safety Guarantees

  * **Type safety**: reading a union member the tag does not name reads an inactive member.
  * **Exception safety**: no operation throws; exceptions are off in the build.

  \sa \ref toy::detail::FormatArgType
*/
struct FormatArg {
  /// Which union member holds the packed value.
  FormatArgType type = FormatArgType::none;

  /// The value the tag names; every other member is inactive.
  union {
    /// A single byte, packed from \c char.
    char character;

    /// A truth value, packed from \c bool.
    bool boolean;
  };
};

/*!
  \brief Packs a single byte to the character slot.

  \param value Byte to pack.

  \return The argument tagged \ref toy::detail::FormatArgType::charValue.
*/
constexpr FormatArg packArg(char value) noexcept;

/*!
  \brief Packs a truth value to the boolean slot.

  \param value Value to pack.

  \return The argument tagged \ref toy::detail::FormatArgType::boolValue.
*/
constexpr FormatArg packArg(bool value) noexcept;

/*!
  \brief Deleted: any other argument type, so an implicit conversion to \c bool cannot select the boolean packer.

  A pointer, a pointer to member, a null pointer constant, or a class with such a conversion would otherwise pack as a
  truth value.

  \tparam T Type of the rejected argument.

  \param value Argument the pack rejects.

  \warning Calling this overload is a compile error; only \c char and \c bool have a pack.
*/
template <typename T>
constexpr FormatArg packArg(T value) noexcept = delete;

} // namespace toy::detail

#include "format_arg.inl"

#endif // INCLUDE_CORE_DETAIL_FORMAT_ARG_HPP_
