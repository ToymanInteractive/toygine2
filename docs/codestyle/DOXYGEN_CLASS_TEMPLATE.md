# Doxygen Documentation Template for Classes

Templates for generating Doxygen class documentation in the ToyGine2 project.

## General Rules

1. Each item in `\section features Key Features` uses **bold** feature names.
2. All sections use plain text without emoji.
3. Code examples must be practical and compilable.
4. Performance characteristics use Big-O notation.
5. Safety guarantees must be specific and accurate.
6. The `compatibility` section is only added for classes with special requirements.
7. Documentation must be concise, neutral, and technical. Avoid marketing language.

---

## Template 1: Regular Class (non-template)

Used for regular classes without template parameters (e.g., `Vector2`, `Point`).

```cpp
/*!
  \class ClassName
  \brief Brief one-line description of the class.

  Detailed description of the class purpose, scope, and intended usage.
  2-4 sentences are usually sufficient. Do not duplicate the \brief.

  \section features Key Features

  - **Feature 1**: Brief description of the first feature
  - **Feature 2**: Description (e.g., constexpr support)
  - **Feature 3**: Description (e.g., all operations are noexcept)
  - **Feature 4**: Description (e.g., optimized for a specific use case)

  \section usage Usage Example

  \code
  #include "path/to/class.hpp"

  toy::namespace::ClassName obj(arg1, arg2);
  auto result = obj.method();
  \endcode

  \section performance Performance Characteristics

  - **Construction**: O(1) constant time
  - **Assignment**: O(1) constant time
  - **Operation1**: O(n) where n is...
  - **Comparison**: O(1) constant time
  - **Memory usage**: X bytes (description)

  \section safety Safety Guarantees

  - **Contracts**: Description of debug-mode checks
  - **Bounds safety**: Description of bounds checking
  - **Type safety**: Description of type safety
  - **Exception safety**: All operations are noexcept

  \note Additional note about the class, if necessary.

  \sa \ref toy::namespace::RelatedClass
  \sa toy::namespace::relatedFunction()
*/
class ClassName {
  // ...
};
```

---

## Template 2: Template Class

Used for template classes (e.g., `FixedString`, `FixedVector`).

```cpp
/*!
  \class ClassName
  \brief Brief one-line description of the template class.

  Detailed description of purpose, scope, and intended usage.
  Specify what the class is designed for and when it should be used.

  \tparam TemplateParam1 Description. Specify constraints and possible values.
  \tparam TemplateParam2 Description, if any.

  \section features Key Features

  - **Feature 1**: Brief description
  - **Feature 2**: Description (e.g., constexpr support)
  - **Feature 3**: Description (e.g., all operations are noexcept)
  - **Feature 4**: Description (e.g., STL compatibility)
  - **Feature 5**: Description (e.g., UTF-8 support)
  - **Feature 6**: Description (e.g., type safety through concepts)

  \section usage Usage Example

  \code
  #include "path/to/class.hpp"

  toy::ClassName<Type1, Size> obj(arg1, arg2);
  constexpr auto obj2 = toy::ClassName<Type2, 32>("data");
  \endcode

  \section performance Performance Characteristics

  - **Construction**: O(1) or O(n) where n is...
  - **Assignment**: O(n) where n is the length...
  - **Operation1**: O(n*m) where n is... and m is...
  - **Comparison**: O(n) where n is...
  - **Memory usage**: Fixed at compile time, no heap allocation

  \section safety Safety Guarantees

  - **Contracts**: Description of debug-mode checks
  - **Bounds safety**: Description of bounds checking
  - **Type safety**: Description (e.g., uses C++20 concepts)
  - **Exception safety**: All operations are noexcept

  \section compatibility Compatibility

  - **C++ standard**: Requires C++20 or later
  - **STL integration**: Description
  - **Cross-platform**: Supported on all target platforms
  - **Embedded systems**: Suitable (no dynamic allocation)

  \note Additional note, if necessary (e.g., buffer size limitations).

  \warning Important warning, if necessary (e.g., lifetime or ownership semantics).

  \sa \ref toy::namespace::RelatedClass
  \sa \ref toy::namespace::RelatedConcept
*/
template <typename TemplateParam1, size_t TemplateParam2>
class ClassName {
  // ...
};
```

---

## Template 3: Struct

Used for structures (e.g., `Version`).

