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
  \file   socket_type.hpp
  \brief  Transport semantics a socket is created with.

  Defines \ref toy::network::SocketType, which names the datagram or stream transport a socket is created with.

  \note Included by network.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_NETWORK_SOCKET_TYPE_HPP_
#define INCLUDE_NETWORK_SOCKET_TYPE_HPP_

namespace toy::network {

/*!
  \enum  SocketType
  \brief Transport semantics requested from the platform socket layer at creation.

  The values never combine: a socket carries exactly one.

  \sa \ref toy::network::SocketFlags
*/
enum class SocketType : uint8_t {
  Datagram, ///< Connectionless transport where each send is one message.
  Stream,   ///< Connection-oriented transport of an ordered byte stream.
};

} // namespace toy::network

#endif // INCLUDE_NETWORK_SOCKET_TYPE_HPP_
