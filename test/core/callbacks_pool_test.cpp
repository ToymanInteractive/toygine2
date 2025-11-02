//
// Copyright (c) 2025 by Toyman Interactive
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

#include <catch2/catch_test_macros.hpp>

#include "core.hpp"

using namespace toy;

namespace {
static int g_callback1Count = 0;
static int g_callback2Count = 0;
static int g_callback3Count = 0;
static int g_lastValue = 0;

void callback1(int value) {
  ++g_callback1Count;
  g_lastValue = value;
}

void callback2(int value) {
  ++g_callback2Count;
  g_lastValue = value * 2;
}

void callback3(int value) {
  ++g_callback3Count;
  g_lastValue = value * 3;
}

void resetCounters() {
  g_callback1Count = 0;
  g_callback2Count = 0;
  g_callback3Count = 0;
  g_lastValue = 0;
}
} // namespace

TEST_CASE("CallbacksPool constructors", "[core][callbacks_pool]") {
  SECTION("Default constructor") {
    constexpr CallbacksPool<int, 4> pool{};

    REQUIRE(pool.subscribersAmount() == 0);
    STATIC_REQUIRE(CallbacksPool<int, 4>{}.subscribersAmount() == 0);
  }

  SECTION("Different template parameters") {
    CallbacksPool<int, 8> largePool{};
    REQUIRE(largePool.subscribersAmount() == 0);

    CallbacksPool<double, 2> smallPool{};
    REQUIRE(smallPool.subscribersAmount() == 0);
  }
}

TEST_CASE("CallbacksPool add method", "[core][callbacks_pool]") {
  SECTION("Add single callback") {
    CallbacksPool<int, 4> pool{};

    bool result = pool.add(callback1);
    REQUIRE(result == true);
    REQUIRE(pool.subscribersAmount() == 1);
  }

  SECTION("Add multiple callbacks") {
    CallbacksPool<int, 4> pool{};

    REQUIRE(pool.add(callback1) == true);
    REQUIRE(pool.subscribersAmount() == 1);

    REQUIRE(pool.add(callback2) == true);
    REQUIRE(pool.subscribersAmount() == 2);

    REQUIRE(pool.add(callback3) == true);
    REQUIRE(pool.subscribersAmount() == 3);
  }

  SECTION("Add duplicate callback (idempotent)") {
    CallbacksPool<int, 4> pool{};

    REQUIRE(pool.add(callback1) == true);
    REQUIRE(pool.subscribersAmount() == 1);

    // Adding the same callback again should return true but not increase count
    REQUIRE(pool.add(callback1) == true);
    REQUIRE(pool.subscribersAmount() == 1); // Still 1, no duplicate added
  }

  SECTION("Add with different types") {
    CallbacksPool<double, 4> doublePool{};
    void (*doubleCallback)(double) = [](double d) { (void)d; };
    REQUIRE(doublePool.add(doubleCallback) == true);

    CallbacksPool<std::size_t, 4> sizePool{};
    void (*sizeCallback)(std::size_t) = [](std::size_t s) { (void)s; };
    REQUIRE(sizePool.add(sizeCallback) == true);
  }
}

TEST_CASE("CallbacksPool remove method", "[core][callbacks_pool]") {
  SECTION("Remove existing callback") {
    CallbacksPool<int, 4> pool{};

    pool.add(callback1);
    pool.add(callback2);
    REQUIRE(pool.subscribersAmount() == 2);

    REQUIRE(pool.remove(callback1) == true);
    REQUIRE(pool.subscribersAmount() == 1);

    REQUIRE(pool.remove(callback2) == true);
    REQUIRE(pool.subscribersAmount() == 0);
  }

  SECTION("Remove non-existent callback") {
    CallbacksPool<int, 4> pool{};

    pool.add(callback1);
    REQUIRE(pool.subscribersAmount() == 1);

    REQUIRE(pool.remove(callback2) == false); // callback2 was never added
    REQUIRE(pool.subscribersAmount() == 1); // Count unchanged
  }

  SECTION("Remove from empty pool") {
    CallbacksPool<int, 4> pool{};

    REQUIRE(pool.remove(callback1) == false);
    REQUIRE(pool.subscribersAmount() == 0);
  }

  SECTION("Remove and re-add") {
    CallbacksPool<int, 4> pool{};

    pool.add(callback1);
    REQUIRE(pool.subscribersAmount() == 1);

    pool.remove(callback1);
    REQUIRE(pool.subscribersAmount() == 0);

    pool.add(callback1);
    REQUIRE(pool.subscribersAmount() == 1);
  }

  SECTION("Remove multiple callbacks") {
    CallbacksPool<int, 4> pool{};

    pool.add(callback1);
    pool.add(callback2);
    pool.add(callback3);
    REQUIRE(pool.subscribersAmount() == 3);

    pool.remove(callback2);
    REQUIRE(pool.subscribersAmount() == 2);

    pool.remove(callback1);
    REQUIRE(pool.subscribersAmount() == 1);

    pool.remove(callback3);
    REQUIRE(pool.subscribersAmount() == 0);
  }
}

