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
  \file   hashes.hpp
  \brief  Cyclic redundancy checks over a byte range.

  Defines toy::hash::crc8(), toy::hash::crc16() and toy::hash::crc32(), the cyclic redundancy checks of \ref toy::hash.
  Used where stored or transmitted data needs an integrity value, from a save block to a baked asset.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_HASHES_HPP_
#define INCLUDE_CORE_HASHES_HPP_

namespace toy::hash {

/*!
  \brief  CRC-8/MAXIM checksum of \a data, continued from the running value \a crc.

  Runs the reflected polynomial 0x31, the Dallas one-wire sum, over every byte starting from \a crc.

  \param data  Bytes to checksum.
  \param crc   Running value to continue (default: \c 0x00 starts a new sum).

  \return The checksum of \a data with \a crc carried in; \a crc itself when \a data is empty.

  \pre \a data names readable bytes for the duration of the call.
  \pre \a data is empty or names a non-null pointer, checked by assert_message in debug builds.

  \sa toy::hash::crc16(), toy::hash::crc32()
*/
[[nodiscard]] uint8_t crc8(span<const byte> data, uint8_t crc = 0x00) noexcept;

/*!
  \brief  CRC-16/ARC checksum of \a data, continued from the running value \a crc.

  Runs the reflected polynomial 0x8005, the ARC sum, over every byte starting from \a crc.

  \param data  Bytes to checksum.
  \param crc   Running value to continue (default: \c 0x0000 starts a new sum).

  \return The checksum of \a data with \a crc carried in; \a crc itself when \a data is empty.

  \pre \a data names readable bytes for the duration of the call.
  \pre \a data is empty or names a non-null pointer, checked by assert_message in debug builds.

  \sa toy::hash::crc8(), toy::hash::crc32()
*/
[[nodiscard]] uint16_t crc16(span<const byte> data, uint16_t crc = 0x0000) noexcept;

/*!
  \brief  CRC-32/ISO-HDLC checksum of \a data, continued from the running value \a crc.

  Runs the reflected polynomial 0xEDB88320, the zlib sum, over every byte starting from \a crc.

  \param data  Bytes to checksum.
  \param crc   Running value to continue (default: \c 0x00000000 starts a new sum).

  \return The checksum of \a data with \a crc carried in; \a crc itself when \a data is empty.

  \pre \a data names readable bytes for the duration of the call.
  \pre \a data is empty or names a non-null pointer, checked by assert_message in debug builds.

  \sa toy::hash::crc8(), toy::hash::crc16()
*/
[[nodiscard]] uint32_t crc32(span<const byte> data, uint32_t crc = 0x00000000) noexcept;

} // namespace toy::hash

#endif // INCLUDE_CORE_HASHES_HPP_
