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

#include "core.hpp"

namespace toy {

static int callback1Count = 0;
static int callback2Count = 0;
static int callback3Count = 0;

static int lastValue = 0;

static void callback1(int value) {
  ++callback1Count;
  lastValue = value;
}

static void callback2(int value) {
  ++callback2Count;
  lastValue = value * 2;
}

static void callback3(int value) {
  ++callback3Count;
  lastValue = value * 3;
}

static void resetCounters() {
  callback1Count = 0;
  callback2Count = 0;
  callback3Count = 0;
  lastValue = 0;
}

// Default and template-parameterized constructors.
TEST_CASE("core/callbacks_pool/constructors") {
  // Default constructor yields zero subscribers.
  SUBCASE("default_constructor") {
    constexpr CallbacksPool<int, 4> pool;

    REQUIRE(pool.subscribersAmount() == 0);

    static_assert(pool.subscribersAmount() == 0, "default-constructed pool must have zero subscribers");
  }

  // Different template parameters yield zero subscribers.
  SUBCASE("different_template_parameters") {
    constexpr CallbacksPool<double, 2> smallPool;
    constexpr CallbacksPool<int, 8> largePool;

    REQUIRE(smallPool.subscribersAmount() == 0);
    REQUIRE(largePool.subscribersAmount() == 0);

    static_assert(smallPool.subscribersAmount() == 0, "default-constructed pool must have zero subscribers");
    static_assert(largePool.subscribersAmount() == 0, "default-constructed pool must have zero subscribers");
  }
}

// add() subscribes callbacks; duplicate add is idempotent.
TEST_CASE("core/callbacks_pool/add_method") {
  // Add single callback.
  SUBCASE("add_single_callback") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.subscribersAmount() == 1);
  }

  // Add multiple callbacks.
  SUBCASE("add_multiple_callbacks") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.subscribersAmount() == 1);

    REQUIRE(pool.add(callback2));
    REQUIRE(pool.subscribersAmount() == 2);

    REQUIRE(pool.add(callback3));
    REQUIRE(pool.subscribersAmount() == 3);
  }

  // Duplicate add is idempotent; count unchanged.
  SUBCASE("add_duplicate_callback_idempotent") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.subscribersAmount() == 1);

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.subscribersAmount() == 1);
  }

  // Add with different parameter types.
  SUBCASE("add_with_different_types") {
    CallbacksPool<double, 4> doublePool;
    CallbacksPool<size_t, 4> sizePool;

    void (*doubleCallback)(double) = [](double d) { (void)d; };
    REQUIRE(doublePool.add(doubleCallback));

    void (*sizeCallback)(size_t) = [](size_t s) { (void)s; };
    REQUIRE(sizePool.add(sizeCallback));
  }
}

// remove() unsubscribes callbacks; remove from empty or non-existent returns false.
TEST_CASE("core/callbacks_pool/remove_method") {
  // Remove existing callback.
  SUBCASE("remove_existing_callback") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.add(callback2));
    REQUIRE(pool.subscribersAmount() == 2);

    REQUIRE(pool.remove(callback1));
    REQUIRE(pool.subscribersAmount() == 1);

    REQUIRE(pool.remove(callback2));
    REQUIRE(pool.subscribersAmount() == 0);
  }

  // Remove nonexistent callback leaves pool unchanged.
  SUBCASE("remove_nonexistent_callback") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.subscribersAmount() == 1);

    REQUIRE(!pool.remove(callback2));
    REQUIRE(pool.subscribersAmount() == 1);
  }

  // Remove from empty pool returns false.
  SUBCASE("remove_from_empty_pool") {
    CallbacksPool<int, 4> pool;

    REQUIRE(!pool.remove(callback1));
    REQUIRE(pool.subscribersAmount() == 0);
  }

  // Remove and readd same callback.
  SUBCASE("remove_and_readd") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.subscribersAmount() == 1);

    REQUIRE(pool.remove(callback1));
    REQUIRE(pool.subscribersAmount() == 0);

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.subscribersAmount() == 1);
  }

  // Remove multiple callbacks.
  SUBCASE("remove_multiple_callbacks") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.add(callback2));
    REQUIRE(pool.add(callback3));
    REQUIRE(pool.subscribersAmount() == 3);

    REQUIRE(pool.remove(callback2));
    REQUIRE(pool.subscribersAmount() == 2);

    REQUIRE(pool.remove(callback1));
    REQUIRE(pool.subscribersAmount() == 1);

    REQUIRE(pool.remove(callback3));
    REQUIRE(pool.subscribersAmount() == 0);
  }
}

