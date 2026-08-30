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

  Single public entry point for the core module. It aggregates the module's public headers into namespace \ref toy,
  which so far provide assertion reporting, the opt-in bitwise operators for flag enumerations, and the string-like
  concept. From the standard library it re-exports toy::size_t, the fixed-width integers toy::int8_t through
  toy::uint64_t, toy::to_underlying, and toy::array. The header also defines the assert and assert_message macros, which
  call into that reporting.

  \note Include this header only; do not include internal headers directly.
*/

#ifndef INCLUDE_CORE_HPP_
#define INCLUDE_CORE_HPP_

#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

//----------------------------------------------------------------------------------------------------------------------

/*!
  \namespace toy

  \brief Root namespace for every engine module.
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

#if defined(assert)
// Undefine any existing assert macro to avoid conflicts
#undef assert
#endif

#ifdef _DEBUG

/*!
  \def __FUNC_SIGNATURE__
  \brief Name of the enclosing function, as the active compiler spells it.

  Expands to \c __FUNCSIG__ on MSVC, to \c __PRETTY_FUNCTION__ on GCC and Clang, and to \c __func__ elsewhere, so a
  failure report carries the full signature where the compiler offers one.

  \note Defined only when \c _DEBUG is defined, and expanded by assert and assert_message; call sites never spell it.
*/
#if defined(_MSC_VER)
#define __FUNC_SIGNATURE__ __FUNCSIG__
#elif defined(__GNUC__) || defined(__clang__)
#define __FUNC_SIGNATURE__ __PRETTY_FUNCTION__
#else
#define __FUNC_SIGNATURE__ __func__
#endif

/*!
  \def assert
  \brief Checks \a expression in a debug build and reports a failure with its source location.

  Passes the expression as written, the file, the enclosing function, and the line to toy::assertion::assertion(); the
  registered \ref toy::assertion::AssertionCallback decides what the failure does. A failure during constant evaluation
  calls toy::assertion::assertCompileTimeError() instead, which makes the enclosing constant expression non-constant and
  fails the build.

  \param expression Condition that must hold. Evaluated once, and only in a debug build.

  \pre toy::assertion::initialize() has been called; without it a failure reports nothing.

  \note Replaces the \c assert macro of \c \<cassert\>, which this header undefines first.
  \note Compiled in only when \c _DEBUG is defined. Every other build expands to \c ((void)0), so \a expression is
        never evaluated and a side effect written inside it is lost.

  \sa assert_message
*/
#define assert(expression)                                                                                             \
  do {                                                                                                                 \
    if (!(expression)) {                                                                                               \
      if (std::is_constant_evaluated()) {                                                                              \
        toy::assertion::assertCompileTimeError();                                                                      \
      } else {                                                                                                         \
        toy::assertion::assertion(#expression, nullptr, __FILE__, __FUNC_SIGNATURE__, __LINE__);                       \
      }                                                                                                                \
    }                                                                                                                  \
  } while (0)

/*!
  \def assert_message
  \brief Checks \a expression in a debug build and reports a failure with \a message and its source location.

  Routes the failure exactly as assert does, with \a message carried into the description so that a report reads without
  opening the source.

  \param expression Condition that must hold. Evaluated once, and only in a debug build.
  \param message    Reason the condition must hold, in human-readable form, or \c nullptr for none. Read during the call
                    only.

  \pre toy::assertion::initialize() has been called; without it a failure reports nothing.

  \note Compiled in only when \c _DEBUG is defined. Every other build expands to \c ((void)0), so neither argument is
        evaluated.

  \sa assert
*/
#define assert_message(expression, message)                                                                            \
  do {                                                                                                                 \
    if (!(expression)) {                                                                                               \
      if (std::is_constant_evaluated()) {                                                                              \
        toy::assertion::assertCompileTimeError();                                                                      \
      } else {                                                                                                         \
        toy::assertion::assertion(#expression, message, __FILE__, __FUNC_SIGNATURE__, __LINE__);                       \
      }                                                                                                                \
    }                                                                                                                  \
  } while (0)

#else // _DEBUG

#define assert(expression) ((void)0)

#define assert_message(expression, message) ((void)0)

#endif // _DEBUG

//--------------------------------------------------------------------------------------------------------------------

#include "core/assertion.hpp"
#include "core/bitwise_enum.hpp"
#include "core/string_like.hpp"

//--------------------------------------------------------------------------------------------------------------------

#include "core/bitwise_enum.inl"

#endif // INCLUDE_CORE_HPP_
