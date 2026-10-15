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
  \file   string.c
  \brief  Sega MD implementations of the \c string.h functions the ClownMDSDK overlay declares.

  memcpy(), memset() and memcmp() are 68000 assembly that uses only the scratch registers d0, d1, a0 and a1; sizes of
  64 KiB and more go to C fallbacks. Stack offsets follow the \c -mshort ABI, where \c int takes 16 bits, so memset()
  finds the fill byte at 9(sp) and the count at 10(sp).
*/

#include <stdint.h>
#include <string.h>

/*!
  \brief  Repeats a byte in all four bytes of an \c unsigned \c long.

  \param  ch  Byte value to repeat.
  \return \a ch in every byte of the result.
  \pre    \a ch is in [0, 255]; a wider value corrupts the upper bytes.
*/
static unsigned long _RepeatByteToUnsignedLong(const int ch)
{
  unsigned long value = 0;

  unsigned int i;

  for (i = 0; i < sizeof(unsigned long); ++i)
  {
    value <<= 8;
    value |= ch;
  }

  return value;
}

/// Handles sizes of 64 KiB and more, which the 16-bit counters of memcpy() cannot count; only its assembly calls it.
static __attribute__((used, noinline)) void* _MemcpyLarge(void* const dest, const void* const src, size_t count)
{
  unsigned char *source = (unsigned char*)src;
  unsigned char *destination = (unsigned char*)dest;

  if (count == 0)
    return dest;

  // Slow path where one is word-aligned but the other isn't.
  if ((((uintptr_t)destination ^ (uintptr_t)source) & 1) != 0)
  {
    size_t count_div_10 = count / 0x10;

    __asm__ volatile(
      "jmp  1f(%%pc,%3.w)\n"
      "0:\n"
      "  move.b  (%2)+,(%0)+\n"
      "  move.b  (%2)+,(%0)+\n"
      "  move.b  (%2)+,(%0)+\n"
      "  move.b  (%2)+,(%0)+\n"
      "  move.b  (%2)+,(%0)+\n"
      "  move.b  (%2)+,(%0)+\n"
      "  move.b  (%2)+,(%0)+\n"
      "  move.b  (%2)+,(%0)+\n"
      "  move.b  (%2)+,(%0)+\n"
      "  move.b  (%2)+,(%0)+\n"
      "  move.b  (%2)+,(%0)+\n"
      "  move.b  (%2)+,(%0)+\n"
      "  move.b  (%2)+,(%0)+\n"
      "  move.b  (%2)+,(%0)+\n"
      "  move.b  (%2)+,(%0)+\n"
      "  move.b  (%2)+,(%0)+\n"
      "1:\n"
      "  dbf  %1,0b\n"
      "  clr.w  %1\n"
      "  subq.l  #1,%1\n"
      "  bcc.s  0b\n"
      : "+a" (destination), "+d" (count_div_10), "+a" (source)
      : "da" (-((count % 0x10) * 2))
      : "cc", "memory"
    );

    return dest;
  }

  if (((uintptr_t)destination & 1) != 0)
  {
    __asm__ volatile(
      "move.b  (%1)+,(%0)+\n"
      : "+a" (destination), "+a" (source)
      :
      : "cc", "memory"
    );

    --count;
  }

  if (count >= 4)
  {
    size_t count_div_10 = count / 0x10;

    __asm__ volatile(
      "jmp  1f(%%pc,%3.w)\n"
      "0:\n"
      "  move.l  (%2)+,(%0)+\n"
      "  move.l  (%2)+,(%0)+\n"
      "  move.l  (%2)+,(%0)+\n"
      "  move.l  (%2)+,(%0)+\n"
      "1:\n"
      "  dbf  %1,0b\n"
      "  clr.w  %1\n"
      "  subq.l  #1,%1\n"
      "  bcc.s  0b\n"
      : "+a" (destination), "+d" (count_div_10), "+a" (source)
      : "da" (-((count % 0x10 / 4) * 2))
      : "cc", "memory"
    );
  }

  if ((count & 2) != 0)
  {
    __asm__ volatile(
      "move.w  (%1)+,(%0)+\n"
      : "+a" (destination), "+a" (source)
      :
      : "cc", "memory"
    );
  }

  if ((count & 1) != 0)
  {
    __asm__ volatile(
      "move.b  (%1)+,(%0)+\n"
      : "+a" (destination), "+a" (source)
      :
      : "cc", "memory"
    );
  }

  return dest;
}

