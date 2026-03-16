# Doxygen Documentation Style Guide

Style guide for Doxygen documentation in the ToyGine2 project.

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

### Documentation Tone

All documentation must be:

- **Concise** — no filler words or redundant phrasing.
- **Neutral** — factual descriptions, no subjective claims.
- **Technical** — precise, uses correct terminology.
- **No marketing language** — avoid "lightweight", "blazing-fast", "powerful", "cutting-edge", etc.

Each item in the `\section features Key Features` list uses **bold emphasis** for the feature name.

### Comment Blocks

- Use `/*! ... */` for multi-line documentation blocks.
- Use `///` for single-line comments (typically for member variables).
- Use `//!<` for inline documentation after a declaration (type aliases, simple members).
- Always start with `/*!` on a new line.
- Align closing `*/` with the opening `/*!`.

```cpp
/*!
  \brief Short description.

  Detailed description here.
*/
```

### Indentation

- Use 2 spaces for indentation in documentation blocks.
- Align Doxygen commands (`\param`, `\return`, etc.) to the same column.
- Maintain consistent spacing between sections.

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

- Use `\file` with the actual filename.
- `\brief` is a concise one-line description.
- Place before include guards.

---

## Class/Struct Documentation

### Class Documentation Structure

Class documentation follows this order:

1. `\class` or `\struct`
2. `\brief` — one-line description
3. Detailed description (2-4 sentences)
4. `\tparam` — template parameters (if applicable)
5. `\section features Key Features` — bullet list with **bold** feature names
6. `\section usage Usage Example` — code block
7. `\section performance Performance Characteristics` — complexity and memory (plain text)
8. `\section safety Safety Guarantees` — safety information (plain text)
9. `\section compatibility Compatibility` — platform/standard requirements (plain text, optional)
10. `\note` — notes
11. `\warning` — warnings (if needed)
12. `\sa` — related classes/types

### Class Documentation Example

```cpp
/*!
  \class FixedString
  \brief Template string class with a fixed-size character buffer.

  FixedString stores characters in a stack-allocated buffer of size \a allocatedSize.
  It provides a subset of the std::string interface without dynamic memory allocation.

  \tparam allocatedSize The size of the internal character buffer, including space for the null terminator.
                        Must be greater than zero.

  \section features Key Features

  - **No dynamic allocation**: All memory is on the stack
  - **Constexpr support**: Most operations are constexpr
  - **Exception safety**: All operations are noexcept
  - **STL-compatible interface**: Supports a subset of the std::string API

  \section usage Usage Example

  \code
  #include "fixed_string.hpp"
  toy::FixedString<32> str("Hello, World!");
  \endcode

  \section performance Performance Characteristics

  - **Construction**: O(n) where n is the source string length
  - **Memory usage**: Fixed at compile time, no heap allocation

  \section safety Safety Guarantees

  - **Contracts**: Bounds and capacity are asserted in debug builds
  - **Exception safety**: All operations are noexcept

  \section compatibility Compatibility

  - **C++ standard**: Requires C++20 or later
  - **Cross-platform**: Works on all platforms supported by the compiler

  \note The maximum string length is allocatedSize - 1 (null terminator takes one byte).

  \sa std::string
  \sa StringLike
*/
```

---

## Method/Function Documentation

### Method Documentation Structure

Method documentation follows this order:

1. `\brief` — one-line description
2. Detailed description (1-2 sentences, if needed)
3. `\tparam` — template parameters (if applicable)
4. `\param` — parameters (aligned)
5. `\return` — return value
6. `\pre` — preconditions
7. `\post` — postconditions (for state-changing methods)
8. `\note` — additional notes
9. `\warning` — warnings (if needed)
10. `\sa` — related methods/operators

### Method Documentation Example

```cpp
/*!
  \brief Appends contents of a StringLike object.

  \tparam stringType Source string type. Must satisfy the StringLike concept.

  \param string The source object to append from.

  \return Reference to this string after appending.

  \pre The combined length after appending must not exceed allocated size.

  \post The string size is increased by the size of \a string.

  \sa operator+=(const stringType &)
*/
```

---

## Parameter Documentation

### Parameter Format

- Start with `\param`.
- Use `\a` to reference parameter names in other descriptions.
- Align descriptions when multiple parameters exist.
- Mention default values when applicable.

### Alignment

```cpp
\param dest     Pointer to the destination buffer.
\param destSize Size of the destination buffer in bytes.
\param src      Pointer to the source data.
```

### Default Values

```cpp
\param count    Number of repetitions (default: \c 1).
\param position Position to start searching from (default: \c 0).
```

### Constraints Belong in \pre, Not \param

`\param` describes what the parameter represents. Constraints go in `\pre`.

**Correct:**

```cpp
\param string Source C string.

\pre \a string must not be null.
\pre \a string must be null-terminated.
```

**Incorrect:**

```cpp
\param string Source C string (must not be null, must be null-terminated).
```

