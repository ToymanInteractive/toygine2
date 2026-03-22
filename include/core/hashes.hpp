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
  \file   hashes.hpp
  \brief  Table-driven CRC-8, CRC-16, and CRC-32 over byte buffers.

  Declares crc8(), crc16(), and crc32() in namespace \ref toy.

  Included by \ref core.hpp; do not include this file directly.
*/

#ifndef INCLUDE_CORE_HASHES_HPP_
#define INCLUDE_CORE_HASHES_HPP_

namespace toy {

/*!
  \brief Computes Dallas/Maxim CRC-8 (polynomial 0x31, reflected 8-bit CRC).

  Processes bytes from low to high address. The \a crc argument carries state for chained updates: first chunk uses the
  default initial value, subsequent chunks pass the previous result as \a crc.

  \param src  Pointer to the first byte; must not be null (asserted in debug).
  \param size Byte length of the range.
  \param crc  Running CRC; default initial value \c 0x00.

  \return Updated CRC-8 value.

  \pre \a src is not null. \a size is the length of readable memory at \a src.

  \note If \a size is \c 0, the return value equals the initial \a crc argument (no input bytes).

  \note Polynomial matches Dallas/Maxim style CRC-8 (0x31 reflected); compare vectors with your protocol’s CRC catalogue
        entry.
*/
[[nodiscard]] uint8_t crc8(const void * src, size_t size, uint8_t crc = 0x00) noexcept;

/*!
  \brief Computes CRC-16-IBM / ARC (polynomial 0x8005, reflected).

  Processes bytes from low to high address. The \a crc argument carries state for chained updates: first chunk uses the
  default initial value, subsequent chunks pass the previous result as \a crc.

  \param src  Pointer to the first byte; must not be null (asserted in debug).
  \param size Byte length of the range.
  \param crc  Running CRC; default initial value \c 0x0000.

  \return Updated CRC-16 value.

  \pre \a src is not null. \a size is the length of readable memory at \a src.

  \note If \a size is \c 0, the return value equals the initial \a crc argument (no input bytes).

  \note Polynomial is CRC-16-IBM / ARC; confirm against Modbus, ARC, or other specs that name this variant.
*/
[[nodiscard]] uint16_t crc16(const void * src, size_t size, uint16_t crc = 0x0000) noexcept;

/*!
  \brief Computes CRC-32 per IEEE 802.3 / Ethernet (polynomial 0x04C11DB7, reflected).

  Processes bytes from low to high address. The \a crc argument carries state for chained updates: first chunk uses the
  default initial value, subsequent chunks pass the previous result as \a crc.

  \param src  Pointer to the first byte; must not be null (asserted in debug).
  \param size Byte length of the range.
  \param crc  Running CRC; default initial value \c 0x00000000.

  \return Updated CRC-32 value.

  \pre \a src is not null. \a size is the length of readable memory at \a src.

  \note If \a size is \c 0, the return value equals the initial \a crc argument (no input bytes).

  \note Result matches common IEEE 802.3 / Ethernet / PKZIP polynomial; parameterization (init, xorout) may differ by
        protocol—use known test vectors for integration.
*/
[[nodiscard]] uint32_t crc32(const void * src, size_t size, uint32_t crc = 0x00000000) noexcept;

} // namespace toy

#endif // INCLUDE_CORE_HASHES_HPP_