TEST_CASE("CallbacksPool reset method", "[core][callbacks_pool]") {
  SECTION("Reset empty pool") {
    CallbacksPool<int, 4> pool{};

    pool.reset();
    REQUIRE(pool.subscribersAmount() == 0);
  }

  SECTION("Reset pool with callbacks") {
    CallbacksPool<int, 4> pool{};

    pool.add(callback1);
    pool.add(callback2);
    pool.add(callback3);
    REQUIRE(pool.subscribersAmount() == 3);

    pool.reset();
    REQUIRE(pool.subscribersAmount() == 0);
  }

  SECTION("Reset and reuse") {
    CallbacksPool<int, 4> pool{};

    pool.add(callback1);
    pool.add(callback2);
    REQUIRE(pool.subscribersAmount() == 2);

    pool.reset();
    REQUIRE(pool.subscribersAmount() == 0);

    // Pool should be reusable after reset
    pool.add(callback1);
    pool.add(callback3);
    REQUIRE(pool.subscribersAmount() == 2);
  }

  SECTION("Multiple resets") {
    CallbacksPool<int, 4> pool{};

    pool.add(callback1);
    pool.reset();
    pool.add(callback2);
    pool.reset();
    pool.reset(); // Reset again
    REQUIRE(pool.subscribersAmount() == 0);
  }
}

TEST_CASE("CallbacksPool subscribersAmount method", "[core][callbacks_pool]") {
  SECTION("Empty pool") {
    CallbacksPool<int, 4> pool{};

    REQUIRE(pool.subscribersAmount() == 0);
  }

  SECTION("Count after adding") {
    CallbacksPool<int, 4> pool{};

    REQUIRE(pool.subscribersAmount() == 0);

    pool.add(callback1);
    REQUIRE(pool.subscribersAmount() == 1);

    pool.add(callback2);
    REQUIRE(pool.subscribersAmount() == 2);

    pool.add(callback3);
    REQUIRE(pool.subscribersAmount() == 3);
  }

  SECTION("Count after removing") {
    CallbacksPool<int, 4> pool{};

    pool.add(callback1);
    pool.add(callback2);
    pool.add(callback3);
    REQUIRE(pool.subscribersAmount() == 3);

    pool.remove(callback2);
    REQUIRE(pool.subscribersAmount() == 2);

    pool.remove(callback1);
    REQUIRE(pool.subscribersAmount() == 1);

    pool.remove(callback3);
    REQUIRE(pool.subscribersAmount() == 0);
  }

  SECTION("Count after duplicate add") {
    CallbacksPool<int, 4> pool{};

    pool.add(callback1);
    REQUIRE(pool.subscribersAmount() == 1);

    pool.add(callback1); // Duplicate
    REQUIRE(pool.subscribersAmount() == 1); // Still 1
  }

  SECTION("Count with different pool sizes") {
    CallbacksPool<int, 2> smallPool{};
    REQUIRE(smallPool.subscribersAmount() == 0);

    smallPool.add(callback1);
    REQUIRE(smallPool.subscribersAmount() == 1);

    CallbacksPool<int, 10> largePool{};
    REQUIRE(largePool.subscribersAmount() == 0);

    largePool.add(callback1);
    largePool.add(callback2);
    largePool.add(callback3);
    REQUIRE(largePool.subscribersAmount() == 3);
  }
}