---

## Return Value Documentation

### Return Value Format

- Start with `\return`.
- Describe what is returned.
- Use `\c` for constants and literal values.
- Mention special return values (e.g., `\ref npos`).

### Examples

```cpp
\return Reference to this string after appending.

\return \c true if the string is empty (size is \c 0), \c false otherwise.

\return Position of the first occurrence, or \ref npos if not found.
```

---

## Preconditions and Postconditions

### Preconditions (\pre)

Document all requirements that must be met before calling the function:

```cpp
\pre The \a index must be within the bounds of the current string.
\pre The combined length after appending must not exceed allocated size.
\pre \a string must not be null.
```

### Postconditions (\post)

Required for methods that change object state:

```cpp
\post This string contains the same content as the source.
\post The string size equals the size of the source.

\post The vector is empty (size is \c 0).
\post All elements are destroyed.
```

---

## Notes and Warnings

### Notes (\note)

- One line per `\note` for simple notes.
- Multiple `\note` tags for different aspects.

```cpp
\note Self-assignment is handled correctly.
\note The search is case-sensitive.
```

```cpp
\note For ASCII strings, utf8_size() equals size().
\note For UTF-8 strings, utf8_size() may be less than size().
```

If a note is too long, split it into shorter `\note` tags.

### Warnings (\warning)

Use for critical information about misuse or potential issues:

```cpp
\warning The underlying C string must remain valid for the lifetime of the CStringView object.
         CStringView does not own the string data.
```

---

## Cross-References

### \sa Tags

Link between:

- Methods and their equivalent operators (`append()` and `operator+=()`)
- Const and non-const overloads
- Method synonyms (`size()` and `length()`)
- Related classes (`Point` and `Vector2`)
- Similar methods (`find()` and `rfind()`)

### Cross-Reference Format

```cpp
\sa operator+=(const stringType &)
\sa size()
\sa Point
```

---

## Type Requirements

### Template Parameters (\tparam)

Always document template parameters with constraints and requirements:

```cpp
\tparam allocatedSize Size of the internal buffer, including the null terminator.
                      Must be greater than zero.

\tparam stringType Source string type. Must satisfy the StringLike concept.
```

---

## Constants and Values

### Using \c

Wrap constants, literal values, and numeric values with `\c`:

```cpp
\param count Number of repetitions (default: \c 1).
\return \c true if empty, \c false otherwise.
\post The string size is increased by \c 1.
```

### Using \a

Reference parameter names in descriptions with `\a`:

```cpp
This method appends the contents of \a string to the end of this string.
```

### Using \ref

Cross-reference documented entities:

```cpp
\return Position of the first occurrence, or \ref npos if not found.
```

---

## Code Examples

Use `\section usage Usage Example` with `\code ... \endcode`:

```cpp
\section usage Usage Example

\code
#include "fixed_string.hpp"

toy::FixedString<32> str("Hello, World!");
str += " Test.";
\endcode
```

---

## Sections

### Standard Sections for Classes

1. `\section features Key Features` — **bold** feature names
2. `\section usage Usage Example` — code example
3. `\section performance Performance Characteristics` — Big-O complexity, plain text
4. `\section safety Safety Guarantees` — guarantees, plain text
5. `\section compatibility Compatibility` — platform/standard, plain text (optional)

### Section Format

```cpp
\section features Key Features

- **Feature name**: Description
- **Feature name**: Description

\section performance Performance Characteristics

- **Operation**: O(1) constant time
- **Memory usage**: Description

\section safety Safety Guarantees

- **Contracts**: Description
- **Exception safety**: All operations are noexcept

\section compatibility Compatibility

- **C++ standard**: C++20 or later
- **Cross-platform**: Description
```

---

## Checklist

Before committing, verify:

- [ ] File has `\file` documentation block
- [ ] Class/struct has `\class`/`\struct` and `\brief`
- [ ] All methods have `\brief`
- [ ] All parameters have `\param` with proper alignment
- [ ] Default values mentioned in `\param`
- [ ] All constraints in `\pre`, not in `\param`
- [ ] All state-changing methods have `\post`
- [ ] Constants wrapped with `\c`
- [ ] Parameter names referenced with `\a`
- [ ] `\return` documented for all non-void functions
- [ ] `\tparam` documented for all template parameters
- [ ] `\note` tags are one line each (or split)
- [ ] Related methods/classes have `\sa` tags
- [ ] No emoji in documentation
- [ ] No marketing language

---

## Style Consistency

### Method Documentation Order

```text
\brief
[detailed description]
\tparam (if template)
\param (aligned, with default values)
\return
\pre
\post (for state-changing methods)
\note
\warning (if needed)
\sa
```

### Class Documentation Order

```text
\class or \struct
\brief
[detailed description]
\tparam (if template)
\section features
\section usage
\section performance
\section safety
\section compatibility (optional)
\note
\warning (if needed)
\sa
```
