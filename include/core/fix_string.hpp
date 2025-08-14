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
/*
  \file   fix_string.hpp
  \brief  Template string of chars, wrapper to char[]
*/

#ifndef INCLUDE_CORE_FIX_STRING_HPP_
#define INCLUDE_CORE_FIX_STRING_HPP_

namespace toygine {

template <std::size_t allocatedSize>
class FixString {
public:
  /*!
    \brief Default constructor for FixString class.

    This is the default constructor for the FixString class. It initializes a FixString object with a size of 0 and a
    null character in the data array.

    \pre The allocated size must be bigger than zero.

    \post The size of the FixString object is set to 0 and the data array contains a null character.
  */
  constexpr inline FixString() noexcept;

  /*!
    \brief Destructor for the FixString class.

    This destructor cleans up the FixString object. Since FixString does not manage dynamic memory, no special cleanup
    is required. The destructor ensures that any necessary invariants or assertions related to the allocatedSize are
    enforced.

    \pre The allocated size must be greater than zero.
  */
  constexpr inline ~FixString() noexcept;

  /*!
    \brief Copy constructor for FixString class.

    This is the copy constructor for the FixString class. It initializes a FixString object by copying the data from
    another FixString object.

    \param string The source FixString object to copy data from.

    \pre The allocated size of both FixString objects must be greater than zero.

    \post A new FixString object is created with the contents of the source FixString object.
  */
  constexpr inline FixString(const FixString<allocatedSize> & string) noexcept;

  /*!
    \brief Constructor for FixString class with different allocated sizes.

    This constructor initializes a FixString object by copying the data from another FixString object with a potentially
    different allocated size.

    \tparam allocatedSize2 The allocated size of the source FixString object.

    \param string The source FixString object to copy data from.

    \pre The allocated size of both FixString objects must be greater than zero.

    \post A new FixString object is created with the contents of the source FixString object.
  */
  template <std::size_t allocatedSize2>
  constexpr inline explicit FixString(const FixString<allocatedSize2> & string) noexcept;

  /*!
    \brief Constructor for FixString class with a C string.

    This constructor initializes a FixString object with a C string.

    \param string The C string to copy data from.

    \pre The allocated size must be bigger than zero.

    \pre The source string must not be null.

    \post A new FixString object is created with the contents of the source string.
  */
  constexpr explicit inline FixString(const char * string) noexcept;

  /*!
    \brief Constructor for FixString class with a character and a count.

    This constructor initializes a FixString object with a character repeated the given number of times.

    \param symbol The character to repeat.
    \param count  The number of times to repeat the character.

    \pre The allocated size must be bigger than zero.

    \post A new FixString object is created with the contents of the character repeated the given number of times.
  */
  constexpr inline explicit FixString(char symbol, std::size_t count = 1) noexcept;
  //    explicit inline FixString(const String & string);

  /*!
    \brief Copy assignment operator for FixString class.

    This operator assigns the contents of another FixString object with the same allocated size to this FixString
    object.

    \param string The source FixString object to copy data from.

    \return A reference to this FixString object after assignment.

    \pre The allocated size of both FixString objects must be greater than zero.
    \pre The source FixString object must be valid.

    \post This FixString object contains the same data as the source FixString object.
  */
  constexpr inline FixString<allocatedSize> & operator=(const FixString<allocatedSize> & string) noexcept;

  /*!
    \brief Template assignment operator for FixString class with different allocated sizes.

    This operator assigns the contents of another FixString object with a potentially different allocated size to this
    FixString object.

    \tparam allocatedSize2 The allocated size of the source FixString object.

    \param string The source FixString object to copy data from.

    \return A reference to this FixString object after assignment.

    \pre The allocated size of both FixString objects must be greater than zero.
    \pre The source FixString object must be valid.
    \pre The source string length must not exceed this object's allocated size.

    \post This FixString object contains the same data as the source FixString object, truncated if necessary.
  */
  template <std::size_t allocatedSize2>
  constexpr inline FixString<allocatedSize> & operator=(const FixString<allocatedSize2> & string) noexcept;

  /*!
    \brief C string assignment operator for FixString class.

    This operator assigns the contents of a C string to this FixString object.

    \param string The C string to copy data from.

    \return A reference to this FixString object after assignment.

    \pre The allocated size must be greater than zero.
    \pre The source string must not be null.
    \pre The source string length must not exceed this object's allocated size.

    \post This FixString object contains the same data as the source C string, truncated if necessary.
  */
  constexpr inline FixString<allocatedSize> & operator=(const char * string) noexcept;

