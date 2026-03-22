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
  \file   network.hpp
  \brief  Umbrella header for the network module.

  Defines \ref toy::network::Endpoint, \ref toy::network::SocketError, \ref toy::network::SocketFlags,
  \ref toy::network::SocketStatistics, \ref toy::network::SocketType, and related networking types.

  Include this header only; do not include internal headers directly.
*/

#ifndef INCLUDE_NETWORK_HPP_
#define INCLUDE_NETWORK_HPP_

#include "core.hpp"

/*!
  \namespace toy::network
  \brief Network types: endpoint, socket error, flags, statistics, type, and related APIs.

  \sa toy::network::Endpoint, toy::network::SocketError, toy::network::SocketFlags, toy::network::SocketStatistics,
      toy::network::SocketType
*/

#include "network/endpoint.hpp"
#include "network/socket_error.hpp"
#include "network/socket_flags.hpp"
#include "network/socket_statistics.hpp"
#include "network/socket_type.hpp"

//--------------------------------------------------------------------------------------------------------------------

#include "network/endpoint.inl"

#endif // INCLUDE_NETWORK_HPP_
