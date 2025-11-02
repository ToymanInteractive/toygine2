# Code Style Guide

This document describes the coding style conventions used throughout the ToyGine2 project. All code should adhere to these guidelines to ensure consistency, readability, and maintainability.

## Table of Contents

1. [C++ Standard](#c-standard)
2. [Formatting](#formatting)
3. [Naming Conventions](#naming-conventions)
4. [Code Organization](#code-organization)
5. [Modern C++ Features](#modern-c-features)
6. [Exception Safety](#exception-safety)
7. [Documentation](#documentation)
8. [Includes and Headers](#includes-and-headers)
9. [Best Practices](#best-practices)

---

## C++ Standard

- **Minimum**: C++23 or later
- Use modern C++ features where appropriate
- Prefer standard library facilities over custom implementations
- Use C++20 concepts for template constraints

```cpp
template <StringLike stringType>
constexpr FixedString(const stringType & string) noexcept;

template <typename InputIterator>
  requires(!std::is_integral_v<InputIterator>)
FixedVector(InputIterator first, InputIterator last);
```

---

## Formatting

### Indentation and Spacing

- Use **2 spaces** for indentation (no tabs)
- Maximum line width: **120 characters**
- Use spaces, not tabs
- Trailing whitespace is not allowed

### Braces

- Use **attached brace style** (opening brace on the same line)
- Place closing brace on its own line, aligned with the opening statement

```cpp
// Good
class FixedString {
public:
  constexpr FixedString() noexcept;
};

// Bad
class FixedString
{
public:
  constexpr FixedString() noexcept
  {
  }
};
```

### Pointer and Reference Alignment

- Use **middle alignment** for pointers and references

```cpp
// Good
type * pointer;
type & reference;
const type * const_pointer;

// Bad
type* pointer;
type &reference;
```

### Binary Operators

- Break **before** binary operators when line exceeds column limit

```cpp
// Good
result = value1
  + value2
  + value3;

// Bad
result = value1 +
  value2 +
  value3;
```

### Empty Lines

- Maximum **1 empty line** between code sections
- No empty lines at the start of blocks

---

## Naming Conventions

### Classes and Structs

- Use **PascalCase** for class and struct names

```cpp
class FixedString;
class Vector2D;
struct Version;
```

### Template Parameters

- Use **camelCase** for template parameter names

```cpp
template <std::size_t allocatedSize>
template <typename type>
template <StringLike stringType>
```

### Member Variables

- Use **underscore prefix** for private member variables

```cpp
class FixedString {
private:
  char _data[allocatedSize];
  std::size_t _size;
};
```

### Functions and Methods

- Use **camelCase** for function and method names
- Follow STL naming conventions where applicable (e.g., `push_back`, `find_first_of`)

```cpp
constexpr void push_back(char character) noexcept;
constexpr std::size_t find_first_of(char character, std::size_t position = 0) const noexcept;
```

### Type Aliases

- Use **snake_case** with `_type` suffix for type aliases

```cpp
using value_type = type;
using size_type = std::size_t;
using difference_type = std::ptrdiff_t;
using reference = value_type &;
using const_reference = const value_type &;
```

### Constants and Enums

- Use **UPPER_SNAKE_CASE** for constants and enum values

```cpp
static constexpr std::size_t npos = std::size_t(-1);
enum class Color { RED, GREEN, BLUE };
```

### Namespaces

- Use **lowercase** for namespace names
- Use nested namespaces for logical organization

```cpp
namespace toy {
  namespace math {
    class Vector2D;
  }
  namespace app {
    struct Version;
  }
}
```

---

## Code Organization

### Include Guards

- Use **uppercase** include guards with full path

```cpp
#ifndef INCLUDE_CORE_FIXED_STRING_HPP_
#define INCLUDE_CORE_FIXED_STRING_HPP_
// ... code ...
#endif  // INCLUDE_CORE_FIXED_STRING_HPP_
```

### File Structure

- Header files: `.hpp`
- Implementation files: `.cpp` or `.inl` (for template implementations)
- Organize includes in the following order:
  1. Corresponding header (for `.cpp` files)
  2. Standard library headers (`<...>`)
  3. Third-party headers
  4. Project headers (`"..."`)

### Access Modifiers

- Use access modifier offset of **-2** (consistent with clang-format)
- Order: `public`, `protected`, `private`

```cpp
class FixedString {
public:
  // Public interface

private:
  // Private implementation
};
```

---

## Modern C++ Features

### constexpr

- Use `constexpr` extensively for compile-time evaluation
- Prefer `constexpr` constructors and methods when possible

```cpp
constexpr FixedString() noexcept;
constexpr std::size_t size() const noexcept;
constexpr bool empty() const noexcept;
```

### noexcept

- Mark all functions that don't throw exceptions with `noexcept`
- This project uses no exceptions - all operations are `noexcept`

```cpp
constexpr FixedString(const FixedString & string) noexcept;
constexpr FixedString & operator=(const FixedString & string) noexcept;
```

### Attributes

- Use `[[nodiscard]]` for functions whose return values should not be ignored

```cpp
[[nodiscard]] constexpr std::size_t size() const noexcept;
[[nodiscard]] constexpr bool empty() const noexcept;
[[nodiscard]] constexpr const char * data() const noexcept;
```

### Concepts (C++20)

- Use concepts to constrain template parameters
- Define reusable concepts for common patterns

```cpp
template <typename T>
concept StringLike = requires(T t) {
  { t.size() } -> std::convertible_to<std::size_t>;
  { t.data() } -> std::convertible_to<const char *>;
};
```

### Type Aliases

- Prefer `using` declarations over `typedef`

```cpp
// Good
using size_type = std::size_t;
using value_type = type;

// Avoid
typedef std::size_t size_type;
```

---

## Exception Safety

- **No exceptions**: All operations are `noexcept`
- Use assertions for precondition checks in debug builds
- Violating preconditions in release builds results in undefined behavior
- Document preconditions with `\pre` tags in Doxygen comments

```cpp
constexpr char & at(std::size_t offset) noexcept {
  assert(offset < _size);
  return _data[offset];
}
```

---

## Documentation

### Doxygen Comments

- Use `/*! ... */` for multi-line documentation blocks
- Use `///` for single-line member variable comments
- Always document public APIs
- Follow the [Doxygen Style Guide](DOXYGEN_STYLE_GUIDE.md)
- Use [Doxygen Class Templates](DOXYGEN_CLASS_TEMPLATE.md) when documenting classes

```cpp
/*!
  \brief Returns the size of the string.

  This method returns the current number of characters in the string,
  excluding the terminating null character.

  \return The number of characters in the string.

  \see length()
*/
[[nodiscard]] constexpr std::size_t size() const noexcept;
```

### Documentation Templates

The project provides templates and style guides for Doxygen documentation:

- **[Doxygen Style Guide](DOXYGEN_STYLE_GUIDE.md)**: Comprehensive guide covering all aspects of Doxygen documentation, including formatting rules, tag usage, parameter documentation, and best practices.
- **[Doxygen Class Template](DOXYGEN_CLASS_TEMPLATE.md)**: Ready-to-use templates for documenting classes, including regular classes, template classes, structs, and simplified versions. Includes examples for all common documentation sections.

When creating new documentation:

1. Refer to the [Doxygen Style Guide](DOXYGEN_STYLE_GUIDE.md) for formatting rules and conventions
2. Use templates from [Doxygen Class Template](DOXYGEN_CLASS_TEMPLATE.md) as a starting point for class documentation
3. Follow the checklist in the Style Guide to ensure completeness

### Member Variable Documentation

- Use inline `//!<` comments for type aliases and simple member variables

```cpp
using value_type = type;  //!< Type of elements stored in the vector.
using size_type = std::size_t;  //!< Type used for vector size and capacity.
```

---

## Includes and Headers

### Header File Documentation

- Every header file must start with a file documentation block

```cpp
/*!
  \file   fixed_string.hpp
  \brief  Template string class with fixed-size character buffer.
*/
```

### Include Order

1. System headers (`<iostream>`, `<vector>`, etc.)
2. Third-party headers (`<SDL.h>`, etc.)
3. Project headers (`"core/fixed_string.hpp"`)

### Forward Declarations

- Prefer forward declarations when possible to reduce compilation dependencies

---

## Best Practices

### Const Correctness

- Mark methods as `const` when they don't modify object state
- Use `const` references for parameters that shouldn't be modified

```cpp
constexpr std::size_t size() const noexcept;
constexpr const char * data() const noexcept;
constexpr bool operator==(const FixedString & other) const noexcept;
```

### Default Member Initialization

- Use default member initialization when appropriate
- Prefer constructor initialization lists

```cpp
template <std::size_t allocatedSize>
constexpr FixedString() noexcept
  : _data{'\0'}
  , _size(0) {}
```

### Static Assertions

- Use `static_assert` for compile-time checks

```cpp
template <std::size_t allocatedSize>
class FixedString {
  static_assert(allocatedSize > 0, "FixedString capacity must be greater than zero.");
};
```

### Explicit Constructors

- Mark single-parameter constructors as `explicit` to prevent implicit conversions

```cpp
explicit FixedString(const char * string) noexcept;
explicit FixedVector(size_type count);
```

### Default and Delete

- Use `= default` for default constructors and destructors when appropriate
- Use `= delete` to prevent unwanted operations

```cpp
constexpr ~FixedString() noexcept = default;
FixedString(FixedString &&) = delete;
FixedString & operator=(FixedString &&) = delete;
```

### Range-Based For Loops

- Prefer range-based for loops when iterating over containers

```cpp
for (const auto & element : vector) {
  // Process element
}
```

### Auto Type Deduction

- Use `auto` when the type is obvious or verbose
- Avoid `auto` when it reduces readability

```cpp
// Good
auto size = string.size();
const auto & element = vector[0];

// Less clear - prefer explicit type
auto result = complexFunction();  // What type is result?
```

---

## Tools and Automation

### Clang-Format

- The project uses `.clang-format` for automatic code formatting
- Run `clang-format` before committing code
- Configuration is based on C++20 standard with 2-space indentation

### Pre-commit Checklist

Before committing code, ensure:

- [ ] Code is formatted with `clang-format`
- [ ] All public APIs are documented with Doxygen
- [ ] All functions are marked `constexpr` where appropriate
- [ ] All functions are marked `noexcept` where appropriate
- [ ] `[[nodiscard]]` is used for functions whose return values shouldn't be ignored
- [ ] Naming conventions are followed
- [ ] No compiler warnings
- [ ] Code follows the organization guidelines

---

## Summary

Key principles:

- **Consistency**: Follow the established conventions throughout the codebase
- **Modern C++**: Use C++23 features where appropriate
- **Safety**: Mark functions `constexpr` and `noexcept` where applicable
- **Clarity**: Write self-documenting code with meaningful names
- **Documentation**: Document all public APIs with Doxygen

This style guide should be followed for all new code and used as a reference when updating existing code. For questions or suggestions, please refer to the project maintainers.