```cpp
/*!
  \struct StructName
  \brief Brief one-line description of the struct.

  Detailed description of purpose, data format, and intended usage.

  \section features Key Features

  - **Feature 1**: Brief description
  - **Feature 2**: Description (e.g., constexpr support)
  - **Feature 3**: Description (e.g., all operations are noexcept)
  - **Feature 4**: Description (e.g., immutable design)

  \section usage Usage Example

  \code
  #include "path/to/struct.hpp"

  toy::namespace::StructName obj{value1, value2};
  constexpr auto obj2 = toy::namespace::StructName{1, 2, 3, 4};
  \endcode

  \section performance Performance Characteristics

  - **Construction**: O(1) constant time
  - **Access**: O(1) constant time for all members
  - **Comparison**: O(1) constant time
  - **Memory usage**: X bytes (e.g., 4 * sizeof(type))

  \section safety Safety Guarantees

  - **Type safety**: Description
  - **Exception safety**: All operations are noexcept
  - **Memory safety**: No dynamic allocation

  \note Additional note, if necessary.

  \sa \ref toy::namespace::RelatedStandard
*/
struct StructName {
  // ...
};
```

---

## Template 4: Simplified Version (without compatibility)

Used for simple classes that do not require a `compatibility` section (e.g., `Vector2`, `Point`).

```cpp
/*!
  \class ClassName
  \brief Brief one-line description of the class.

  Detailed description of purpose and usage. 2-4 sentences.

  \section features Key Features

  - **Constexpr support**: Most operations can be evaluated at compile time
  - **Exception safety**: All operations are noexcept
  - **Optimized**: Designed for [specific application]
  - **Type safety**: Strong typing with clear [semantics]

  \section usage Usage Example

  \code
  #include "path/to/class.hpp"

  toy::namespace::ClassName obj(value1, value2);
  obj.method();
  \endcode

  \section performance Performance Characteristics

  - **Construction**: O(1) constant time
  - **Assignment**: O(1) constant time
  - **Arithmetic**: O(1) constant time
  - **Comparison**: O(1) constant time
  - **Memory usage**: X bytes

  \section safety Safety Guarantees

  - **Contracts**: Description of debug-mode checks
  - **Bounds safety**: All operations are bounds-safe
  - **Type safety**: Strong typing prevents [what it prevents]
  - **Exception safety**: All operations are noexcept

  \note Additional note, if necessary.

  \sa \ref toy::namespace::RelatedClass
*/
class ClassName {
  // ...
};
```

---

## Filling Guidelines

### \brief

- One line.
- Format: what the class does + what it is used for.

### Detailed Description

- 2-4 sentences.
- Explains purpose and scope. Does not duplicate `\brief`.

### \section features

- 4-8 items.
- Each item: **bold name** + description.
- Most important features first.

### \section usage

- Practical, compilable example.
- Includes the `#include` directive.

### \section performance

- Big-O complexity for key operations.
- Memory usage if relevant.
- Plain text with **bold operation names**.

### \section safety

- Specific guarantees.
- Debug-mode checks, exception policy.
- Plain text with **bold category names**.

### \section compatibility (optional)

- Only for classes with special requirements.
- Plain text with **bold category names**.

### \note

- Additional information. Does not duplicate the main description.

### \warning (optional)

- Critical warnings only (lifetime, ownership, etc.).

### \sa

- Related classes, functions, concepts.
- 2-3 references maximum.
- Non-function symbols (classes, types, enums, namespaces, concepts) must use `\ref` with full namespace qualification (e.g. `\sa \ref toy::CStringView`).
- Functions, methods, and operators remain plain text and must be fully qualified when the target is outside the surrounding scope (e.g. `\sa toy::format()`).
- Standard-library symbols, macros, and URLs stay as plain text without `\ref`.

---

## Pre-Commit Checklist

- [ ] `\class` / `\struct` is present
- [ ] `\brief` is filled (one line)
- [ ] Detailed description is present (2-4 sentences)
- [ ] `\section features` contains 4+ items with **bold** names
- [ ] `\section usage` contains a compilable code example
- [ ] `\section performance` is filled with Big-O complexity
- [ ] `\section safety` is filled with specific guarantees
- [ ] `\section compatibility` is added if needed
- [ ] `\note` is added if there is useful information
- [ ] `\warning` is added if there are critical warnings
- [ ] `\sa` contains relevant references
- [ ] No emoji in documentation
- [ ] All descriptions are written in English
- [ ] No marketing language anywhere

---

## Examples from the Project

For reference, see:

- `FixedString` — template class with compatibility section
- `CStringView` — template class with `\warning`
- `Vector2` — simplified template without compatibility
- `Version` — struct template
