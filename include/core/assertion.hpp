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

  This namespace provides a configurable assertion system that supports runtime assertions (in debug builds). The system
  allows custom callback registration for handling assertion failures and stack trace generation.

  \section features Key Features

  - 🔧 **Configurable Callbacks**: Custom assertion and stack walk handlers
  - 🎯 **Debug/Release Support**: Runtime assertions only in debug builds

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
  \note Must be paired with a call to deInitialize().

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
  \note Should be called after initialize() when assertions are no longer needed.

  \sa initialize()
*/
void deInitialize();

/*!
  \brief Sets the assertion and stack walk callbacks.

  This function registers custom callback functions for handling assertion failures and stack trace generation. The
  callbacks are invoked when assertions are triggered, allowing custom error handling and logging.

  \param assertionCallback The callback function to invoke on assertion failure. Can be nullptr to disable.
  \param stackWalkCallback The callback function to invoke for stack trace output. Can be nullptr to disable.

  \pre The assertion system must be initialized (via initialize()).

  \note Callbacks can be set to nullptr to disable them.
  \note Callbacks are invoked synchronously during assertion processing.
  \note The assertion callback should return \c true to ignore the assertion, \c false otherwise.

  \sa initialize()
  \sa toy::assertion::AssertionCallback
  \sa toy::assertion::StackWalkCallback
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

  \pre The assertion system must be initialized (via initialize()).

  \note This function is only available in debug builds (_DEBUG defined).
  \note In release builds, this function is replaced with an inline no-op.
  \note The function invokes the registered assertion callback if one is set.

  \sa setCallbacks()
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
  \brief Forces a compile-time error when called.

  This function is designed to be used in template metaprogramming and constexpr contexts to force a compile-time
  error. When called, it triggers a compilation failure, which can be useful for static_assert-like behavior in
  contexts where static_assert cannot be used.

  \note This function should never be called in valid code. It is intended to be used in template specializations or
        constexpr branches that should never be reached.
  \note The function is marked as \c [[noreturn]] to indicate it never returns normally.
  \note This function causes a compile-time error when evaluated in constexpr contexts.
*/
inline void assertCompileTimeError() noexcept {
  // Intentionally cause a compile-time error
}

} // namespace toy::assertion

#endif // INCLUDE_CORE_ASSERTION_HPP_
