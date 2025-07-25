//
// Copyright (c) 2025 by Toyman Interactive
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
#include <core.hpp>

namespace toygine
{

  wchar_t * utf8toWChar(wchar_t * dest, std::size_t destSize, char const * src, std::size_t srcLen)
  {
    if (dest == nullptr || destSize == 0)
      return nullptr;

    wchar_t * destPointer = dest;
    if (srcLen > 0 && src != nullptr) {
      wchar_t const * unicodeEndPos = dest + (destSize - 1);
      std::size_t srcIterator = 0;

      while (srcIterator < srcLen && destPointer < unicodeEndPos) {
        std::uint8_t symbol = static_cast<std::uint8_t>(src[srcIterator++]);
        if (symbol <= 0x7F) {
          *destPointer = symbol;
        }
        else {
          std::size_t charBytes = 0;
          while (symbol & 0x80) {
            ++charBytes;
            symbol <<= 1;
          }

          wchar_t unicodeChar = static_cast<wchar_t>(symbol >> charBytes);
          while (charBytes-- > 1) {
            unicodeChar <<= 6;
            unicodeChar |= src[srcIterator++] & 0x3F;
          }

          *destPointer = unicodeChar;
        }
        ++destPointer;
      }
    }

    *destPointer = L'\0';

    return dest;
  }

} // namespace toygine
