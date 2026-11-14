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
  \file   chrono_steady_clock.test.cpp
  \brief  Unit tests for \ref toy::chrono::SteadyClock.
*/

#include "core.hpp"
#include "toy_test.hpp"

namespace toy::chrono {

TEST_CASE("chrono/steady_clock/types") {
  static_assert(SteadyClock::is_steady, "the steady clock must never step back");
  static_assert(std::is_same_v<SteadyClock::rep, int64_t>, "the tick counter must not overflow in a session");
  static_assert(SteadyClock::period::den == steadyClockPeriodDenominator,
                "the tick period follows the platform denominator");
}

} // namespace toy::chrono