// Copies under 28 bytes use a byte table; longer ones align both pointers and move 64 bytes per pass.
__asm__(
  ".pushsection .text\n"
  ".globl memcpy\n"
  ".type memcpy,@function\n"
  ".even\n"
  "memcpy:\n"
  "  movea.l 4(%sp),%a0\n"
  "  movea.l 8(%sp),%a1\n"
  "  move.l  12(%sp),%d0\n"
  "  moveq   #28,%d1\n"
  "  cmp.l   %d1,%d0\n"
  "  bcc.s   .Lmemcpy_long\n"
  // Jumps back 'count' instructions from the end of the table.
  "  add.w   %d0,%d0\n"
  "  neg.w   %d0\n"
  "  jmp     .Lmemcpy_short_end(%pc,%d0.w)\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  ".Lmemcpy_short_end:\n"
  "  move.l  4(%sp),%d0\n"
  "  rts\n"
  ".Lmemcpy_large:\n"
  "  bra.w   _MemcpyLarge\n"
  ".Lmemcpy_long:\n"
  "  tst.w   12(%sp)\n"
  "  bne.s   .Lmemcpy_large\n"
  // The low bit of the pointer difference is set when the two pointers have different parity.
  "  move.w  %a0,%d1\n"
  "  sub.w   %a1,%d1\n"
  "  lsr.w   #1,%d1\n"
  "  bcs.s   .Lmemcpy_bytes\n"
  "  move.w  %a0,%d1\n"
  "  lsr.w   #1,%d1\n"
  "  bcc.s   .Lmemcpy_even\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  subq.w  #1,%d0\n"
  ".Lmemcpy_even:\n"
  // The high word of d1 keeps the count for the tail, the low word counts 64-byte passes for 'dbf'.
  "  move.w  %d0,%d1\n"
  "  swap    %d1\n"
  "  move.w  %d0,%d1\n"
  "  lsr.w   #6,%d1\n"
  // Enters the loop body past the longwords the first pass skips.
  "  lsr.w   #1,%d0\n"
  "  andi.w  #0x1e,%d0\n"
  "  neg.w   %d0\n"
  "  jmp     .Lmemcpy_longs_end(%pc,%d0.w)\n"
  ".Lmemcpy_longs:\n"
  "  move.l  (%a1)+,(%a0)+\n"
  "  move.l  (%a1)+,(%a0)+\n"
  "  move.l  (%a1)+,(%a0)+\n"
  "  move.l  (%a1)+,(%a0)+\n"
  "  move.l  (%a1)+,(%a0)+\n"
  "  move.l  (%a1)+,(%a0)+\n"
  "  move.l  (%a1)+,(%a0)+\n"
  "  move.l  (%a1)+,(%a0)+\n"
  "  move.l  (%a1)+,(%a0)+\n"
  "  move.l  (%a1)+,(%a0)+\n"
  "  move.l  (%a1)+,(%a0)+\n"
  "  move.l  (%a1)+,(%a0)+\n"
  "  move.l  (%a1)+,(%a0)+\n"
  "  move.l  (%a1)+,(%a0)+\n"
  "  move.l  (%a1)+,(%a0)+\n"
  "  move.l  (%a1)+,(%a0)+\n"
  ".Lmemcpy_longs_end:\n"
  "  dbf     %d1,.Lmemcpy_longs\n"
  "  swap    %d1\n"
  "  btst    #1,%d1\n"
  "  beq.s   .Lmemcpy_no_word\n"
  "  move.w  (%a1)+,(%a0)+\n"
  ".Lmemcpy_no_word:\n"
  "  btst    #0,%d1\n"
  "  beq.s   .Lmemcpy_done\n"
  "  move.b  (%a1)+,(%a0)+\n"
  ".Lmemcpy_done:\n"
  "  move.l  4(%sp),%d0\n"
  "  rts\n"
  // Pointers of different parity cannot move words, so this path copies 32 single bytes per loop pass.
  ".Lmemcpy_bytes:\n"
  "  move.w  %d0,%d1\n"
  "  lsr.w   #5,%d1\n"
  "  andi.w  #31,%d0\n"
  "  add.w   %d0,%d0\n"
  "  neg.w   %d0\n"
  "  jmp     .Lmemcpy_bytes_end(%pc,%d0.w)\n"
  ".Lmemcpy_bytes_loop:\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  "  move.b  (%a1)+,(%a0)+\n"
  ".Lmemcpy_bytes_end:\n"
  "  dbf     %d1,.Lmemcpy_bytes_loop\n"
  "  move.l  4(%sp),%d0\n"
  "  rts\n"
  ".size memcpy,.-memcpy\n"
  ".popsection\n"
);

