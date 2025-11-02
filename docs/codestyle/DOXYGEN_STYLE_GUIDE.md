# Doxygen Documentation Style Guide

This document describes the comprehensive style guide for Doxygen documentation used throughout the project.

## Table of Contents

1. [General Formatting](#general-formatting)
2. [File Documentation](#file-documentation)
3. [Class/Struct Documentation](#classstruct-documentation)
4. [Method/Function Documentation](#methodfunction-documentation)
5. [Parameter Documentation](#parameter-documentation)
6. [Return Value Documentation](#return-value-documentation)
7. [Preconditions and Postconditions](#preconditions-and-postconditions)
8. [Notes and Warnings](#notes-and-warnings)
9. [Cross-References](#cross-references)
10. [Type Requirements](#type-requirements)
11. [Constants and Values](#constants-and-values)
12. [Code Examples](#code-examples)
13. [Sections](#sections)

---

## General Formatting

### Comment Blocks

- Use `/*! ... */` for multi-line documentation blocks
- Use `///` for single-line comments (typically for member variables)
- Always start with `/*!` on a new line
- Align closing `*/` with the opening `/*!`

```cpp
/*!
  \brief Short description.

  Detailed description here.
*/
```

### Indentation

- Use 2 spaces for indentation in documentation blocks
- Align Doxygen commands (e.g., `\param`, `\return`) to the same column
- Maintain consistent spacing between sections

---

## File Documentation

Every header file must start with a file documentation block:

```cpp
/*!
  \file   filename.hpp
  \brief  Brief description of the file's purpose.
*/
```

**Rules:**

- Use `\file` with the actual filename (relative to include directory)
- `\brief` should be a concise one-line description
- Place before include guards

---

## Class/Struct Documentation

### Structure

Class documentation should follow this structure:

1. `\class` or `\struct`
2. `\brief` - one-line description
3. Detailed description (2-4 paragraphs)
4. `\tparam` - template parameters (if applicable)
5. `\section features Key Features` - bullet list with emoji prefixes
6. `\section usage Usage Example` - code block
7. `\section performance Performance Characteristics` - complexity and memory info
8. `\section safety Safety Guarantees` - safety-related information
9. `\section compatibility Compatibility` - platform/standard requirements
10. `\note` - important notes (one line each for simple notes, multiple for complex cases)
11. `\warning` - warnings (if needed)
12. `\see` - related classes/types

### Class/Struct Example

```cpp
/*!
  \class FixedString
  \brief Template string class with fixed-size character buffer.

  FixedString is a lightweight, high-performance string class that uses a fixed-size character buffer allocated on the
  stack. It provides a std::string-like interface while avoiding dynamic memory allocation, making it suitable for
  embedded systems, real-time applications, and performance-critical code where memory allocation overhead must be
  minimized.

  \tparam allocatedSize The size of the internal character buffer, including space for the null terminator.
                        Must be greater than zero.

  \section features Key Features

  - ⚙️ **Zero Dynamic Allocation**: All memory is allocated on the stack at compile time
  - 🔧 **constexpr Support**: Most operations can be evaluated at compile time
  - 🛡️ **Exception Safety**: All operations are noexcept

  \section usage Usage Example

  \code
  #include "fixed_string.hpp"
  toy::FixedString<32> str("Hello, World!");
  \endcode

  \section performance Performance Characteristics

  - ⚙️ **Construction**: O(n) where n is the length of the source string
  - 💾 **Memory Usage**: Fixed at compile time, no heap allocation

  \section safety Safety Guarantees

  - 🛡️ **Contracts & Debug Checks**: Bounds/capacity are asserted in debug
  - ⚠️ **Exception Safety**: All operations are noexcept, no exceptions thrown

  \section compatibility Compatibility

  - 🆕 **C++23**: Requires C++23 or later for full functionality
  - 🌐 **Cross-Platform**: Works on all platforms supported by the compiler

  \note The internal buffer size is allocatedSize, but the maximum string length is allocatedSize - 1 (null terminator).

  \see std::string
  \see StringLike
*/
```

### Emoji Usage in Features

Use appropriate emojis for feature categories:

- ⚙️ - Memory/Resource management
- 🔧 - Language features (constexpr, noexcept, etc.)
- 🛡️ - Safety/Exception guarantees
- 🔗 - Compatibility/Integration
- 🌍 - Internationalization/Standards
- 🧬 - Type safety/Concepts
- 📏 - Size/Layout
- 🎯 - Purpose/Optimization

---

## Method/Function Documentation

### Structure

Method documentation should follow this order:

1. `\brief` - one-line description
2. Detailed description (1-2 paragraphs)
3. `\tparam` - template parameters (if applicable)
4. `\param` - parameters (aligned)
5. `\return` - return value
6. `\pre` - preconditions
7. `\post` - postconditions (for state-changing methods)
8. `\note` - additional notes
9. `\see` - related methods/operators
10. `\warning` - warnings (if needed)

### Method/Function Example

```cpp
/*!
  \brief Appends a StringLike object to the end of this string.

  This method appends the contents of a StringLike object to the end of this string. The append operation
  increases the string size accordingly.

  \tparam stringType The type of the source string. Must satisfy the StringLike concept.

  \param string The source StringLike object to append content from.

  \return A reference to this string after appending.

  \pre The combined length after appending must not exceed the allocated size.

  \post The StringLike object is appended to the end of this string.
  \post The string size is increased by the size of the appended StringLike object.

  \note This method is equivalent to the addition assignment operator.

  \see operator+=(const stringType &)
*/
```

---

## Parameter Documentation

### Parameter Format

- Start with `\param`
- Use `\a` to reference the parameter name in descriptions
- Align parameter descriptions to the longest parameter name
- Include default values when applicable
- Keep descriptions concise (one line when possible)

### Alignment

When multiple parameters exist, align descriptions:

```cpp
\param dest     A pointer to the destination buffer where data will be stored.
\param destSize The size of the destination buffer in bytes.
\param src      A pointer to the source data to copy from.
```

### Default Values

Always mention default values in parameter descriptions:

```cpp
\param count     The number of times to repeat the \a character (default: \c 1).
\param position The position to start searching from (default: \c 0).
\param tolerance The tolerance for equality comparison (default: \c 0).
```

### Restrictions

- **Do NOT** include conditions or requirements in `\param` descriptions
- Move all conditions to `\pre` tags
- Keep `\param` focused on what the parameter represents, not its constraints

**Correct:**

```cpp
\param string The source C string to copy content from.

\pre The source C \a string must not be null.
\pre The source C \a string must be null-terminated.
```

**Incorrect:**

```cpp
\param string The source C string to copy content from (must not be null, must be null-terminated).
```

---

## Return Value Documentation

### Return Value Format

- Start with `\return`
- Describe what is returned
- Use `\c` for constants and literal values
- Mention special return values (e.g., `\ref npos`, `nullptr`)

### Example

```cpp
\return A reference to this string after appending.

\return \c true if the string is empty (size is \c 0), \c false otherwise.

\return The position of the first occurrence of \a string, or \ref npos if not found.

\return A pointer to the destination buffer, or \c nullptr if the operation failed.
```

---

## Preconditions and Postconditions

### Preconditions (\pre)

- Document all requirements that must be met before calling the function
- Include parameter validity checks
- Include size/bounds checks
- Include type requirements (when not obvious from signature)

### Preconditions/Postconditions Format

```cpp
\pre The \a index must be within the bounds of the current string.
\pre The combined length after appending must not exceed the allocated size.
\pre The source C \a string must not be null.
\pre The source C \a string must be null-terminated.
\pre The \a first and \a last iterators must be valid.
```

### Postconditions (\post)

- Required for all methods that change object state
- Describe the state of the object after the method completes
- Include size/state changes
- Multiple `\post` tags for multiple state changes

### Format

```cpp
\post This string contains the same content as the source StringLike object.
\post The string size equals the size of the source StringLike object.

\post The vector is empty (size is \c 0).
\post All elements are properly destroyed.
```

---

## Notes and Warnings

### Notes (\note)

- Use for additional information, clarifications, or implementation details
- **Simple notes**: One line per `\note`
- **Complex notes**: Multiple `\note` tags for different aspects
- Keep notes concise; long explanations should be split across multiple notes

**Simple:**

```cpp
\note Self-assignment is handled correctly and safely.
\note The search is case-sensitive.
```

**Complex:**

```cpp
\note For ASCII strings, utf8_size() equals size().
\note For UTF-8 strings, utf8_size() may be less than size().
\note Invalid UTF-8 sequences are handled gracefully and may affect the count.
\note This method is useful for internationalization and text processing.
```

- If a note is too long for one line, either:
  1. Split into two shorter `\note` tags, or
  2. Shorten the wording

### Warnings (\warning)

- Use for critical information about misuse or potential issues
- Typically multi-line

```cpp
\warning The underlying C string must remain valid for the lifetime of the CStringView object.
         CStringView does not own the string data and will not manage its lifetime.
```

---

## Cross-References

### \see Tags

- Add `\see` tags for all related methods, classes, and types
- Link between:
  - Methods and their equivalent operators (`append()` ↔ `operator+=()`)
  - Const and non-const overloads (`data()` ↔ `data() const`)
  - Method synonyms (`size()` ↔ `length()`, `data()` ↔ `c_str()`)
  - Related classes (`Point` ↔ `Vector2D`, `FixedString` ↔ `CStringView`)
  - Similar methods (`find()` ↔ `rfind()`, `find_first_of()` ↔ `find_first_not_of()`)

### Format

```cpp
\see operator+=(const stringType &)
\see size()
\see data() const
\see Point
\see find()
```

---

## Type Requirements

### Template Parameters (\tparam)

- Always document template parameters
- Include type constraints and requirements
- Use clear, descriptive names

### Format

```cpp
\tparam allocatedSize The size of the internal character buffer, including space for the null terminator.
                      Must be greater than zero.

\tparam stringType The type of the source string. Must satisfy the StringLike concept.

\tparam type          The type of elements stored in the vector. Must be a copy-constructible and destructible type.
\tparam allocatedSize The maximum number of elements that can be stored in the vector. Must be greater than zero.

\tparam InputIterator The type of the input iterator. Must satisfy the InputIterator concept and must not be an integral type.
```

---

## Constants and Values

### Using \c

- Wrap all constants, literal values, and numeric values with `\c`
- Use in:
  - `\param` descriptions (default values)
  - `\return` descriptions
  - `\post` descriptions
  - `\note` descriptions

### Examples

```cpp
\param count The number of times to repeat the \a character (default: \c 1).
\return \c true if the string is empty (size is \c 0), \c false otherwise.
\post The string size is increased by \c 1.
\note If \a count is \c npos, all characters from \a offset to the end are removed.
```

### Using \a

- Use `\a` to reference parameter names in descriptions
- Helps create readable, linked documentation

```cpp
\param string The source StringLike object to append content from.

This method appends the contents of the \a string to the end of this string.
```

### Using \ref

- Use `\ref` for cross-references to other documented entities
- Use for special constants (e.g., `\ref npos`)

```cpp
\return The position of the first occurrence, or \ref npos if not found.
\see operator+=(const stringType &)
```

---

## Code Examples

### Usage Examples in Classes

Use `\section usage Usage Example` with `\code ... \endcode`:

```cpp
\section usage Usage Example

\code
#include "fixed_string.hpp"

toy::FixedString<32> str("Hello, World!");
str += " This is a test.";
\endcode
```

### Inline Code References

Use backticks for inline code in notes:

```cpp
\note This method is equivalent to the expression: `size() == \c 0`.
```

---

## Sections

### Standard Sections

Classes should include these standard sections:

1. **\section features Key Features** - List of main features with emoji prefixes
2. **\section usage Usage Example** - Code example showing typical usage
3. **\section performance Performance Characteristics** - Complexity and memory information
4. **\section safety Safety Guarantees** - Safety-related guarantees
5. **\section compatibility Compatibility** - Platform/standard requirements

### Section Format

```cpp
\section features Key Features

- ⚙️ **Feature Name**: Description
- 🔧 **Feature Name**: Description

\section usage Usage Example

\code
// Code example here
\endcode

\section performance Performance Characteristics

- ⚙️ **Operation**: Complexity or description
- 💾 **Memory Usage**: Description

\section safety Safety Guarantees

- 🛡️ **Category**: Description
- ⚠️ **Category**: Description

\section compatibility Compatibility

- 🆕 **Platform/Standard**: Description
- 🌐 **Platform**: Description
```

---

## Checklist for Documentation

Before committing, ensure:

- [ ] File has `\file` documentation block
- [ ] Class/struct has `\class`/`\struct` and `\brief`
- [ ] All methods have `\brief`
- [ ] All parameters have `\param` with proper alignment
- [ ] Default values are mentioned in `\param`
- [ ] All conditions are in `\pre`, not in `\param`
- [ ] All state-changing methods have `\post`
- [ ] All constants are wrapped with `\c`
- [ ] Parameter names are referenced with `\a`
- [ ] `\return` is documented for all non-void functions
- [ ] `\tparam` is documented for all template parameters
- [ ] `\note` tags are one line each (or properly split)
- [ ] Related methods/classes have `\see` tags
- [ ] Template parameters have type requirements documented

---

## Style Consistency Rules

### Method Documentation Order

Always follow this order:

```text
\brief
[detailed description]
\tparam (if template)
\param (aligned, with default values)
\return
\pre
\post (for state-changing methods)
\note
\see
\warning (if needed)
```

### Class Documentation Order

Always follow this order:

```text
\class or \struct
\brief
[detailed description]
\tparam (if template)
\section features
\section usage
\section performance
\section safety
\section compatibility
\note
\warning (if needed)
\see
```

### Consistency Checks

- Use consistent terminology throughout
- Use consistent formatting for similar constructs
- Maintain consistent level of detail
- Use consistent abbreviations (e.g., "C string" vs "C-string")

---

This style guide should be followed for all new documentation and used as a reference when updating existing documentation.
