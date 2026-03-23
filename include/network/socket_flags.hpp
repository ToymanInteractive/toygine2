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
  \file   socket_flags.hpp
  \brief  Socket option and mode flags for the network module.

  Defines \ref toy::network::SocketFlags: bitmask flags for non-blocking mode, address reuse, broadcast, listen, and
  bind. Used when creating or configuring sockets.

  \note Included by network.hpp; do not include this file directly.
*/

#ifndef INCLUDE_NETWORK_SOCKET_FLAGS_HPP_
#define INCLUDE_NETWORK_SOCKET_FLAGS_HPP_

namespace toy::network {

/*!
  \enum SocketFlags
  \brief Bitmask flags for socket options and operating mode.

  Values can be combined (e.g. \c NonBlocking | \c ReuseAddress). Interpretation is
  platform-dependent; maps to socket options and flags such as \c O_NONBLOCK, \c SO_REUSEADDR.
*/
enum class SocketFlags : uint8_t {
  /// No flags set.
  None = 0x00,

  /// Socket operates in non-blocking mode.
  NonBlocking = 0x01,
  /// Allow local address reuse (e.g. \c SO_REUSEADDR).
  ReuseAddress = 0x02,
  /// Enable broadcast send/receive.
  Broadcast = 0x04,
  /// Socket is in listen state (for accepting connections).
  Listen = 0x08,

  /// Socket is bound to a local address.
  Bind = 0x10,
};

} // namespace toy::network

ENABLE_BITWISE_OPERATORS(toy::network::SocketFlags)

#endif // INCLUDE_NETWORK_SOCKET_FLAGS_HPP_
