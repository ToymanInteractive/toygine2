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
  \file   memory.hpp
  \brief  Heap allocation macros the engine calls in place of the C allocation functions.

  Defines \c MALLOC, \c CALLOC, \c REALLOC, and \c FREE, the one path from engine code to the heap. They forward to the
  C runtime; the indirection lets a memory-debugging build route them to an engine allocator that records each live
  block and reports the blocks never freed.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_MEMORY_HPP_
#define INCLUDE_CORE_MEMORY_HPP_

/*!
  \brief Allocates an uninitialized heap block of \a allocSize bytes.

  \param allocSize Size of the block in bytes.

  \return Pointer to the block, aligned for any fundamental type, or \c nullptr when the allocation fails.

  \note A zero \a allocSize yields \c nullptr or a unique pointer, depending on the C runtime; \c FREE accepts either.
  \note Release the block with \c FREE, not \c free, so a tracking build records both the allocation and the release.

  \sa CALLOC, FREE
*/
#define MALLOC(allocSize) std::malloc(allocSize)

/*!
  \brief Allocates a zero-filled heap block for \a count elements of \a allocSize bytes each.

  \param count     Number of elements.
  \param allocSize Size of one element in bytes.

  \return Pointer to the block, aligned for any fundamental type, or \c nullptr when the allocation fails.

  \note Release the block with \c FREE.

  \sa MALLOC, FREE
*/
#define CALLOC(count, allocSize) std::calloc(count, allocSize)

/*!
  \brief Resizes a heap block, moving it when it cannot grow in place.

  \param pointer Block to resize, or \c nullptr to allocate a new one as \c MALLOC does.
  \param newSize Size of the resized block in bytes.

  \return Pointer to the resized block, or \c nullptr when the allocation fails; the original block then stays live.

  \pre \a pointer is \c nullptr or a block from \c MALLOC, \c CALLOC, or \c REALLOC that has not been released since.
  \pre \a newSize is not \c 0; release a block with \c FREE instead.

  \post On success the block keeps its contents up to the smaller of the old and new sizes, bytes past the old size are
        uninitialized, and \a pointer no longer refers to a live block.

  \warning Assigning the result straight back to \a pointer leaks the block when the call fails.

  \sa MALLOC, FREE
*/
#define REALLOC(pointer, newSize) std::realloc(pointer, newSize)

/*!
  \brief Releases a heap block.

  \param pointer Block to release, or \c nullptr for no effect.

  \pre \a pointer is \c nullptr or a block from \c MALLOC, \c CALLOC, or \c REALLOC that has not been released since.

  \post The block is released; \a pointer and every copy of it dangle.

  \sa MALLOC, REALLOC
*/
#define FREE(pointer) std::free(pointer)

#endif // INCLUDE_CORE_MEMORY_HPP_