  /*!
    \brief Character assignment operator for FixString class.

    This operator assigns a single character to this FixString object, creating a string of length 1.

    \param symbol The character to assign.

    \return A reference to this FixString object after assignment.

    \pre The allocated size must be greater than zero.

    \post This FixString object contains only the specified character with a size of 1.
  */
  constexpr inline FixString<allocatedSize> & operator=(char symbol) noexcept;

  /*!
    \brief Copy assign method for FixString class.

    This method assigns the contents of another FixString object with the same allocated size to this FixString object.

    \param string The source FixString object to copy data from.

    \return A reference to this FixString object after assignment.

    \pre The allocated size of both FixString objects must be greater than zero.
    \pre The source FixString object must be valid.

    \post This FixString object contains the same data as the source FixString object.
  */
  constexpr inline FixString<allocatedSize> & assign(const FixString<allocatedSize> & string) noexcept;

  /*!
    \brief Template assign method for FixString class with different allocated sizes.

    This method assigns the contents of another FixString object with a potentially different allocated size to this
    FixString object.

    \tparam allocatedSize2 The allocated size of the source FixString object.

    \param string The source FixString object to copy data from.

    \return A reference to this FixString object after assignment.

    \pre The allocated size of both FixString objects must be greater than zero.
    \pre The source FixString object must be valid.
    \pre The source string length must not exceed this object's allocated size.

    \post This FixString object contains the same data as the source FixString object, truncated if necessary.
  */
  template <std::size_t allocatedSize2>
  constexpr inline FixString<allocatedSize> & assign(const FixString<allocatedSize2> & string) noexcept;

  /*!
    \brief C string assign method for FixString class.

    This method assigns the contents of a C string to this FixString object.

    \param string The C string to copy data from.

    \return A reference to this FixString object after assignment.

    \pre The allocated size must be greater than zero.
    \pre The source string must not be null.
    \pre The source string length must not exceed this object's allocated size.

    \post This FixString object contains the same data as the source C string, truncated if necessary.
  */
  constexpr inline FixString<allocatedSize> & assign(const char * string) noexcept;

  /*!
    \brief Character fill assign method for FixString class.

    This method assigns a character repeated the specified number of times to this FixString object.

    \param symbol The character to repeat.
    \param count The number of times to repeat the character.

    \return A reference to this FixString object after assignment.

    \pre The allocated size must be greater than zero.
    \pre The count must not exceed this object's allocated size.

    \post This FixString object contains the specified character repeated count times.
  */
  constexpr inline FixString<allocatedSize> & assign(char symbol, std::size_t count) noexcept;
  //  constexpr inline FixString<allocatedSize> & assign(const String & string) noexcept;

  /*!
    \brief Access a character in the string at a given offset.

    \param offset The offset of the character to access in the string.

    \return A reference to the character at the specified offset.

    \pre The offset must be within the bounds of the string.

    \post The character at the specified offset is returned.
  */
  constexpr inline char & at(std::size_t offset) noexcept;

  /*!
    \brief Access a character in the string at a given offset.

    \param offset The offset of the character to access in the string.

    \return A const reference to the character at the specified offset.

    \pre The offset must be within the bounds of the string.

    \post The character at the specified offset is returned.
  */
  constexpr inline const char & at(std::size_t offset) const noexcept;

  /*!
    \brief Access a character in the string at a given offset.

    \param offset The offset of the character to access in the string.

    \return A reference to the character at the specified offset.

    \pre The offset must be within the bounds of the string.

    \post The character at the specified offset is returned.
  */
  constexpr inline char & operator[](std::size_t offset) noexcept;

  /*!
    \brief Access a character in the string at a given offset.

    \param offset The offset of the character to access in the string.

    \return A const reference to the character at the specified offset.

    \pre The offset must be within the bounds of the string.

    \post The character at the specified offset is returned.
  */
  constexpr inline const char & operator[](std::size_t offset) const noexcept;

  /*!
    \brief Return a pointer to the data of the string.

    \return A pointer to the data of the string.
  */
  constexpr inline char * data() noexcept;

  /*!
    \brief Return a pointer to the data of the string.

    \return A pointer to the data of the string.
  */
  constexpr inline const char * data() const noexcept;

  /*!
    \brief Return a pointer to the C string representation of this FixString object.

    This function returns a pointer to the C string representation of this FixString object. The returned pointer is
    valid until the FixString object is modified or destroyed. The returned string is null terminated and can be used
    with C style string functions.

    \return A pointer to the C string representation of this FixString object.
  */
  constexpr inline const char * c_str() const noexcept;

