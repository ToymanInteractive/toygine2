//
// Copyright (c) 2025 Toyman Interactive
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
  \file   assertion_macro_gcc_clang.hpp
  \brief  Debug and Release assertion macro implementations for GCC/Clang platforms.
*/

#ifndef SRC_PLATFORMS_COMMON_ASSERTION_MACRO_GCC_CLANG_HPP_
#define SRC_PLATFORMS_COMMON_ASSERTION_MACRO_GCC_CLANG_HPP_

#if defined(assert)
// Undefine any existing assert macro to avoid conflicts
#undef assert
#endif

#ifdef _DEBUG

/*!
  \def assert(expression)
  \brief Debug assertion macro for runtime \a expression checking.

  This macro provides runtime assertion checking in debug builds. It evaluates the given \a expression and triggers an
  assertion failure if the \a expression is false. In release builds, this macro expands to nothing.

  \param expression The boolean expression to evaluate. Must be convertible to bool.

  \note This macro is only active in debug builds (_DEBUG defined).
  \note In release builds, this macro expands to ((void)0) and has no effect.
  \note Assertion failures will call toy::assertion::assertion with file, function, and line information.
  \note The \a expression is evaluated only once and should not have side effects.

  \warning Do not rely on the \a expression being evaluated in release builds.
  \warning The \a expression should not have side effects as it may not be evaluated in release builds.

  \see assert_message
  \see toy::assertion::assertion
*/
#define assert(expression)                                                                                             \
  do {                                                                                                                 \
    if (std::is_constant_evaluated()) {                                                                                \
      toy::assertion::constexpr_assert(expression, #expression);                                                       \
    } else if (!(expression)) {                                                                                        \
      toy::assertion::assertion(#expression, nullptr, __FILE__, __PRETTY_FUNCTION__, __LINE__);                        \
    }                                                                                                                  \
  } while (0)

/*!
  \def assert_message(expression, message)
  \brief Debug assertion macro with custom \a message for runtime \a expression checking.

  This macro provides runtime assertion checking in debug builds with a custom error \a message. It evaluates the given
  \a expression and triggers an assertion failure with the provided \a message if the \a expression is false. In release
  builds, this macro expands to nothing.

  \param expression The boolean expression to evaluate. Must be convertible to bool.
  \param message    A custom error message to display on assertion failure. Must be a C string literal.

  \note This macro is only active in debug builds (_DEBUG defined).
  \note In release builds, this macro expands to ((void)0) and has no effect.
  \note Assertion failures will call toy::assertion::assertion with file, function, line, and custom \a message.
  \note The \a expression is evaluated only once and should not have side effects.
  \note The \a message parameter is passed as-is to the assertion handler.

  \warning Do not rely on the \a expression being evaluated in release builds.
  \warning The \a expression should not have side effects as it may not be evaluated in release builds.
  \warning The \a message should be a C string literal for optimal performance.

  \see assert
  \see toy::assertion::assertion
*/
#define assert_message(expression, message)                                                                            \
  do {                                                                                                                 \
    if (std::is_constant_evaluated()) {                                                                                \
      toy::assertion::constexpr_assert(expression, message);                                                           \
    } else if (!(expression)) {                                                                                        \
      toy::assertion::assertion(#expression, message, __FILE__, __PRETTY_FUNCTION__, __LINE__);                        \
    }                                                                                                                  \
  } while (0)

#else // _DEBUG

/*!
  \def assert(expression)
  \brief Release build assertion macro (no-op).

  In release builds, this macro expands to nothing and has no effect. The \a expression is not evaluated.

  \param expression The boolean expression (ignored in release builds).

  \note This macro is only defined in release builds (!_DEBUG).
  \note The \a expression parameter is ignored and not evaluated.
  \note This ensures zero overhead in release builds.

  \see assert_message
*/
#define assert(expression) ((void)0)

/*!
  \def assert_message(expression, message)
  \brief Release build assertion macro with \a message (no-op).

  In release builds, this macro expands to nothing and has no effect. Neither the \a expression nor the \a message are
  evaluated.

  \param expression The boolean expression (ignored in release builds).
  \param message    The custom error message (ignored in release builds).

  \note This macro is only defined in release builds (!_DEBUG).
  \note Both parameters are ignored and not evaluated.
  \note This ensures zero overhead in release builds.

  \see assert
*/
#define assert_message(expression, message) ((void)0)

#endif // _DEBUG

#endif // SRC_PLATFORMS_COMMON_ASSERTION_MACRO_GCC_CLANG_HPP_
