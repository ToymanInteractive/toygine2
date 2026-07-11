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

  Defines the public core surface: assertions, callbacks pool, chrono, fixed strings and vectors, formatting, general
  utilities, hashing, logging, output string stream, platform, and string utilities.

  \note Include this header only; do not include internal headers directly.
*/

#ifndef INCLUDE_CORE_HPP_
#define INCLUDE_CORE_HPP_

#include <cstdint>

//----------------------------------------------------------------------------------------------------------------------

/*!
  \namespace toy

  \brief Root namespace containing all engine modules.
*/
namespace toy {

/// signed integer type with width of exactly 8 bits and no padding bits
using std::int8_t;

/// signed integer type with width of exactly 16 bits and no padding bits
using std::int16_t;

/// signed integer type with width of exactly 32 bits and no padding bits
using std::int32_t;

/// signed integer type with width of exactly 64 bits and no padding bits
using std::int64_t;

/// unsigned integer type with width of exactly 8 bits and no padding bits
using std::uint8_t;

/// unsigned integer type with width of exactly 16 bits and no padding bits
using std::uint16_t;

/// unsigned integer type with width of exactly 32 bits and no padding bits
using std::uint32_t;

/// unsigned integer type with width of exactly 64 bits and no padding bits
using std::uint64_t;

} // namespace toy

#endif // INCLUDE_CORE_HPP_
