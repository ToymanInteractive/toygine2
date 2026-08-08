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
  \file   core.hpp
  \brief  Umbrella header for the engine core module.

  Single public entry point for the core module. It aggregates the module's public headers into namespace \ref toy;
  the remaining ones (chrono, fixed strings and vectors, formatting, hashing, logging, platform, and string utilities)
  join them as they are added. It currently carries assertion reporting and the opt-in bitwise operators for flag
  enumerations, alongside the standard-library aliases toy::size_t, the fixed-width integers toy::int8_t through
  toy::uint64_t, toy::to_underlying, and \ref toy::array.

  \note Include this header only; do not include internal headers directly.
*/

#ifndef INCLUDE_CORE_HPP_
#define INCLUDE_CORE_HPP_

#include <array>
#include <cstddef>
#include <cstdint>
#include <utility>

//----------------------------------------------------------------------------------------------------------------------

/*!
  \namespace toy

  \brief Root namespace containing all engine modules.
*/
namespace toy {

/// Unsigned integer type large enough to hold the size in bytes of any object; result type of \c sizeof
using std::size_t;

/// Signed integer type with width of exactly 8 bits and no padding bits
using std::int8_t;

/// Signed integer type with width of exactly 16 bits and no padding bits
using std::int16_t;

/// Signed integer type with width of exactly 32 bits and no padding bits
using std::int32_t;

/// Signed integer type with width of exactly 64 bits and no padding bits
using std::int64_t;

/// Unsigned integer type with width of exactly 8 bits and no padding bits
using std::uint8_t;

/// Unsigned integer type with width of exactly 16 bits and no padding bits
using std::uint16_t;

/// Unsigned integer type with width of exactly 32 bits and no padding bits
using std::uint32_t;

/// Unsigned integer type with width of exactly 64 bits and no padding bits
using std::uint64_t;

//--------------------------------------------------------------------------------------------------------------------

// (lvl 1)               Utility library https://en.cppreference.com/cpp/utility

/*!
  \brief Value of an enumerator in the underlying type of its enumeration; alias for std::to_underlying.

  \sa https://en.cppreference.com/cpp/utility/to_underlying
*/
using std::to_underlying;

//--------------------------------------------------------------------------------------------------------------------

// (lvl 1)               Containers library https://en.cppreference.com/w/cpp/container.html

/*!
  \brief Fixed-size array container with a compile-time constant number of elements; alias for std::array.

  \sa https://en.cppreference.com/w/cpp/container/array.html
*/
using std::array;

} // namespace toy

//--------------------------------------------------------------------------------------------------------------------

#include "core/assertion.hpp"
#include "core/bitwise_enum.hpp"

//--------------------------------------------------------------------------------------------------------------------

#include "core/bitwise_enum.inl"

#endif // INCLUDE_CORE_HPP_