void* memmove(void* const dest, const void* const src, size_t count)
{
  unsigned char *source = (unsigned char*)src + count;
  unsigned char *destination = (unsigned char*)dest + count;

  // 'memcpy' copies forwards, which is safe unless the destination begins inside the source.
  if ((uintptr_t)dest - (uintptr_t)src >= count)
    return memcpy(dest, src, count);

  // Otherwise, copy backwards, mirroring 'memcpy'.

  // Slow path where one is word-aligned but the other isn't.
  if ((((uintptr_t)destination ^ (uintptr_t)source) & 1) != 0)
  {
    size_t count_div_10 = count / 0x10;

    __asm__ volatile(
      "jmp  1f(%%pc,%3.w)\n"
      "0:\n"
      "  move.b  -(%2),-(%0)\n"
      "  move.b  -(%2),-(%0)\n"
      "  move.b  -(%2),-(%0)\n"
      "  move.b  -(%2),-(%0)\n"
      "  move.b  -(%2),-(%0)\n"
      "  move.b  -(%2),-(%0)\n"
      "  move.b  -(%2),-(%0)\n"
      "  move.b  -(%2),-(%0)\n"
      "  move.b  -(%2),-(%0)\n"
      "  move.b  -(%2),-(%0)\n"
      "  move.b  -(%2),-(%0)\n"
      "  move.b  -(%2),-(%0)\n"
      "  move.b  -(%2),-(%0)\n"
      "  move.b  -(%2),-(%0)\n"
      "  move.b  -(%2),-(%0)\n"
      "  move.b  -(%2),-(%0)\n"
      "1:\n"
      "  dbf  %1,0b\n"
      "  clr.w  %1\n"
      "  subq.l  #1,%1\n"
      "  bcc.s  0b\n"
      : "+a" (destination), "+d" (count_div_10), "+a" (source)
      : "da" (-((count % 0x10) * 2))
      : "cc", "memory"
    );

    return dest;
  }

  if (((uintptr_t)destination & 1) != 0)
  {
    __asm__ volatile(
      "move.b  -(%1),-(%0)\n"
      : "+a" (destination), "+a" (source)
      :
      : "cc", "memory"
    );

    --count;
  }

  if (count >= 4)
  {
    size_t count_div_10 = count / 0x10;

    __asm__ volatile(
      "jmp  1f(%%pc,%3.w)\n"
      "0:\n"
      "  move.l  -(%2),-(%0)\n"
      "  move.l  -(%2),-(%0)\n"
      "  move.l  -(%2),-(%0)\n"
      "  move.l  -(%2),-(%0)\n"
      "1:\n"
      "  dbf  %1,0b\n"
      "  clr.w  %1\n"
      "  subq.l  #1,%1\n"
      "  bcc.s  0b\n"
      : "+a" (destination), "+d" (count_div_10), "+a" (source)
      : "da" (-((count % 0x10 / 4) * 2))
      : "cc", "memory"
    );
  }

  if ((count & 2) != 0)
  {
    __asm__ volatile(
      "move.w  -(%1),-(%0)\n"
      : "+a" (destination), "+a" (source)
      :
      : "cc", "memory"
    );
  }

  if ((count & 1) != 0)
  {
    __asm__ volatile(
      "move.b  -(%1),-(%0)\n"
      : "+a" (destination), "+a" (source)
      :
      : "cc", "memory"
    );
  }

  return dest;
}

/// Handles sizes of 64 KiB and more, which the 16-bit counter of memset() cannot count; only its assembly calls it.
static __attribute__((used, noinline)) void* _MemsetLarge(void* const dest, const int ch, size_t count)
{
  unsigned char *destination = (unsigned char*)dest;

  if (count == 0)
    return dest;

  const unsigned long value = _RepeatByteToUnsignedLong((unsigned char)ch);

  if (((uintptr_t)destination & 1) != 0)
  {
    __asm__ volatile(
      "move.b  %1,(%0)+\n"
      : "+a" (destination)
      : "d" (value)
      : "cc", "memory"
    );

    --count;
  }

  if (count >= 4)
  {
    size_t count_div_10 = count / 0x10;

    __asm__ volatile(
      "jmp  1f(%%pc,%2.w)\n"
      "0:\n"
      "  move.l  %3,(%0)+\n"
      "  move.l  %3,(%0)+\n"
      "  move.l  %3,(%0)+\n"
      "  move.l  %3,(%0)+\n"
      "1:\n"
      "  dbf  %1,0b\n"
      "  clr.w  %1\n"
      "  subq.l  #1,%1\n"
      "  bcc.s  0b\n"
      : "+a" (destination), "+d" (count_div_10)
      : "da" (-((count % 0x10 / 4) * 2)), "d" (value)
      : "cc", "memory"
    );
  }

  if ((count & 2) != 0)
  {
    __asm__ volatile(
      "move.w  %1,(%0)+\n"
      : "+a" (destination)
      : "d" (value)
      : "cc", "memory"
    );
  }

  if ((count & 1) != 0)
  {
    __asm__ volatile(
      "move.b  %1,(%0)+\n"
      : "+a" (destination)
      : "d" (value)
      : "cc", "memory"
    );
  }

  return dest;
}

