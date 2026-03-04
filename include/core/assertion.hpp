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
  \brief  Assertion system: callbacks, initialize/shutdown, and assertion() entry point.

  Used by \c assert and \c assert_message macros. In debug builds assertion() formats failure context and invokes the
  registered callback; in release it is a no-op. Supports custom stack-walk callback for trace output.
*/

#ifndef INCLUDE_CORE_ASSERTION_HPP_
#define INCLUDE_CORE_ASSERTION_HPP_

/*!
  \namespace toy::assertion
  \brief Configurable assertion failure handling and optional stack trace.

  Call initialize() before using assertion macros; register callbacks with setCallbacks(). assertion() is invoked by
  \c assert / \c assert_message when a check fails (debug only). assertCompileTimeError() forces a compile-time error in
  unreachable branches.

  \section features Key Features

  - **Callbacks**: AssertionCallback for failure handling, StackWalkCallback for stack frames.
  - **Debug-only**: assertion() does real work only when \c _DEBUG is defined; otherwise no-op.
*/
namespace toy::assertion {

/*!
  \brief Called on assertion failure; receives a single formatted string (expression, message, file, function, line).

  \param assertionString Formatted failure description; valid only for the duration of the callback.

  \return \c true to treat the assertion as ignored (caller may continue), \c false otherwise.

  \note Must not throw. Prefer setCallbacks() to register after initialize().
*/
using AssertionCallback = bool (*)(const char * assertionString);

/*!
  \brief Called for each stack frame when generating a trace; receives one formatted frame string.

  \param stackFrameString Formatted frame; valid only for the duration of the callback.

  \note Must not throw.
*/
using StackWalkCallback = void (*)(const char * stackFrameString);

/*!
  \brief Prepares the assertion system; call before using assertion macros or setCallbacks().

  \post Callbacks are reset to \c nullptr; system is ready for setCallbacks() and assertion().

  \note Safe to call multiple times.

  \sa deInitialize(), setCallbacks()
*/
void initialize();

/*!
  \brief Shuts down the assertion system and clears callbacks.

  \post Callbacks are \c nullptr; no resources held.

  \note Safe to call multiple times.

  \sa initialize()
*/
void deInitialize();

/*!
  \brief Registers the assertion and stack-walk callbacks.

  \param assertionCallback Invoked on assertion failure; may be \c nullptr to disable.
  \param stackWalkCallback  Invoked per stack frame when walking; may be \c nullptr to disable.

  \pre initialize() must have been called.

  \note Assertion callback returns \c true to ignore the failure, \c false otherwise. Callbacks must not throw.

  \sa initialize(), AssertionCallback, StackWalkCallback
*/
void setCallbacks(AssertionCallback assertionCallback, StackWalkCallback stackWalkCallback);

#ifdef _DEBUG

/*!
  \brief Handles a failed assertion: formats \a code, \a message, \a fileName, \a functionName, \a lineNumber and
         invokes the registered AssertionCallback if set.

  Called by the \c assert / \c assert_message macros. Only compiled when \c _DEBUG is defined.

  \param code         Assertion expression as string (e.g. "x != nullptr").
  \param message      Optional message or \c nullptr.
  \param fileName     Source file where the assertion failed.
  \param functionName Function name where the assertion failed.
  \param lineNumber   Source line number.

  \pre initialize() must have been called.

  \sa setCallbacks()
*/
void assertion(const char * code, const char * message, const char * fileName, const char * functionName,
               size_t lineNumber);

#else // _DEBUG

/*!
  \brief No-op in release builds; provided so call sites compile without \c _DEBUG.

  Parameters are unused. The call is optimized away.

  \param code         Assertion expression as string (e.g. "x != nullptr").
  \param message      Optional message or \c nullptr.
  \param fileName     Source file where the assertion failed.
  \param functionName Function name where the assertion failed.
  \param lineNumber   Source line number.
*/
inline void assertion([[maybe_unused]] const char * code, [[maybe_unused]] const char * message,
                      [[maybe_unused]] const char * fileName, [[maybe_unused]] const char * functionName,
                      [[maybe_unused]] size_t lineNumber) {
  // Intentionally empty - no-op in release builds
}

#endif // _DEBUG

/*!
  \brief Intentionally causes a compile-time error when instantiated or evaluated in a constexpr context.

  Use in unreachable template branches or constexpr branches that must never be taken (e.g. exhaustive switch
  default). Marked \c [[noreturn]]; does not return.

  \note For valid code this function should never be called at runtime.
*/
inline void assertCompileTimeError() noexcept {
  // Intentionally cause a compile-time error
}

} // namespace toy::assertion

#endif // INCLUDE_CORE_ASSERTION_HPP_
