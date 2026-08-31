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
  \brief  Debug-only precondition check for the runner's own code.

  Defines the \c TOY_TEST_ASSERT macro and toy::test::detail::assertionFailed(): what a violated precondition does
  inside the runner. Written without \c <cassert> so the runner keeps building on targets that ship no hosted headers.

  \note Reached through toy_test.hpp; included directly only by the runner's own headers and by the unit test for this
        type.
*/

#ifndef INCLUDE_TESTS_RUNNER_ASSERTION_HPP_
#define INCLUDE_TESTS_RUNNER_ASSERTION_HPP_

// MSVC declares __debugbreak() here. Every other toolchain uses a compiler builtin and needs no header, which is what
// keeps the runner buildable where no hosted header exists.
#if defined(_MSC_VER) && !defined(__clang__)
#include <intrin.h>
#endif // defined(_MSC_VER) && !defined(__clang__)

namespace toy::test::detail {

/*!
  \brief Stops the process at a violated precondition.

  \param expression  Source text of the condition that failed.
  \param message     Reason the condition must hold.

  \note Called by the \c TOY_TEST_ASSERT macro, never directly.
  \note Both arguments are passed for a debugger to read at the frame and are otherwise unused: the runner has no output
        of its own, and the hosted headers that would give it one are unavailable on its targets.

  \warning Execution never continues past the call: the process stops on a trap instruction, or on a breakpoint a
           debugger cannot resume past.
*/
[[noreturn]] void assertionFailed(const char * expression, const char * message) noexcept;

} // namespace toy::test::detail

/*!
  \def TOY_TEST_ASSERT
  \brief Checks a precondition of the runner's own code.

  Evaluates \a condition and, when it does not hold, calls toy::test::detail::assertionFailed() with the condition's
  source text and \a message. Defining \c NDEBUG replaces the whole check with a no-op that evaluates neither argument.

  \param condition  Expression that must hold.
  \param message    Reason the condition must hold, in human-readable form.

  \note Guards the runner against its own callers. A test states an expectation with \c CHECK or \c REQUIRE instead.

  \warning A failed check stops the process without a report; the runner cannot print one.
*/
#ifdef NDEBUG
#define TOY_TEST_ASSERT(condition, message) ((void)0)
#else // NDEBUG
#define TOY_TEST_ASSERT(condition, message)                                                                            \
  ((condition) ? void(0) : ::toy::test::detail::assertionFailed(#condition, (message)))
#endif // NDEBUG

#include "assertion.inl"

#endif // INCLUDE_TESTS_RUNNER_ASSERTION_HPP_
