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
  \file   socket_error.hpp
  \brief  Socket and network operation error codes.
*/

#ifndef INCLUDE_NETWORK_SOCKET_ERROR_HPP_
#define INCLUDE_NETWORK_SOCKET_ERROR_HPP_

namespace toy::network {

/*!
  \enum SocketError
  \brief Error codes for socket and network operations.

  Returned by socket APIs to indicate failure reason. Use for error handling and diagnostics.
*/
enum class SocketError {
  /// No error; operation succeeded.
  None,

  /// Operation not allowed in current state (e.g. send on closed socket).
  InvalidOperation,
  /// I/O failure during read or write.
  IOError,
  /// Invalid or unsupported address format.
  InvalidAddress,
  /// Address or port already in use.
  AddressInUse,
  /// Socket handle is invalid or closed.
  SocketInvalid,
  /// Operation would block (non-blocking mode).
  WouldBlock,
  /// Operation timed out.
  Timeout,
  /// Memory allocation failed.
  MemoryError,
  /// Platform-specific system error.
  SystemError,

  /// Sentinel for iteration or testing; not a real error.
  Dummy
};

} // namespace toy::network

#endif // INCLUDE_NETWORK_SOCKET_ERROR_HPP_
