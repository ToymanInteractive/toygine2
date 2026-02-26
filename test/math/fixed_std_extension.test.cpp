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

#include <doctest/doctest.h>

#include "math.hpp"

namespace toy::math {

using Fixed = fixed<int32_t, int64_t, 8>;
using FixedNoRounding = fixed<int32_t, int64_t, 8, false>;
using Fixed4 = fixed<int32_t, int64_t, 4>;
using Fixed16 = fixed<int32_t, int64_t, 16>;

// numeric_limits specialization for fixed.
TEST_CASE("math/fixed/numeric_limits") {
  // Core traits match fixed-point semantics.
  SUBCASE("traits") {
    REQUIRE(numeric_limits<Fixed>::is_specialized);
    REQUIRE(numeric_limits<Fixed>::is_signed);
    REQUIRE(!numeric_limits<Fixed>::is_integer);
    REQUIRE(numeric_limits<Fixed>::is_exact);

    static_assert(numeric_limits<Fixed>::is_specialized, "numeric_limits must be specialized");
    static_assert(numeric_limits<Fixed>::is_signed, "Fixed is signed");
    static_assert(!numeric_limits<Fixed>::is_integer, "fixed-point is not integer");
    static_assert(numeric_limits<Fixed>::is_exact, "fixed-point is exact");
  }

  // No infinity or NaN.
  SUBCASE("no_special_values") {
    REQUIRE(!numeric_limits<Fixed>::has_infinity);
    REQUIRE(!numeric_limits<Fixed>::has_quiet_NaN);
    REQUIRE(!numeric_limits<Fixed>::has_signaling_NaN);

    static_assert(!numeric_limits<Fixed>::has_infinity, "fixed-point has no infinity");
    static_assert(!numeric_limits<Fixed>::has_quiet_NaN, "fixed-point has no NaN");
    static_assert(!numeric_limits<Fixed>::has_signaling_NaN, "fixed-point has no signaling NaN");
  }

  // Precision and radix.
  SUBCASE("precision") {
    REQUIRE(numeric_limits<Fixed>::radix == 2);
    REQUIRE(numeric_limits<Fixed>::digits == 31);
    REQUIRE(numeric_limits<Fixed>::digits10 == 9);
    REQUIRE(numeric_limits<Fixed>::max_digits10 == 10);

    static_assert(numeric_limits<Fixed>::radix == 2, "radix must be 2");
    static_assert(numeric_limits<Fixed>::digits == 31, "digits for int32_t 8 frac is 31");
    static_assert(numeric_limits<Fixed>::digits10 == 9,
                  "digits10 for fixed<int32_t,int64_t,8> must be floor(31 * log10(2)) = 9");
    static_assert(numeric_limits<Fixed>::max_digits10 == 10, "max_digits10 for fixed<int32_t,int64_t,8> must be 10");
  }

  // min, max, lowest.
  SUBCASE("min_max_lowest") {
    constexpr auto minVal = numeric_limits<Fixed>::min();
    constexpr auto maxVal = numeric_limits<Fixed>::max();
    constexpr auto lowestVal = numeric_limits<Fixed>::lowest();

    REQUIRE(minVal.rawValue() == 1);
    REQUIRE(maxVal.rawValue() == numeric_limits<int32_t>::max());
    REQUIRE(lowestVal.rawValue() == numeric_limits<int32_t>::min());

    static_assert(minVal.rawValue() == 1, "min must be smallest positive");
    static_assert(maxVal.rawValue() == numeric_limits<int32_t>::max(), "max must match Base::max()");
    static_assert(lowestVal.rawValue() == numeric_limits<int32_t>::min(), "lowest must match Base::min()");
  }

  // Epsilon equals 1 LSB; round_error is 0.5 when Rounding is true, 1.0 when false.
  SUBCASE("epsilon_round_error") {
    constexpr auto eps = numeric_limits<Fixed>::epsilon();
    constexpr auto roundErr = numeric_limits<Fixed>::round_error();

    REQUIRE(eps.rawValue() == 1);
    REQUIRE(roundErr.rawValue() == 128);

    static_assert(eps.rawValue() == 1, "epsilon is 1 LSB");
    static_assert(roundErr.rawValue() == 128, "round_error is 0.5 in raw units for Rounding true");
  }

  // round_error for FixedNoRounding returns 1.0 (raw 256).
  SUBCASE("round_error_no_rounding") {
    constexpr auto roundErr = numeric_limits<FixedNoRounding>::round_error();

    REQUIRE(roundErr.rawValue() == 256);

    static_assert(roundErr.rawValue() == 256, "round_error must be 1.0 when Rounding is false");
  }

  // round_style: round_to_nearest for Fixed, round_toward_zero for FixedNoRounding.
  SUBCASE("round_style") {
    REQUIRE(numeric_limits<Fixed>::round_style == std::round_to_nearest);
    REQUIRE(numeric_limits<FixedNoRounding>::round_style == std::round_toward_zero);

    static_assert(numeric_limits<Fixed>::round_style == std::round_to_nearest,
                  "round_style must be round_to_nearest when Rounding is true");
    static_assert(numeric_limits<FixedNoRounding>::round_style == std::round_toward_zero,
                  "round_style must be round_toward_zero when Rounding is false");
  }

  // Exponent range: min_exponent, max_exponent, min_exponent10, max_exponent10.
  SUBCASE("exponent_range") {
    REQUIRE(numeric_limits<Fixed>::min_exponent == 1 - 8);
    REQUIRE(numeric_limits<Fixed>::max_exponent == 31 - 8);
    REQUIRE(numeric_limits<Fixed>::min_exponent10 <= 0);
    REQUIRE(numeric_limits<Fixed>::max_exponent10 >= 0);

    static_assert(numeric_limits<Fixed>::min_exponent == 1 - 8, "min_exponent is 1 - Fraction");
    static_assert(numeric_limits<Fixed>::max_exponent == 31 - 8, "max_exponent is digits - Fraction");
    static_assert(numeric_limits<Fixed>::min_exponent10 <= 0,
                  "min_exponent10 must be non-positive for fractional resolution");
    static_assert(numeric_limits<Fixed>::max_exponent10 >= 0, "max_exponent10 must be non-negative for integer part");
  }

  // denorm_min returns min().
  SUBCASE("denorm_min") {
    constexpr auto d = numeric_limits<Fixed>::denorm_min();
    constexpr auto m = numeric_limits<Fixed>::min();

    REQUIRE(d.rawValue() == m.rawValue());

    static_assert(d.rawValue() == m.rawValue(), "denorm_min must equal min for fixed-point");
  }

  // infinity, quiet_NaN, signaling_NaN return zero.
  SUBCASE("stub_special_return_zero") {
    REQUIRE(numeric_limits<Fixed>::infinity().rawValue() == 0);
    REQUIRE(numeric_limits<Fixed>::quiet_NaN().rawValue() == 0);
    REQUIRE(numeric_limits<Fixed>::signaling_NaN().rawValue() == 0);

    static_assert(numeric_limits<Fixed>::infinity().rawValue() == 0, "infinity must return zero for fixed-point");
    static_assert(numeric_limits<Fixed>::quiet_NaN().rawValue() == 0, "quiet_NaN must return zero for fixed-point");
    static_assert(numeric_limits<Fixed>::signaling_NaN().rawValue() == 0,
                  "signaling_NaN must return zero for fixed-point");
  }
}

// std::numbers variable template specializations for fixed.
TEST_CASE("math/fixed_std_extension/numbers_constants") {
  // All constants are constexpr and positive where expected.
  SUBCASE("constexpr_and_sign") {
    constexpr auto e = std::numbers::e_v<Fixed>;
    constexpr auto pi = std::numbers::pi_v<Fixed>;
    constexpr auto sqrt2 = std::numbers::sqrt2_v<Fixed>;
    constexpr auto ln2 = std::numbers::ln2_v<Fixed>;
    constexpr auto inv_pi = std::numbers::inv_pi_v<Fixed>;

    REQUIRE(e.rawValue() > 0);
    REQUIRE(pi.rawValue() > 0);
    REQUIRE(sqrt2.rawValue() > 0);
    REQUIRE(ln2.rawValue() > 0);
    REQUIRE(inv_pi.rawValue() > 0);

    static_assert(std::numbers::e_v<Fixed>.rawValue() > 0, "e must be positive");
    static_assert(std::numbers::pi_v<Fixed>.rawValue() > 0, "pi must be positive");
    static_assert(std::numbers::sqrt2_v<Fixed>.rawValue() > 0, "sqrt2 must be positive");
  }

  // e and pi are close to std double constants when cast to double.
  SUBCASE("e_pi_near_std") {
    const double eDouble = static_cast<double>(std::numbers::e_v<Fixed>);
    const double piDouble = static_cast<double>(std::numbers::pi_v<Fixed>);

    REQUIRE(eDouble > 2.0);
    REQUIRE(eDouble < 3.5);
    REQUIRE(std::abs(eDouble - std::numbers::e_v<double>) < 0.1);

    REQUIRE(piDouble > 3.0);
    REQUIRE(piDouble < 3.5);
    REQUIRE(std::abs(piDouble - std::numbers::pi_v<double>) < 0.1);
  }

  // All 12 constants instantiate for Fixed and FixedNoRounding and are non-zero where expected.
  SUBCASE("all_constants_instantiate") {
    REQUIRE(std::numbers::e_v<Fixed>.rawValue() != 0);
    REQUIRE(std::numbers::pi_v<Fixed>.rawValue() != 0);
    REQUIRE(std::numbers::log2e_v<Fixed>.rawValue() != 0);
    REQUIRE(std::numbers::log10e_v<Fixed>.rawValue() != 0);
    REQUIRE(std::numbers::sqrt2_v<Fixed>.rawValue() != 0);
    REQUIRE(std::numbers::sqrt3_v<Fixed>.rawValue() != 0);
    REQUIRE(std::numbers::inv_pi_v<Fixed>.rawValue() != 0);
    REQUIRE(std::numbers::inv_sqrtpi_v<Fixed>.rawValue() != 0);
    REQUIRE(std::numbers::ln2_v<Fixed>.rawValue() != 0);
    REQUIRE(std::numbers::ln10_v<Fixed>.rawValue() != 0);
    REQUIRE(std::numbers::egamma_v<Fixed>.rawValue() != 0);
    REQUIRE(std::numbers::phi_v<Fixed>.rawValue() != 0);

    REQUIRE(std::numbers::e_v<FixedNoRounding>.rawValue() != 0);
    REQUIRE(std::numbers::pi_v<FixedNoRounding>.rawValue() != 0);

    static_assert(std::numbers::e_v<Fixed>.rawValue() != 0, "e must be non-zero");
    static_assert(std::numbers::pi_v<Fixed>.rawValue() != 0, "pi must be non-zero");
  }
}

} // namespace toy::math
