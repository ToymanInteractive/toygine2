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
  \file   assertion.hpp
  \brief  Assertion failure reporting and the handlers an application registers for it.

  Defines \ref toy::assertion, which reports a failed assert or assert_message check to the handler the application
  registers, and that handler decides what the check does next. Only builds that define \c TOYGINE_DEBUG compile
  reporting in.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_ASSERTION_HPP_
#define INCLUDE_CORE_ASSERTION_HPP_

/*!
  \namespace toy::assertion

  \brief Assertion failure reporting and the handlers that receive it.

  The application decides what a failed check does. setCallbacks() registers a handler that receives the failure
  description and answers with the \ref toy::assertion::Action the check takes, plus a second handler that receives
  stack frames. Registration is valid between initialize() and deInitialize(). With no handler registered, a failed
  check reports nothing and stops at the check.

  Typical wiring, from the composition root:

  \code
  #include <cstdio>

  #include "core.hpp"

  namespace {

  toy::assertion::Action reportAssertion(const char * assertionString) noexcept {
    std::fputs(assertionString, stderr);

    return toy::assertion::Action::Break;
  }

  } // namespace

  int main() {
    toy::assertion::initialize();
    toy::assertion::setCallbacks(reportAssertion, nullptr);

    // Run the application; in a debug build a failed check reaches reportAssertion() and then breaks.

    toy::assertion::deInitialize();

    return 0;
  }
  \endcode

  \note A check that must fail the build uses \c static_assert.
*/
namespace toy::assertion {

/*!
  \enum Action
  \brief What a failed check does once its failure is reported.

  Returned by the registered \ref toy::assertion::AssertionCallback and carried out by the \c assert and
  \c assert_message macros at the failing call site.
*/
enum class Action : uint8_t {
  Break,        ///< Stops at the failed check: a debugger breaks there, and without one the process ends.
  Ignore,       ///< Continues past the failed check.
  IgnoreAlways, ///< Continues and reports no later failure at the same call site; acts as Ignore on consoles.
  Abort,        ///< Ends the process before toy::assertion::assertion() returns.
};

/*!
  \brief Handler invoked on a failed check; receives one preformatted failure description.

  \param assertionString Description built from the failed expression, its message, and the source location. Valid only
                         for the duration of the call.

  \return What the failed check does next.

  \note Registered with setCallbacks(); a null pointer disables reporting.
  \note Declared \c noexcept: the handler runs while a failure is already being reported.

  \sa \ref toy::assertion::StackWalkCallback
*/
using AssertionCallback = Action (*)(const char * assertionString) noexcept;

/*!
  \brief Handler invoked once per stack frame while a trace is walked.

  \param stackFrameString One formatted frame. Valid only for the duration of the call.

  \note Declared \c noexcept: frames are delivered while a failure is already being reported.

  \sa \ref toy::assertion::AssertionCallback
*/
using StackWalkCallback = void (*)(const char * stackFrameString) noexcept;

/*!
  \brief Prepares assertion reporting; call from the composition root before any check can fail.

  \post No handler is registered; setCallbacks() and, in a debug build, assertion() may be called.

  \sa deInitialize(), setCallbacks()
*/
void initialize() noexcept;

/*!
  \brief Releases assertion reporting and clears both registered handlers.

  \pre initialize() must have been called.
  \post No handler is registered; a failed check reports nothing until initialize() runs again.

  \sa initialize()
*/
void deInitialize() noexcept;

/*!
  \brief Registers the failure handler and the stack-frame handler.

  Replaces both registrations at once; \c nullptr clears the corresponding handler.

  \param assertionCallback Invoked on a failed check, or \c nullptr to report nothing.
  \param stackWalkCallback Invoked per stack frame while a trace is walked, or \c nullptr to skip the trace.

  \pre initialize() must have been called.
  \post Subsequent assertion() calls route through \a assertionCallback.

  \sa \ref toy::assertion::AssertionCallback, \ref toy::assertion::StackWalkCallback
*/
void setCallbacks(AssertionCallback assertionCallback, StackWalkCallback stackWalkCallback) noexcept;

#ifdef TOYGINE_DEBUG

/*!
  \brief Reports a failed check through the registered \ref toy::assertion::AssertionCallback.

  Called by the \c assert and \c assert_message macros with the context they capture at the failing call site, never
  directly.

  \param code         Failed expression as written at the call site (e.g. \c "pointer != nullptr").
  \param message      Reason the condition must hold, in human-readable form, or \c nullptr when the call site supplies
                      none.
  \param fileName     Source file of the failed check.
  \param functionName Enclosing function of the failed check.
  \param lineNumber   Source line of the failed check.

  \return What the failed check does next: the answer of the registered handler, \ref toy::assertion::Action::Break
          with no handler registered, and \ref toy::assertion::Action::Ignore for a failure raised while another is
          still being reported. Never \ref toy::assertion::Action::Abort, which ends the process inside the call.

  \pre initialize() must have been called.
  \pre \a code, \a fileName, and \a functionName are non-null and outlive the call.

  \note Declared only when \c TOYGINE_DEBUG is defined. Every other build has no assertion() to call: the assert and
        assert_message macros keep their arguments in unevaluated \c sizeof operands, so a failed check costs nothing
        in a shipping binary.

  \sa setCallbacks()
*/
Action assertion(const char * code, const char * message, const char * fileName, const char * functionName,
                 size_t lineNumber) noexcept;

#endif // TOYGINE_DEBUG

/*!
  \brief Fails the build when reached during constant evaluation; does nothing at runtime.

  Not \c constexpr, so a call inside a constant expression makes that expression non-constant, and the compiler reports
  the enclosing \c constexpr or \c consteval evaluation as the error. Place it in a branch that must be unreachable at
  compile time, beside the assertion() call covering the same case at runtime.

  \note Only constant evaluation is affected. The same call in ordinary runtime code compiles and does nothing.

  \sa assertion()
*/
inline void assertCompileTimeError() noexcept {
  // Empty on purpose: the call fails constant evaluation because the function is not constexpr.
}

} // namespace toy::assertion

#endif // INCLUDE_CORE_ASSERTION_HPP_
