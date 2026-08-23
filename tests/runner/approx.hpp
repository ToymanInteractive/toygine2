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
  \file   approx.hpp
  \brief  Tolerant floating-point comparison shared by the doctest and built-in test runners.

  Defines \ref toy::test::Approx: a value carrying a relative tolerance, compared through a free \c operator==. Used
  wherever a test asserts a floating-point result, under either runner.

  \note Included by toy_test.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_TESTS_RUNNER_APPROX_HPP_
#define INCLUDE_TESTS_RUNNER_APPROX_HPP_

#include <concepts>
#include <limits>
#include <type_traits>

namespace toy::test {

/*!
  \class Approx
  \brief Floating-point value carrying a comparison tolerance.

  Compares equal to a floating-point value whose difference falls within a tolerance that scales with magnitude, so the
  same epsilon works for small and large values.

  \tparam T  Floating-point type of the compared value; satisfies \c std::floating_point, so \c float, \c double and
             <tt>long double</tt> are accepted.

  \section features Key Features

  * **Constexpr support**: every operation is usable in a constant expression
  * **Relative tolerance**: the epsilon scales with the larger operand
  * **Mixed precision**: comparison against any floating-point type happens in their common type
  * **Freestanding**: no dependency on \c <cmath> or any hosted header

  \section usage Usage Example

  \code
  #include "toy_test.hpp"

  REQUIRE(computeRatio() == toy::test::Approx(0.5));
  REQUIRE(measureShort() == toy::test::Approx(1.0F).epsilon(0.01F));

  toy::test::Approx<long double> wide{1.0L};
  constexpr auto tolerant = toy::test::Approx<double>(1.0).epsilon(0.5);
  \endcode

  \section performance Performance Characteristics

  * **Construction**: O(1) constant time
  * **Comparison**: O(1) constant time
  * **Memory usage**: two objects of type \a T

  \section safety Safety Guarantees

  * **Contracts**: none; every finite value of type \a T is a valid argument
  * **Type safety**: uses C++23 concepts; a non-floating-point argument fails the constraint
  * **Memory safety**: no dynamic allocation
  * **Exception safety**: No operation throws; exceptions are off in the build

  \section compatibility Compatibility

  Requires C++20 concepts. Allocates nothing and includes no hosted header, so it builds on every console and embedded
  target the engine supports. Verdicts match DocTest on every value \c double represents; comparison happens in the
  operands' own precision rather than in \c double, which DocTest always uses.

  \note A comparison involving NaN is false, matching the behaviour of the built-in operator.

  \note The default tolerance is the same constant for every \a T, deliberately: DocTest uses that one value regardless
        of type, and matching it keeps a test's verdict identical under both runners.
*/
template <std::floating_point T>
class Approx final {
public:
  explicit constexpr Approx(T value) noexcept;

  /*!
    \brief Returns a copy carrying the given tolerance.

    \tparam U     Floating-point type of the tolerance; converted to \a T.
    \param  value Relative tolerance; scaled by the larger operand during comparison.

    \return Copy of this value with \a value as its tolerance.

    \sa tolerance()
  */
  template <std::floating_point U>
  [[nodiscard]] constexpr Approx epsilon(U value) const noexcept;

  /// Returns the compared value.
  [[nodiscard]] constexpr T value() const noexcept;

  /// Returns the relative tolerance; see epsilon().
  [[nodiscard]] constexpr T tolerance() const noexcept;

private:
  T _value;
  T _epsilon;
};

/*!
  \brief Compares a tolerant value against a plain one.

  \tparam T    Floating-point type of the tolerant value.
  \tparam U    Floating-point type of the plain value.
  \param  lhs  Tolerant value.
  \param  rhs  Plain value.

  \return \c true when the difference is strictly below <tt>tolerance * (1 + max(|lhs|, |rhs|))</tt>.

  \note The formula and its strict inequality reproduce DocTest 2.5.3. DocTest's adjustable scale factor is fixed at one
        here; no test configures it.

  \note Both operands are widened to their common type before comparison, so mixing precisions loses nothing. DocTest
        instead narrows everything to \c double. The two agree on every value \c double represents, because the gap
        between the precisions stays orders of magnitude below the tolerance; they can disagree on a
        <tt>long double</tt> beyond the range of \c double, where narrowing yields infinity.

  \note The form with the plain value on the left resolves through the reversed candidate C++20 synthesizes, so no
        second operator is declared.

  \sa toy::test::Approx
*/
template <std::floating_point T, std::floating_point U>
[[nodiscard]] constexpr bool operator==(const Approx<T> & lhs, U rhs) noexcept;

} // namespace toy::test

#include "approx.inl"

#endif // INCLUDE_TESTS_RUNNER_APPROX_HPP_
