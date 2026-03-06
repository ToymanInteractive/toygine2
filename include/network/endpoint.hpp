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
  \file   endpoint.hpp
  \brief  Base type for network endpoint (host and port).
*/

#ifndef INCLUDE_NETWORK_ENDPOINT_HPP_
#define INCLUDE_NETWORK_ENDPOINT_HPP_

namespace toy::network {

/*!
  \class Endpoint
  \brief Abstract base for a network endpoint (hostname and port).

  Subclasses provide platform-specific storage and resolution. port() and setPort() operate on the stored port; hostname
  and resolution are handled by virtual methods.

  \section features Key Features

  - **Port**: Stored in host byte order; get/set via port() and setPort().
  - **Hostname**: Set and queried via setHostname() and hostnameAsText(); resolution is implementation-defined.
  - **Local host**: isLocalHost() and setLocalHost() for loopback/local address.

  \section usage Usage Example

  \code
  #include "network.hpp"

  void configureEndpoint(toy::network::Endpoint* ep) {
    ep->setPort(8080);
    ep->setHostname("example.com");
    const char* host = ep->hostnameAsText();
  }
  \endcode

  \section performance Performance Characteristics

  - **port() / setPort()**: O(1).
  - **Virtual methods**: Platform-dependent (resolution, allocation).

  \section safety Safety Guarantees

  - **hostnameAsText()**: Returned pointer valid until the endpoint is modified or destroyed.
  - **setHostname(\a name)**: \a name must point to valid memory for the duration of the call.

  \sa toy::network::SocketType
*/

class Endpoint {
public:
  virtual ~Endpoint() noexcept;

  /*!
    \brief Returns the port number.

    \return The port in host byte order.
  */
  [[nodiscard]] constexpr uint16_t port() const noexcept;

  /*!
    \brief Sets the port number.

    \param port Port in host byte order.
  */
  constexpr void setPort(uint16_t port) noexcept;

  /*!
    \brief Sets the hostname; resolution is implementation-defined.

    \param name Null-terminated hostname string.

    \return \c true on success, \c false on failure.
  */
  virtual bool setHostname(const char * name) = 0;

  /*!
    \brief Returns whether the endpoint represents the local host.

    \return \c true if local host, \c false otherwise.
  */
  virtual bool isLocalHost() const = 0;

  /*!
    \brief Sets the endpoint to the local host.
  */
  virtual void setLocalHost() = 0;

  /*!
    \brief Returns the hostname as a C string.

    \return Null-terminated hostname, or empty string if none set. Valid until the endpoint is modified or destroyed.
  */
  virtual const char * hostnameAsText() const = 0;

protected:
  constexpr Endpoint() noexcept = default;

private:
  uint16_t _port = 0;
};

} // namespace toy::network

#endif // INCLUDE_NETWORK_ENDPOINT_HPP_
