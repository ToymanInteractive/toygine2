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
  \file   assertion.hpp
  \brief  Assertion system for runtime and compile-time validation of engine invariants.
*/

#ifndef INCLUDE_CORE_ASSERTION_HPP_
#define INCLUDE_CORE_ASSERTION_HPP_

/*!
  \namespace toy::assertion
  \brief Assertion utilities for validating engine invariants and runtime conditions.

  This namespace provides a configurable assertion system that supports both runtime assertions (in debug builds) and
  compile-time assertions (in constexpr contexts). The system allows custom callback registration for handling assertion
  failures and stack trace generation.

  \section features Key Features

  - 🔧 **Configurable Callbacks**: Custom assertion and stack walk handlers
  - 🎯 **Debug/Release Support**: Runtime assertions only in debug builds
  - ⚡ **Compile-Time Assertions**: constexpr_assert for constexpr contexts
  - 🛡️ **Exception Safety**: Compile-time assertions throw exceptions for validation failures

  \section usage Usage Example

  \code
  #include "core/assertion.hpp"

  // Initialize assertion system
  toy::assertion::initialize();

  // Set custom callbacks
  toy::assertion::setCallbacks(myAssertionHandler, myStackWalkHandler);

  // Runtime assertion (debug builds only)
  #ifdef _DEBUG
  toy::assertion::assertion("value > 0", nullptr, __FILE__, __FUNCTION__, __LINE__);
  #endif

  // Compile-time assertion
  constexpr int value = 42;
  toy::assertion::constexpr_assert(value > 0, "value must be positive");

  // Cleanup
  toy::assertion::deInitialize();
  \endcode
*/
namespace toy::assertion {

/*!
  \brief Callback function type for assertion failure handling.

  This callback is invoked when an assertion fails. The callback receives a formatted assertion string containing the
  assertion code, message (if any), file name, function name, and line number.

  \param assertionString The formatted assertion string describing the failure.

  \return \c true if the assertion should be ignored, \c false otherwise.

  \note The callback should not throw exceptions.
  \note The assertion string pointer is valid only during the callback invocation.
*/
using AssertionCallback = bool (*)(const char * assertionString);

/*!
  \brief Callback function type for stack trace generation.

  This callback is invoked to output stack trace information. The callback receives a formatted string containing stack
  frame information.

  \param stackFrameString The formatted string for a single stack frame.

  \note The callback should not throw exceptions.
  \note The stack frame string pointer is valid only during the callback invocation.
*/
using StackWalkCallback = void (*)(const char * stackFrameString);

/*!
  \brief Initializes the assertion system.

  This function must be called before using any assertion functionality. It prepares the internal state of the
  assertion system for use.

  \post The assertion system is ready for use.
  \post All callbacks are reset to nullptr.

  \note This function is safe to call multiple times.
  \note Must be paired with a call to \ref deInitialize().

  \sa deInitialize()
  \sa setCallbacks()
*/
void initialize();

/*!
  \brief Shuts down the assertion system.

  This function cleans up the assertion system and should be called when assertions are no longer needed. It resets all
  callbacks and releases any resources held by the assertion system.

  \post The assertion system is no longer active.
  \post All callbacks are reset to \c nullptr.

  \note This function is safe to call multiple times.
  \note Should be called after \ref initialize() when assertions are no longer needed.

  \sa initialize()
*/
void deInitialize();

/*!
  \brief Sets the assertion and stack walk callbacks.

  This function registers custom callback functions for handling assertion failures and stack trace generation. The
  callbacks are invoked when assertions are triggered, allowing custom error handling and logging.

  \param assertionCallback The callback function to invoke on assertion failure. Can be nullptr to disable.
  \param stackWalkCallback The callback function to invoke for stack trace output. Can be nullptr to disable.

  \pre The assertion system must be initialized (via \ref initialize()).

  \note Callbacks can be set to nullptr to disable them.
  \note Callbacks are invoked synchronously during assertion processing.
  \note The assertion callback should return \c true to ignore the assertion, \c false otherwise.

  \sa initialize()
  \sa AssertionCallback
  \sa StackWalkCallback
*/
void setCallbacks(AssertionCallback assertionCallback, StackWalkCallback stackWalkCallback);

#ifdef _DEBUG

/*!
  \brief Reports an assertion failure with detailed context information.

  This function is called when an assertion fails in debug builds. It formats the assertion information and invokes the
  registered assertion callback (if set). This function is typically invoked by assertion macros, not directly by user
  code.

  \param code         The assertion expression as a string (e.g., "value > 0").
  \param message      Optional custom message string, or nullptr if no message is provided.
  \param fileName     The source file name where the assertion failed.
  \param functionName The function name where the assertion failed.
  \param lineNumber   The line number where the assertion failed.

  \pre The assertion system must be initialized (via \ref initialize()).

  \note This function is only available in debug builds (_DEBUG defined).
  \note In release builds, this function is replaced with an inline no-op.
  \note The function invokes the registered assertion callback if one is set.

  \sa setCallbacks()
  \sa constexpr_assert()
*/
void assertion(const char * code, const char * message, const char * fileName, const char * functionName,
               size_t lineNumber);

#else // _DEBUG

/*!
  \brief No-op assertion function for release builds.

  In release builds, assertions are disabled and this function does nothing. This allows assertion code to compile
  without overhead in release builds.

  \note This function is only available in release builds (when _DEBUG is not defined).
  \note This function has no effect and is completely optimized away by the compiler.
*/
inline void assertion(const char *, const char *, const char *, const char *, size_t) {
  // Intentionally empty - no-op in release builds
}

#endif // _DEBUG

/*!
  \brief Compile-time assertion function for constexpr contexts.

  This function provides assertion capabilities in constexpr contexts where regular runtime assertions cannot be used.
  When the \a condition is false, it throws a std::invalid_argument exception with the provided \a message, causing
  compilation to fail if evaluated at compile time.

  \param condition The condition to check. Must be true for the assertion to pass.
  \param message   The error message to include in the exception if the assertion fails.

  \throws std::invalid_argument If \a condition is false, with \a message as the error description.

  \pre The \a message pointer must point to a valid null-terminated string.

  \note This function is designed for use in constexpr contexts where traditional assertions are not available.
  \note Unlike regular assertions, this function always throws an exception in both debug and release builds when the
        condition fails.
  \note When evaluated at compile time, a failed assertion will cause a compilation error.
  \note When evaluated at runtime, a failed assertion will throw an exception.

  \section usage Usage Example

  \code
  #include "core/assertion.hpp"

  template <size_t N>
  constexpr size_t validateSize() {
    toy::assertion::constexpr_assert(N > 0, "Size must be greater than zero");
    return N;
  }

  constexpr auto size = validateSize<10>();  // OK
  // constexpr auto invalid = validateSize<0>();  // Compilation error
  \endcode

  \sa assert
  \sa assert_message
  \sa assertion()
*/
constexpr void constexpr_assert(bool condition, const char * message) {
  if (!condition)
    throw std::invalid_argument(message ? message : "constexpr_assert failed");
}

} // namespace toy::assertion

#endif // INCLUDE_CORE_ASSERTION_HPP_