// reset() clears all subscribers; pool is reusable after reset.
TEST_CASE("core/callbacks_pool/reset_method") {
  // Reset empty pool.
  SUBCASE("reset_empty_pool") {
    CallbacksPool<int, 4> pool;

    pool.reset();

    REQUIRE(pool.subscribersAmount() == 0);
  }

  // Reset pool with callbacks clears all.
  SUBCASE("reset_pool_with_callbacks") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.add(callback2));
    REQUIRE(pool.add(callback3));
    REQUIRE(pool.subscribersAmount() == 3);

    pool.reset();

    REQUIRE(pool.subscribersAmount() == 0);
  }

  // Reset and reuse pool.
  SUBCASE("reset_and_reuse") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.add(callback2));
    REQUIRE(pool.subscribersAmount() == 2);

    pool.reset();

    REQUIRE(pool.subscribersAmount() == 0);

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.add(callback3));
    REQUIRE(pool.subscribersAmount() == 2);
  }

  // Multiple resets.
  SUBCASE("multiple_resets") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));

    pool.reset();

    REQUIRE(pool.add(callback2));

    pool.reset();
    pool.reset();

    REQUIRE(pool.subscribersAmount() == 0);
  }
}

// subscribersAmount() returns current subscriber count.
TEST_CASE("core/callbacks_pool/subscribers_amount_method") {
  // Empty pool has zero count.
  SUBCASE("empty_pool") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.subscribersAmount() == 0);
  }

  // Count after adding callbacks.
  SUBCASE("count_after_adding") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.subscribersAmount() == 0);
    REQUIRE(pool.add(callback1));
    REQUIRE(pool.subscribersAmount() == 1);
    REQUIRE(pool.add(callback2));
    REQUIRE(pool.subscribersAmount() == 2);
    REQUIRE(pool.add(callback3));
    REQUIRE(pool.subscribersAmount() == 3);
  }

  // Count after removing callbacks.
  SUBCASE("count_after_removing") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.add(callback2));
    REQUIRE(pool.add(callback3));
    REQUIRE(pool.subscribersAmount() == 3);

    REQUIRE(pool.remove(callback2));
    REQUIRE(pool.subscribersAmount() == 2);

    REQUIRE(pool.remove(callback1));
    REQUIRE(pool.subscribersAmount() == 1);

    REQUIRE(pool.remove(callback3));
    REQUIRE(pool.subscribersAmount() == 0);
  }

  // Count after duplicate add unchanged.
  SUBCASE("count_after_duplicate_add") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.subscribersAmount() == 1);

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.subscribersAmount() == 1);
  }

  // Count with different pool sizes.
  SUBCASE("count_with_different_pool_sizes") {
    CallbacksPool<int, 2> smallPool;

    REQUIRE(smallPool.subscribersAmount() == 0);
    REQUIRE(smallPool.add(callback1));
    REQUIRE(smallPool.subscribersAmount() == 1);

    CallbacksPool<int, 10> largePool;
    REQUIRE(largePool.subscribersAmount() == 0);

    REQUIRE(largePool.add(callback1));
    REQUIRE(largePool.add(callback2));
    REQUIRE(largePool.add(callback3));
    REQUIRE(largePool.subscribersAmount() == 3);
  }
}

