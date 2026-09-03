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

#include <stdint.h>
#include <string.h>

int memcmp(const void* const lhs, const void* const rhs, const size_t count)
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
	size_t length;

	for (length = 0; str[length] != '\0'; ++length)
		;

	return length;
}

char* strncpy(char* const dest, const char* const src, const size_t count)
{
	size_t index;

	// Copy the source, stopping at its terminator.
	for (index = 0; index != count && src[index] != '\0'; ++index)
		dest[index] = src[index];

	// Pad the remainder of the destination with terminators.
	for (; index != count; ++index)
		dest[index] = '\0';

	return dest;
}
