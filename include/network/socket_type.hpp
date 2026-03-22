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
  \file   socket_type.hpp
  \brief  Socket type enumeration for network APIs.

  Defines \ref toy::network::SocketType: kind of socket (datagram, stream, or dummy). Used when creating or configuring
  sockets.

  Included by network.hpp; do not include this file directly.
*/

#ifndef INCLUDE_NETWORK_SOCKET_TYPE_HPP_
#define INCLUDE_NETWORK_SOCKET_TYPE_HPP_

namespace toy::network {

/*!
  \enum SocketType
  \brief Kind of socket or socket-like endpoint.

  Used to distinguish datagram (UDP-style) vs stream (TCP-style) sockets. \ref toy::network::SocketType::Dummy is for
  tests or stubs that do not perform real I/O.
*/
enum class SocketType : uint8_t {
  /// Datagram socket (e.g. UDP); message-oriented, unreliable.
  Datagram,

  /// Stream socket (e.g. TCP); connection-oriented, reliable byte stream.
  Stream,

  /// Dummy socket for tests or stubs; no real network I/O.
  Dummy,
};

} // namespace toy::network

#endif // INCLUDE_NETWORK_SOCKET_TYPE_HPP_
