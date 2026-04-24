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
  \brief  Network endpoint (host and port).

  Defines \ref toy::network::Endpoint.

  \note Included by network.hpp only; do not include this file directly.
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

  void configureEndpoint(toy::network::Endpoint * ep) {
    ep->setPort(8080);
    if (ep->setHostname("example.com")) {
      const char* host = ep->hostnameAsText();
      (void)host;
    }
  }
  \endcode

  \section performance Performance Characteristics

  - **port() / setPort()**: O(1).
  - **Virtual methods**: Platform-dependent (resolution, allocation).

  \section safety Safety Guarantees

  - **hostnameAsText()**: Returned pointer valid until the endpoint is modified or destroyed.
  - **setHostname(\a name)**: \a name must point to valid memory for the duration of the call.

  \sa \ref toy::network::SocketType
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

    \pre \a name != nullptr.

    \post On success, hostnameAsText() returns a string equal to \a name and the object is in a valid state.

    \return \c true on success; \c false on invalid input or resolution failure. Caller should check before using
            hostnameAsText().
  */
  virtual bool setHostname(const char * name) noexcept = 0;

  /*!
    \brief Returns whether the endpoint represents the local host.

    \return \c true if local host, \c false otherwise.
  */
  [[nodiscard]] virtual bool isLocalHost() const noexcept = 0;

  /*!
    \brief Sets the endpoint to the local host.
  */
  virtual void setLocalHost() noexcept = 0;

  /*!
    \brief Returns the hostname as a C string.

    \return Null-terminated hostname, or empty string if none set. Valid until the endpoint is modified or destroyed.
  */
  [[nodiscard]] virtual const char * hostnameAsText() const noexcept = 0;

protected:
  /// Default constructor.
  constexpr Endpoint() noexcept = default;

private:
  /// Port in host byte order.
  uint16_t _port = 0;
};

} // namespace toy::network

#endif // INCLUDE_NETWORK_ENDPOINT_HPP_