// Fills under 44 bytes use a byte table; longer ones align the pointer and store 64 bytes per pass.
__asm__(
  ".pushsection .text\n"
  ".globl memset\n"
  ".type memset,@function\n"
  ".even\n"
  "memset:\n"
  "  movea.l 4(%sp),%a0\n"
  "  move.l  10(%sp),%d0\n"
  "  moveq   #44,%d1\n"
  "  cmp.l   %d1,%d0\n"
  "  bcc.s   .Lmemset_long\n"
  "  move.b  9(%sp),%d1\n"
  // Jumps back 'count' instructions from the end of the table.
  "  add.w   %d0,%d0\n"
  "  neg.w   %d0\n"
  "  jmp     .Lmemset_short_end(%pc,%d0.w)\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  "  move.b  %d1,(%a0)+\n"
  ".Lmemset_short_end:\n"
  "  move.l  4(%sp),%d0\n"
  "  rts\n"
  ".Lmemset_large:\n"
  "  bra.w   _MemsetLarge\n"
  ".Lmemset_long:\n"
  "  tst.w   10(%sp)\n"
  "  bne.s   .Lmemset_large\n"
  "  move.w  %a0,%d1\n"
  "  lsr.w   #1,%d1\n"
  "  bcc.s   .Lmemset_even\n"
  "  move.b  9(%sp),(%a0)+\n"
  "  subq.w  #1,%d0\n"
  ".Lmemset_even:\n"
  // The jump offset goes to a1, leaving d0 for the count (high word) and the pass counter (low word).
  "  move.w  %d0,%d1\n"
  "  lsr.w   #1,%d1\n"
  "  andi.w  #0x1e,%d1\n"
  "  neg.w   %d1\n"
  "  movea.w %d1,%a1\n"
  "  move.w  %d0,%d1\n"
  "  swap    %d0\n"
  "  move.w  %d1,%d0\n"
  "  lsr.w   #6,%d0\n"
  // Spreads the byte over a longword through the argument slots, which belong to the callee.
  "  move.b  9(%sp),8(%sp)\n"
  "  move.w  8(%sp),10(%sp)\n"
  "  move.l  8(%sp),%d1\n"
  "  jmp     .Lmemset_longs_end(%pc,%a1.w)\n"
  ".Lmemset_longs:\n"
  "  move.l  %d1,(%a0)+\n"
  "  move.l  %d1,(%a0)+\n"
  "  move.l  %d1,(%a0)+\n"
  "  move.l  %d1,(%a0)+\n"
  "  move.l  %d1,(%a0)+\n"
  "  move.l  %d1,(%a0)+\n"
  "  move.l  %d1,(%a0)+\n"
  "  move.l  %d1,(%a0)+\n"
  "  move.l  %d1,(%a0)+\n"
  "  move.l  %d1,(%a0)+\n"
  "  move.l  %d1,(%a0)+\n"
  "  move.l  %d1,(%a0)+\n"
  "  move.l  %d1,(%a0)+\n"
  "  move.l  %d1,(%a0)+\n"
  "  move.l  %d1,(%a0)+\n"
  "  move.l  %d1,(%a0)+\n"
  ".Lmemset_longs_end:\n"
  "  dbf     %d0,.Lmemset_longs\n"
  "  swap    %d0\n"
  "  btst    #1,%d0\n"
  "  beq.s   .Lmemset_no_word\n"
  "  move.w  %d1,(%a0)+\n"
  ".Lmemset_no_word:\n"
  "  btst    #0,%d0\n"
  "  beq.s   .Lmemset_done\n"
  "  move.b  %d1,(%a0)+\n"
  ".Lmemset_done:\n"
  "  move.l  4(%sp),%d0\n"
  "  rts\n"
  ".size memset,.-memset\n"
  ".popsection\n"
);