  /*!
    \brief Check if the FixString object is empty.

    This function checks if the FixString object is empty, i.e. its size is zero.

    \return True if the FixString object is empty, false otherwise.
  */
  constexpr inline bool empty() const noexcept;

  /*!
    \brief Returns the size of the string.

    \return The length of the string in characters, excluding the terminating null character.
  */
  constexpr inline std::size_t size() const noexcept;

  /*!
    \brief Returns the size of the string in UTF-8 encoded characters.

    \return The number of Unicode characters in the string, excluding the terminating null character.
  */
  constexpr inline std::size_t utf8Size() const noexcept;

  /*!
    \brief Returns the size of the string.

    \return The length of the string in characters, excluding the terminating null character.
  */
  constexpr inline std::size_t length() const noexcept;

  /*!
    \brief Returns the maximum size of the string.

    \return The maximum size of the string in characters, excluding the terminating null character.
  */
  constexpr inline std::size_t max_size() const noexcept;

  /*!
    \brief Returns the capacity of the string.

    \return The maximum size of the string in characters, excluding the terminating null character.
  */
  constexpr inline std::size_t capacity() const noexcept;

  /*!
    \brief Clears the contents of the FixString object.

    This function resets the size of the string object to zero and effectively clear the string.

   \post The FixString object is cleared, and its size is set to zero.
  */
  constexpr inline void clear() noexcept;

  constexpr inline FixString<allocatedSize> & operator+=(const FixString<allocatedSize> & string) noexcept;
  template <std::size_t allocatedSize2>
  constexpr inline FixString<allocatedSize> & operator+=(const FixString<allocatedSize2> & string) noexcept;
  constexpr inline FixString<allocatedSize> & operator+=(const char * string) noexcept;
  constexpr inline FixString<allocatedSize> & operator+=(char symbol) noexcept;

  /*!
    \brief Concatenates two FixString objects and returns the result.

    This operator creates a new FixString object by concatenating the current FixString object with another provided
    FixString object. The resulting FixString will contain the contents of both strings, and its size will be the sum of
    the sizes of the two operands.

    \param string The FixString object to concatenate with the current FixString object.

    \return A new FixString object representing the concatenation of the current FixString and the provided FixString
            object.

    \pre The sum of the sizes of the two FixString objects must be less than the allocated size.
    \pre The allocated size of both FixString objects must be greater than zero.

    \post A new FixString object is created with the contents of both FixString objects.
  */
  constexpr inline FixString<allocatedSize> operator+(const FixString<allocatedSize> & string) const noexcept;

  /*!
    \brief Concatenates two FixString objects and returns the result.

    This operator creates a new FixString object by concatenating the current FixString object with another provided
    FixString object. The resulting FixString will contain the contents of both strings, and its size will be the sum of
    the sizes of the two operands.

    \param string The FixString object to concatenate with the current FixString object.

    \return A new FixString object representing the concatenation of the current FixString and the provided FixString
            object.

    \pre The sum of the sizes of the two FixString objects must be less than the allocated size.
    \pre The allocated size of both FixString objects must be greater than zero.

    \post A new FixString object is created with the contents of both FixString objects.
  */
  template <std::size_t allocatedSize2>
  constexpr inline FixString<allocatedSize> operator+(const FixString<allocatedSize2> & string) const noexcept;

  /*!
    \brief Concatenates two strings and returns the result.

    This operator creates a new FixString object by concatenating the current FixString object with a C string. The
    resulting FixString will contain the contents of both strings, and its size will be the sum of the sizes of the two
    operands.

    \param string The C string to concatenate with the current FixString object.

    \return A new FixString object representing the concatenation of the current FixString and the provided C string.

    \pre The sum of the sizes of the two strings must be less than the allocated size.
    \pre The allocated size must be greater than zero.

    \post A new FixString object is created with the contents of both strings.
  */
  constexpr inline FixString<allocatedSize> operator+(const char * string) const noexcept;

  /*!
    \brief Concatenates this FixString object with a character.

    This operator adds a character to the end of the FixString object and returns the new FixString object.

    \param symbol The character to add to the end of the FixString object.

    \pre The allocated size of the FixString object must be greater than zero.
    \pre The size of the FixString object plus one must be less than the allocated size.

    \post A new FixString object is created with the contents of the original FixString object plus the given character.
  */
  constexpr inline FixString<allocatedSize> operator+(char symbol) const noexcept;

  static const size_t npos = SIZE_MAX;

private:
  static_assert(allocatedSize > 0, "FixString capacity must be greater than zero.");

  char _data[allocatedSize];
  std::size_t _size;
};

} // namespace toygine

#endif // INCLUDE_CORE_FIX_STRING_HPP_
