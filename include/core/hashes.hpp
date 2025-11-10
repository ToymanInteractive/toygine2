//
// Copyright (c) 2025 Toyman Interactive
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
  \brief  CRC hash functions.
*/

#ifndef INCLUDE_CORE_HASHES_HPP_
#define INCLUDE_CORE_HASHES_HPP_

namespace toy {

/*!
  \brief Calculates CRC-8 checksum using Dallas/Maxim polynomial.

  This function computes a CRC-8 checksum for the given data using the Dallas/Maxim polynomial (x⁸ + x⁵ + x⁴ + 1, 0x31).
  The CRC-8 algorithm provides a fast and efficient way to detect data corruption and is widely used in embedded
  systems, communication protocols, and data integrity verification.

  \param src  The source data to calculate CRC-8 for.
  \param size The size of the data in bytes.
  \param crc  The initial CRC value (default: \c 0x00).

  \return The calculated CRC-8 checksum value.

  \pre The \a src pointer must not be null.
  \pre The \a size must be valid for the data pointed to by \a src.

  \note This function uses the Dallas/Maxim CRC-8 polynomial (0x31), which is commonly used in 1-Wire protocols and
        embedded systems.
  \note The function is noexcept and does not throw exceptions.
  \note For empty data (size = \c 0), the function returns the initial CRC value.

  \see https://en.wikipedia.org/wiki/Cyclic_redundancy_check
  \see https://reveng.sourceforge.io/crc-catalogue/1-15.htm#crc.cat-bits.8
*/
[[nodiscard]] uint8_t crc8(const void * src, size_t size, uint8_t crc = 0x00) noexcept;

/*!
  \brief Calculates CRC-16 checksum using IBM/ARC polynomial.

  This function computes a CRC-16 checksum for the given data using the IBM/ARC polynomial (x¹⁶ + x¹⁵ + x² + 1, 0x8005).
  The CRC-16 algorithm provides a reliable way to detect data corruption and is widely used in communication protocols,
  file systems, and data integrity verification systems.

  The CRC-16-IBM/ARC algorithm is one of the most commonly used CRC-16 variants and is
  supported by many standards including:
  - IBM SNA (Systems Network Architecture)
  - ARC (Archive) file format
  - LHA compression format
  - Modbus RTU protocol
  - USB data packets

  \param src  The source data to calculate CRC-16 for.
  \param size The size of the data in bytes.
  \param crc  The initial CRC value (default: \c 0x0000).

  \return The calculated CRC-16 checksum value.

  \pre The \a src pointer must not be null.
  \pre The \a size must be valid for the data pointed to by \a src.

  \note This function uses the IBM/ARC CRC-16 polynomial (0x8005), which is commonly used in communication protocols and
        file systems.
  \note The function is noexcept and does not throw exceptions.
  \note For empty data (size = \c 0), the function returns the initial CRC value.

  \see https://en.wikipedia.org/wiki/Cyclic_redundancy_check
  \see https://reveng.sourceforge.io/crc-catalogue/16.htm#crc.cat.crc-16-arc
*/
[[nodiscard]] uint16_t crc16(const void * src, size_t size, uint16_t crc = 0x0000) noexcept;

/*!
  \brief Calculates CRC-32 checksum using IEEE 802.3 polynomial.

  This function computes a CRC-32 checksum for the given data using the IEEE 802.3 polynomial
  (x³² + x²⁶ + x²³ + x²² + x¹⁶ + x¹² + x¹¹ + x¹⁰ + x⁸ + x⁷ + x⁵ + x⁴ + x² + x + 1, 0x04C11DB7).
  The CRC-32 algorithm provides excellent error detection capabilities and is widely used in networking protocols, file
  systems, and data integrity verification.

  The CRC-32-IEEE 802.3 algorithm is the most commonly used CRC-32 variant and is supported by many standards including:
  - IEEE 802.3 (Ethernet)
  - ZIP file format
  - PNG image format
  - MPEG-2 transport streams
  - SATA disk drives
  - IEEE 1394 (FireWire)

  \param src  The source data to calculate CRC-32 for.
  \param size The size of the data in bytes.
  \param crc  The initial CRC value (default: \c 0x00000000).

  \return The calculated CRC-32 checksum value.

  \pre The \a src pointer must not be null.
  \pre The \a size must be valid for the data pointed to by \a src.

  \note This function uses the IEEE 802.3 CRC-32 polynomial (0x04C11DB7), which is the most widely used CRC-32 variant.
  \note The function is noexcept and does not throw exceptions.
  \note For empty data (size = \c 0), the function returns the initial CRC value.

  \see https://en.wikipedia.org/wiki/Cyclic_redundancy_check
  \see https://reveng.sourceforge.io/crc-catalogue/17plus.htm#crc.cat-bits.32
*/
[[nodiscard]] uint32_t crc32(const void * src, size_t size, uint32_t crc = 0x00000000) noexcept;

} // namespace toy

#endif // INCLUDE_CORE_HASHES_HPP_