// call() invokes all subscribed callbacks with the given argument.
TEST_CASE("core/callbacks_pool/call_method") {
  // Call with no callbacks; counters unchanged.
  SUBCASE("call_with_no_callbacks") {
    CallbacksPool<int, 4> pool;

    resetCounters();
    pool.call(42);

    REQUIRE(callback1Count == 0);
    REQUIRE(callback2Count == 0);
    REQUIRE(callback3Count == 0);
  }

  // Call single callback.
  SUBCASE("call_single_callback") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));

    resetCounters();

    pool.call(10);

    REQUIRE(callback1Count == 1);
    REQUIRE(callback2Count == 0);
    REQUIRE(callback3Count == 0);
    REQUIRE(lastValue == 10);
  }

  // Call multiple callbacks.
  SUBCASE("call_multiple_callbacks") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.add(callback2));
    REQUIRE(pool.add(callback3));

    resetCounters();

    pool.call(5);

    REQUIRE(callback1Count == 1);
    REQUIRE(callback2Count == 1);
    REQUIRE(callback3Count == 1);
    REQUIRE(lastValue == 15);
  }

  // Call after removal invokes remaining only.
  SUBCASE("call_after_removal") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.add(callback2));
    REQUIRE(pool.add(callback3));

    pool.remove(callback2);

    resetCounters();

    pool.call(7);

    REQUIRE(callback1Count == 1);
    REQUIRE(callback2Count == 0);
    REQUIRE(callback3Count == 1);
  }

  // Call multiple times; callback invoked each time.
  SUBCASE("call_multiple_times") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));

    resetCounters();

    pool.call(1);

    REQUIRE(callback1Count == 1);

    pool.call(2);

    REQUIRE(callback1Count == 2);

    pool.call(3);

    REQUIRE(callback1Count == 3);
  }

  // Call with different parameter values.
  SUBCASE("call_with_different_parameter_values") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));

    resetCounters();

    pool.call(100);

    REQUIRE(lastValue == 100);

    pool.call(200);

    REQUIRE(lastValue == 200);

    pool.call(-50);

    REQUIRE(lastValue == -50);
  }

  // Call after reset invokes none.
  SUBCASE("call_after_reset") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.add(callback2));

    pool.reset();

    resetCounters();

    pool.call(42);

    REQUIRE(callback1Count == 0);
    REQUIRE(callback2Count == 0);
  }
}

// Add, remove, reset, call in sequence; duplicate and re-add behavior.
TEST_CASE("core/callbacks_pool/integration") {
  // Full lifecycle: add, call, remove, reset, add again.
  SUBCASE("full_lifecycle") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.subscribersAmount() == 0);

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.add(callback2));

    REQUIRE(pool.subscribersAmount() == 2);

    resetCounters();

    pool.call(10);

    REQUIRE(callback1Count == 1);
    REQUIRE(callback2Count == 1);

    pool.remove(callback1);

    REQUIRE(pool.subscribersAmount() == 1);

    resetCounters();

    pool.call(20);

    REQUIRE(callback1Count == 0);
    REQUIRE(callback2Count == 1);

    pool.reset();

    REQUIRE(pool.subscribersAmount() == 0);

    pool.add(callback3);

    REQUIRE(pool.subscribersAmount() == 1);

    resetCounters();

    pool.call(30);

    REQUIRE(callback3Count == 1);
  }

  // Duplicate add; single subscription, single invocation.
  SUBCASE("duplicate_handling") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));
    REQUIRE(pool.add(callback1));
    REQUIRE(pool.add(callback1));

    REQUIRE(pool.subscribersAmount() == 1);

    resetCounters();

    pool.call(1);

    REQUIRE(callback1Count == 1);
  }

  // Remove and add same callback; count and invocation correct.
  SUBCASE("remove_and_add_same_callback") {
    CallbacksPool<int, 4> pool;

    REQUIRE(pool.add(callback1));

    REQUIRE(pool.subscribersAmount() == 1);

    REQUIRE(pool.remove(callback1));

    REQUIRE(pool.subscribersAmount() == 0);

    REQUIRE(pool.add(callback1));

    REQUIRE(pool.subscribersAmount() == 1);

    resetCounters();

    pool.call(42);

    REQUIRE(callback1Count == 1);
  }
}

} // namespace toy
