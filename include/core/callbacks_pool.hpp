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
  \file   callbacks_pool.hpp
  \brief  Fixed-capacity pool of function-pointer callbacks.

  Defines \ref toy::CallbacksPool for bounded subscriber lists without heap allocation.

  \note Included by core.hpp; do not include this file directly.
*/

#ifndef INCLUDE_CORE_CALLBACKS_POOL_HPP_
#define INCLUDE_CORE_CALLBACKS_POOL_HPP_

namespace toy {

/*!
  \class CallbacksPool
  \brief Fixed-capacity array of function pointers for observer-style notification.

  Stores up to \a AllocatedSize callbacks of type \c void (*)(T). No heap allocation: storage is inline in the object.
  \c add duplicates are ignored (same pointer not stored twice). \c remove swaps the last subscriber into the removed
  slot. \c call invokes subscribers in registration order.

  \tparam T             Callback argument type (passed by value to \c call()).
  \tparam AllocatedSize Maximum number of subscribers. Must be \c > \c 0 (default \c 4).

  \section features Key Features

  - **Bounded storage**: Capacity fixed at compile time; \c add returns \c false when full (assert in debug).
  - **constexpr**: Construction, \c add, \c remove, \c reset, \c subscribersAmount, and \c call are usable in constant
    evaluation when the callbacks allow it.
  - **noexcept**: All members are \c noexcept.
  - **Duplicate prevention**: Registering the same function pointer twice is a no-op; no duplicate entries.

  \section usage Usage Example

  \code
  #include "core.hpp"

  void onEvent(int value) { (void)value; }

  toy::CallbacksPool<int> pool;
  pool.add(onEvent);
  pool.call(42);
  pool.remove(onEvent);
  pool.reset();
  \endcode

  \section performance Performance Characteristics

  - **Construction / reset**: O(1).
  - **add**: O(\a n) in current subscriber count (linear scan for duplicate).
  - **remove**: O(\a n) worst case (linear search).
  - **call**: O(\a n) invocations for \a n subscribers.
  - **subscribersAmount**: O(1).

  \section safety Safety Guarantees

  - **Preconditions**: \c nullptr passed to \c add is asserted in debug; otherwise undefined behavior.
  - **Full pool**: \c add when full returns \c false; asserted in debug.
  - **Lifetime**: The pool stores raw function pointers only; it does not manage callee lifetime.

  \note Registering the same function pointer twice does not increase the subscriber count; the second \c add returns
        \c true without adding a duplicate.

  \warning Function pointers must remain valid for as long as they may be invoked through the pool.
*/
template <typename T, size_t AllocatedSize = 4>
class CallbacksPool {
  static_assert(AllocatedSize > 0, "CallbacksPool capacity must be greater than zero");

public:
  /*!
    \brief Adds a callback to the pool.

    This method registers a callback function in the pool. If the callback is already registered, the operation succeeds
    but does not create a duplicate entry. If the pool is full, the operation fails.

    \param method A pointer to the callback function to register.

    \return \c true if the callback was successfully added, \c false if the pool is full.

    \pre The \a method must not be \c nullptr.

    \note If the callback is already registered, the method returns \c true without adding a duplicate.
    \note If the pool is full, the method returns \c false and asserts in debug mode.
  */
  constexpr bool add(void (*method)(T arg)) noexcept;

  /*!
    \brief Removes a callback from the pool.

    This method unregisters a callback function from the pool. If the callback is not found, the operation has no
    effect.

    \param method A pointer to the callback function to remove.

    \return \c true if the callback was found and removed, \c false otherwise.

    \note Removing a non-existent callback is safe and returns \c false.
  */
  constexpr bool remove(void (*method)(T arg)) noexcept;

  /*!
    \brief Clears all callbacks from the pool.

    This method removes all registered callbacks, resetting the pool to an empty state.

    \post The pool is empty (no callbacks registered).
    \post All callback slots are available for new subscriptions.
  */
  constexpr void reset() noexcept;

  /*!
    \brief Returns the number of registered callbacks.

    This method returns the current number of active callbacks in the pool.

    \return The number of registered callbacks.

    \note Matches the number of slots filled by successful \c add operations (excluding duplicate adds).
  */
  [[nodiscard]] constexpr size_t subscribersAmount() const noexcept;

  /*!
    \brief Invokes all registered callbacks with the given parameter.

    This method calls all registered callback functions with the specified parameter value. Callbacks are invoked in the
    order they were added to the pool.

    \param arg The parameter value to pass to all registered callbacks.
  */
  constexpr void call(T arg) const noexcept;

private:
  //! Internal structure to store a callback function pointer.
  struct StaticCallback {
    //! Pointer to the callback function.
    void (*method)(T arg) = nullptr;
  };

  //! Fixed-size array storing registered callbacks.
  array<StaticCallback, AllocatedSize> _callbacks;

  //! Active callbacks count.
  size_t _subscribersCount{0};
};

} // namespace toy

#endif // INCLUDE_CORE_CALLBACKS_POOL_HPP_