/// Handles sizes of 64 KiB and more, which the 16-bit counters of memcmp() cannot count; only its assembly calls it.
static __attribute__((used, noinline)) int _MemcmpLarge(const void* const lhs, const void* const rhs,
                                                        const size_t count)
{
  const unsigned char* const left = lhs;
  const unsigned char* const right = rhs;
  size_t index;

  // Report the ordering of the first pair of bytes that differ, compared as unsigned.
  for (index = 0; index != count; ++index)
    if (left[index] != right[index])
      return left[index] < right[index] ? -1 : 1;

  return 0;
}

// Compares bytes under 12 bytes or on pointers of different parity, otherwise longwords.
// On the big-endian 68000 the unsigned order of longwords is the byte order.
__asm__(
  ".pushsection .text\n"
  ".globl memcmp\n"
  ".type memcmp,@function\n"
  ".even\n"
  "memcmp:\n"
  "  movea.l 4(%sp),%a0\n"
  "  movea.l 8(%sp),%a1\n"
  "  move.l  12(%sp),%d0\n"
  "  beq.s   .Lmemcmp_equal\n"
  "  moveq   #12,%d1\n"
  "  cmp.l   %d1,%d0\n"
  "  bcc.s   .Lmemcmp_long\n"
  ".Lmemcmp_bytes:\n"
  "  subq.w  #1,%d0\n"
  ".Lmemcmp_byte_loop:\n"
  "  cmpm.b  (%a1)+,(%a0)+\n"
  "  dbne    %d0,.Lmemcmp_byte_loop\n"
  "  bne.s   .Lmemcmp_differ\n"
  ".Lmemcmp_equal:\n"
  "  moveq   #0,%d0\n"
  "  rts\n"
  ".Lmemcmp_differ:\n"
  "  bcs.s   .Lmemcmp_less\n"
  "  moveq   #1,%d0\n"
  "  rts\n"
  ".Lmemcmp_less:\n"
  "  moveq   #-1,%d0\n"
  "  rts\n"
  ".Lmemcmp_large:\n"
  "  bra.w   _MemcmpLarge\n"
  ".Lmemcmp_long:\n"
  "  tst.w   12(%sp)\n"
  "  bne.s   .Lmemcmp_large\n"
  "  move.w  %a0,%d1\n"
  "  sub.w   %a1,%d1\n"
  "  lsr.w   #1,%d1\n"
  "  bcs.s   .Lmemcmp_bytes\n"
  "  move.w  %a0,%d1\n"
  "  lsr.w   #1,%d1\n"
  "  bcc.s   .Lmemcmp_even\n"
  "  cmpm.b  (%a1)+,(%a0)+\n"
  "  bne.s   .Lmemcmp_differ\n"
  "  subq.w  #1,%d0\n"
  ".Lmemcmp_even:\n"
  "  move.w  %d0,%d1\n"
  "  lsr.w   #2,%d0\n"
  "  subq.w  #1,%d0\n"
  "  bcs.s   .Lmemcmp_tail\n"
  ".Lmemcmp_long_loop:\n"
  "  cmpm.l  (%a1)+,(%a0)+\n"
  "  dbne    %d0,.Lmemcmp_long_loop\n"
  "  bne.s   .Lmemcmp_differ\n"
  ".Lmemcmp_tail:\n"
  "  andi.w  #3,%d1\n"
  "  beq.s   .Lmemcmp_equal\n"
  "  subq.w  #1,%d1\n"
  ".Lmemcmp_tail_loop:\n"
  "  cmpm.b  (%a1)+,(%a0)+\n"
  "  dbne    %d1,.Lmemcmp_tail_loop\n"
  "  bne.s   .Lmemcmp_differ\n"
  "  moveq   #0,%d0\n"
  "  rts\n"
  ".size memcmp,.-memcmp\n"
  ".popsection\n"
);

void* memchr(const void* const ptr, const int ch, const size_t count)
{
  const unsigned char* const bytes = ptr;
  const unsigned char value = (unsigned char)ch;
  size_t index;

  for (index = 0; index != count; ++index)
    if (bytes[index] == value)
      return (void*)&bytes[index];

  return NULL;
}

size_t strlen(const char* const str)
{
  const char* end = str;

  // Post-increment in the condition compiles to a two-instruction loop: 'tst.b (a0)+' and 'bne'.
  while (*end++ != '\0');

  return (size_t)(end - str - 1);
}

char* strncpy(char* const dest, const char* const src, const size_t count)
{
  char* to = dest;
  const char* from = src;
  char* const end = dest + count;

  // Copy the source with its terminator; the copied byte sets the flags, so no separate test is needed.
  while (to != end && (*to++ = *from++) != '\0');

  memset(to, '\0', (size_t)(end - to));

  return dest;
}
