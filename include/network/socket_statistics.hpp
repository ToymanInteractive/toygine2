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
  \file   socket_statistics.hpp
  \brief  Socket traffic statistics: bytes sent and received.

  Defines \ref toy::network::SocketStatistics with byte counters for sent and received data. Used to track traffic per
  socket or aggregate across connections.

  \note Included by network.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_NETWORK_SOCKET_STATISTICS_HPP_
#define INCLUDE_NETWORK_SOCKET_STATISTICS_HPP_

namespace toy::network {

/*!
  \struct SocketStatistics
  \brief Byte counters for socket traffic (sent and received).
*/
struct SocketStatistics {
  /// Total bytes sent on the socket.
  size_t bytesSent     = 0;
  /// Total bytes received on the socket.
  size_t bytesReceived = 0;
};

} // namespace toy::network

#endif // INCLUDE_NETWORK_SOCKET_STATISTICS_HPP_