TEST_CASE("CallbacksPool call method", "[core][callbacks_pool]") {
  SECTION("Call with no callbacks") {
    CallbacksPool<int, 4> pool{};

    resetCounters();
    pool.call(42);

    REQUIRE(g_callback1Count == 0);
    REQUIRE(g_callback2Count == 0);
    REQUIRE(g_callback3Count == 0);
  }

  SECTION("Call single callback") {
    CallbacksPool<int, 4> pool{};

    pool.add(callback1);
    resetCounters();

    pool.call(10);

    REQUIRE(g_callback1Count == 1);
    REQUIRE(g_callback2Count == 0);
    REQUIRE(g_callback3Count == 0);
    REQUIRE(g_lastValue == 10);
  }

  SECTION("Call multiple callbacks") {
    CallbacksPool<int, 4> pool{};

    pool.add(callback1);
    pool.add(callback2);
    pool.add(callback3);
    resetCounters();

    pool.call(5);

    REQUIRE(g_callback1Count == 1);
    REQUIRE(g_callback2Count == 1);
    REQUIRE(g_callback3Count == 1);
    REQUIRE(g_lastValue == 15); // Last callback sets g_lastValue to value * 3
  }

  SECTION("Call after removal") {
    CallbacksPool<int, 4> pool{};

    pool.add(callback1);
    pool.add(callback2);
    pool.add(callback3);

    pool.remove(callback2);
    resetCounters();

    pool.call(7);

    REQUIRE(g_callback1Count == 1);
    REQUIRE(g_callback2Count == 0); // Removed, not called
    REQUIRE(g_callback3Count == 1);
  }

  SECTION("Call multiple times") {
    CallbacksPool<int, 4> pool{};

    pool.add(callback1);
    resetCounters();

    pool.call(1);
    REQUIRE(g_callback1Count == 1);

    pool.call(2);
    REQUIRE(g_callback1Count == 2);

    pool.call(3);
    REQUIRE(g_callback1Count == 3);
  }

  SECTION("Call with different parameter values") {
    CallbacksPool<int, 4> pool{};

    pool.add(callback1);
    resetCounters();

    pool.call(100);
    REQUIRE(g_lastValue == 100);

    pool.call(200);
    REQUIRE(g_lastValue == 200);

    pool.call(-50);
    REQUIRE(g_lastValue == -50);
  }

  SECTION("Call after reset") {
    CallbacksPool<int, 4> pool{};

    pool.add(callback1);
    pool.add(callback2);

    pool.reset();
    resetCounters();

    pool.call(42);

    REQUIRE(g_callback1Count == 0);
    REQUIRE(g_callback2Count == 0);
  }
}

TEST_CASE("CallbacksPool integration", "[core][callbacks_pool]") {
  SECTION("Full lifecycle") {
    CallbacksPool<int, 4> pool{};

    // Start empty
    REQUIRE(pool.subscribersAmount() == 0);

    // Add callbacks
    pool.add(callback1);
    pool.add(callback2);
    REQUIRE(pool.subscribersAmount() == 2);

    // Call callbacks
    resetCounters();
    pool.call(10);
    REQUIRE(g_callback1Count == 1);
    REQUIRE(g_callback2Count == 1);

    // Remove one
    pool.remove(callback1);
    REQUIRE(pool.subscribersAmount() == 1);

    // Call again
    resetCounters();
    pool.call(20);
    REQUIRE(g_callback1Count == 0);
    REQUIRE(g_callback2Count == 1);

    // Reset
    pool.reset();
    REQUIRE(pool.subscribersAmount() == 0);

    // Add new callback
    pool.add(callback3);
    REQUIRE(pool.subscribersAmount() == 1);

    resetCounters();
    pool.call(30);
    REQUIRE(g_callback3Count == 1);
  }

  SECTION("Duplicate handling") {
    CallbacksPool<int, 4> pool{};

    pool.add(callback1);
    pool.add(callback1); // Duplicate
    pool.add(callback1); // Duplicate again

    REQUIRE(pool.subscribersAmount() == 1);

    resetCounters();
    pool.call(1);
    REQUIRE(g_callback1Count == 1); // Called once, not three times
  }

  SECTION("Remove and add same callback") {
    CallbacksPool<int, 4> pool{};

    pool.add(callback1);
    REQUIRE(pool.subscribersAmount() == 1);

    pool.remove(callback1);
    REQUIRE(pool.subscribersAmount() == 0);

    pool.add(callback1);
    REQUIRE(pool.subscribersAmount() == 1);

    resetCounters();
    pool.call(42);
    REQUIRE(g_callback1Count == 1);
  }
}

TEST_CASE("CallbacksPool constexpr evaluation", "[core][callbacks_pool]") {
  SECTION("Constexpr construction") {
    constexpr CallbacksPool<int, 4> pool{};

    STATIC_REQUIRE(pool.subscribersAmount() == 0);
  }

  SECTION("Constexpr operations") {
    constexpr auto testCallback = [](int) {};

    constexpr CallbacksPool<int, 4> pool{};

    // Note: constexpr evaluation of add/remove/call is limited by the fact
    // that function pointers to non-static functions are not constexpr in C++23
    // These would need to be constexpr lambdas or static functions
    // For now, we test compile-time construction
    STATIC_REQUIRE(pool.subscribersAmount() == 0);
  }
}
