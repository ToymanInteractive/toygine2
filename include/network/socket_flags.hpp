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
  \file   socket_flags.hpp
  \brief  Bit-flag options requested from a socket at creation.

  Defines \ref toy::network::SocketFlags, the bit-flag set a socket is created with; its enumerators combine through the
  bitwise operators of core.hpp. Used when opening a socket.

  \note Included by network.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_NETWORK_SOCKET_FLAGS_HPP_
#define INCLUDE_NETWORK_SOCKET_FLAGS_HPP_

namespace toy::network {

/*!
  \enum  SocketFlags
  \brief Options requested from the platform socket layer at creation.

  Each enumerator holds one bit, so a caller combines several into one value with the bitwise operators; \c None is the
  empty set.

  \sa ENABLE_BITWISE_OPERATORS
*/
enum class SocketFlags : uint8_t {
  None = 0x00,         ///< Empty set: no behavior requested.

  NonBlocking  = 0x01, ///< Requests a socket whose input and output calls return immediately instead of blocking.
  ReuseAddress = 0x02, ///< Requests that binding reuses a local address another socket still holds.
  Broadcast    = 0x04, ///< Requests permission to send to the broadcast address.
  Listen       = 0x08, ///< Requests that the socket enters the listening state.

  Bind = 0x10,         ///< Requests that the socket binds to a local address.
};

} // namespace toy::network

ENABLE_BITWISE_OPERATORS(toy::network, SocketFlags)

#endif // INCLUDE_NETWORK_SOCKET_FLAGS_HPP_
