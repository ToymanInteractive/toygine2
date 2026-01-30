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
  \brief  Template callback pool class with fixed-size callback storage.
*/

#ifndef INCLUDE_CORE_CALLBACKS_POOL_HPP_
#define INCLUDE_CORE_CALLBACKS_POOL_HPP_

namespace toy {

/*!
  \class CallbacksPool
  \brief Template callback pool class with fixed-size callback storage.

  CallbacksPool is a lightweight, high-performance callback pool class that uses a fixed-size array to store function
  pointers. It provides a simple observer pattern implementation without dynamic memory allocation, making it suitable
  for embedded systems, real-time applications, and performance-critical code where memory allocation overhead must be
  minimized.

  \tparam type          The type of the callback parameter. Must be a copyable type.
  \tparam allocatedSize The maximum number of callbacks that can be stored in the pool. Must be greater than zero
                        (default: \c 4).

  \section features Key Features

  - ⚙️ **Zero Dynamic Allocation**: All memory is allocated on the stack at compile time
  - 🔧 **ConstExpr Support**: All operations can be evaluated at compile time
  - 🛡️ **Exception Safety**: All operations are noexcept
  - 🔗 **Simple Interface**: Easy-to-use callback subscription and invocation
  - 🧬 **Type Safety**: Strong typing with template parameters

  \section usage Usage Example

  \code
  #include "core/callbacks_pool.hpp"

  // Define callback function
  void onEvent(int value) {
    // Handle event
  }

  // Create a callback pool with default size (4 callbacks)
  toy::CallbacksPool<int> pool;

  // Subscribe to events
  pool.add(onEvent);

  // Invoke all registered callbacks
  pool.call(42);

  // Check subscriber count
  auto count = pool.subscribersAmount();

  // Unsubscribe
  pool.remove(onEvent);

  // Clear all subscriptions
  pool.reset();
  \endcode

  \section performance Performance Characteristics

  - ⚙️ **Construction**: O(1) constant time
  - 📝 **Adding Callback**: O(n) where n is the number of registered callbacks (for duplicate check)
  - 🔗 **Removing Callback**: O(n) where n is the number of registered callbacks
  - 🔍 **Calling Callbacks**: O(n) where n is the number of registered callbacks
  - 📊 **Subscriber Count**: O(1) constant time (cached count)
  - 💾 **Memory Usage**: Fixed at compile time
  - ⚡ **Cache Performance**: Excellent due to stack allocation and contiguous memory layout
  - 📋 **Copy Performance**: Fast due to array-based storage

  \section safety Safety Guarantees

  - 🛡️ **Contracts & Debug Checks**: Null pointer checks are asserted in debug; in release, violating preconditions is UB
  - 🔒 **Bounds Safety**: Pool capacity is fixed at compile time; adding beyond capacity is asserted in debug
  - 📐 **Type Safety**: Template parameters ensure type-safe callback invocation
  - ⚠️ **Exception Safety**: All operations are noexcept, no exceptions thrown

  \note The pool does not prevent duplicate subscriptions - adding the same callback multiple times will only register
        it once and return \c true.
  \note When the pool is full, attempts to add new callbacks will fail (asserted in debug mode).

  \warning Callbacks must remain valid for the lifetime of the CallbacksPool object. The pool does not manage callback
           lifetime.
*/
template <typename type, size_t allocatedSize = 4>
class CallbacksPool {
public:
  /*!
    \brief Default constructor.

    Creates an empty callback pool with all slots available.

    \post The pool is empty (no callbacks registered).
  */
  constexpr CallbacksPool() noexcept;

  /*!
    \brief Destructor for the callback pool.

    \note Since the CallbacksPool does not manage callback lifetime, no special cleanup is required.
  */
  constexpr ~CallbacksPool() noexcept = default;

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
  constexpr bool add(void (*method)(type arg)) noexcept;

  /*!
    \brief Removes a callback from the pool.

    This method unregisters a callback function from the pool. If the callback is not found, the operation has no
    effect.

    \param method A pointer to the callback function to remove.

    \return \c true if the callback was found and removed, \c false otherwise.

    \note Removing a non-existent callback is safe and returns \c false.
  */
  constexpr bool remove(void (*method)(type arg)) noexcept;

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

    \note The count is maintained internally for optimal performance.
  */
  [[nodiscard]] constexpr size_t subscribersAmount() const noexcept;

  /*!
    \brief Invokes all registered callbacks with the given parameter.

    This method calls all registered callback functions with the specified parameter value. Callbacks are invoked in the
    order they were added to the pool.

    \param parameter The parameter value to pass to all registered callbacks.
  */
  constexpr void call(type parameter) const noexcept;

private:
  static_assert(allocatedSize > 0, "CallbacksPool capacity must be greater than zero");

  //! Internal structure to store a callback function pointer.
  struct StaticCallback {
    void (*method)(type arg) = nullptr; //!< Pointer to the callback function.
  };

  array<StaticCallback, allocatedSize> _callbacks; //!< Fixed-size array storing registered callbacks.
  size_t _subscribersCount = 0; //!< Active callbacks count.
};

} // namespace toy

#endif // INCLUDE_CORE_CALLBACKS_POOL_HPP_
