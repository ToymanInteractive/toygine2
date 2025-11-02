# Doxygen Documentation Template for Classes

This file contains templates for generating Doxygen documentation for classes in the ToyGine2 project. Use these templates as a reference when creating new documentation.

## General Rules

1. All sections should start with an emoji and **bold text** for the element name
2. Code examples should be practical and clear
3. Performance characteristics should be specified using Big-O notation
4. Safety guarantees should be described specifically and accurately
5. The `compatibility` section is only added for classes with special requirements

---

## Template 1: Regular Class (non-template)

Used for regular classes without template parameters (e.g., `Vector2D`, `Point`).

```cpp
/*!
  \class ClassName
  \brief Brief one-line description of the class.

  Detailed description of the class. Describe the class's purpose, main features,
  and scope of application. This description should provide an understanding of
  when and how to use the class. 2-4 sentences are usually sufficient.

  \section features Key Features

  - ⚙️ **Feature 1**: Brief description of the first class feature
  - 🔧 **Feature 2**: Description of the second feature (e.g., constexpr support)
  - 🛡️ **Feature 3**: Description of the third feature (e.g., exception safety)
  - 🎯 **Feature 4**: Description of the fourth feature (e.g., optimization for specific cases)
  - 🧬 **Feature 5**: Description of the fifth feature (e.g., type safety)

  \section usage Usage Example

  \code
  #include "path/to/class.hpp"

  // Basic usage example
  toy::namespace::ClassName obj(arg1, arg2);

  // More complex usage example
  auto result = obj.method();
  obj.anotherMethod(param);
  \endcode

  \section performance Performance Characteristics

  - ⚙️ **Construction**: O(1) constant time
  - 📝 **Assignment**: O(1) constant time
  - 🔗 **Operation1**: O(n) where n is...
  - ⚖️ **Comparison**: O(1) constant time
  - 💾 **Memory Usage**: X bytes (size description)
  - ⚡ **Cache Performance**: Description (e.g., "Excellent due to small size")
  - 📋 **Copy Performance**: Description (e.g., "Fast due to simple copying")

  \section safety Safety Guarantees

  - 🛡️ **Contracts & Debug Checks**: Description of debug mode checks
  - 🔒 **Bounds Safety**: Description of bounds safety
  - 📐 **Type Safety**: Description of type safety
  - ⚠️ **Exception Safety**: Description of exception guarantees (e.g., "All operations are noexcept")

  \note Additional note about the class, if necessary. For example, about specific
        usage or limitations.

  \see RelatedClass
  \see RelatedFunction
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

  Detailed description of the template class. Describe the purpose, main features,
  and scope of application. Specify what the class is designed for and in which
  cases it should be used.

  \tparam TemplateParam1 Description of the first template parameter. Specify constraints
                          and possible values.
  \tparam TemplateParam2 Description of the second template parameter, if any.

  \section features Key Features

  - ⚙️ **Feature 1**: Brief description of the first feature
  - 🔧 **Feature 2**: Description of the second feature (e.g., constexpr support)
  - 🛡️ **Feature 3**: Description of the third feature (e.g., exception safety)
  - 🔗 **Feature 4**: Description of the fourth feature (e.g., STL compatibility)
  - 🌍 **Feature 5**: Description of the fifth feature (e.g., UTF-8 support)
  - 🧬 **Feature 6**: Description of the sixth feature (e.g., type safety through concepts)

  \section usage Usage Example

  \code
  #include "path/to/class.hpp"

  // Basic usage example
  toy::ClassName<Type1, Size> obj(arg1, arg2);

  // More complex usage example
  constexpr auto obj2 = toy::ClassName<Type2, 32>("data");
  \endcode

  \section performance Performance Characteristics

  - ⚙️ **Construction**: O(1) or O(n) where n is...
  - 📝 **Assignment**: O(n) where n is the length...
  - 🔗 **Operation1**: O(n*m) where n is... and m is...
  - ⚖️ **Comparison**: O(n) where n is...
  - 💾 **Memory Usage**: Description (e.g., "Fixed at compile time, no heap allocation")
  - ⚡ **Cache Performance**: Description
  - 📋 **Copy Performance**: Description

  \section safety Safety Guarantees

  - 🛡️ **Contracts & Debug Checks**: Description of debug mode checks
  - 🔒 **Bounds Safety**: Description of bounds safety
  - 📐 **Type Safety**: Description of type safety (e.g., "Uses C++20 concepts")
  - ⚠️ **Exception Safety**: Description of exception guarantees

  \section compatibility Compatibility

  - 🆕 **C++ Standard**: C++ standard requirements (e.g., "Requires C++23 or later")
  - 🔗 **STL Integration**: STL compatibility
  - 🌐 **Cross-Platform**: Platform compatibility
  - 🔧 **Embedded Systems**: Suitability for embedded systems

  \note Additional note, if necessary. For example, about buffer size limitations
        or specifics of working with template parameters.

  \warning Important warning about features, if necessary. For example, about lifetime
           for non-owning classes or memory limitations.

  \see RelatedClass
  \see RelatedConcept
*/
template <typename TemplateParam1, std::size_t TemplateParam2>
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

  Detailed description of the struct. Describe the purpose, main features,
  data format, if applicable. Specify what the struct is designed for.

  \section features Key Features

  - ⚙️ **Feature 1**: Brief description of the first feature
  - 🔧 **Feature 2**: Description of the second feature (e.g., constexpr support)
  - 🛡️ **Feature 3**: Description of the third feature (e.g., exception safety)
  - 🔗 **Feature 4**: Description of the fourth feature (e.g., plain struct)
  - 🎯 **Feature 5**: Description of the fifth feature (e.g., versioning)
  - 🧬 **Feature 6**: Description of the sixth feature (e.g., type safety)
  - 🌍 **Feature 7**: Description of the seventh feature (e.g., standard)
  - 📏 **Feature 8**: Description of the eighth feature (e.g., immutable design)

  \section usage Usage Example

  \code
  #include "path/to/struct.hpp"

  // Basic usage example
  toy::namespace::StructName obj{value1, value2};

  // Example usage in constexpr context
  constexpr auto obj2 = toy::namespace::StructName{1, 2, 3, 4};

  // Field access
  std::cout << obj.field1 << "." << obj.field2 << std::endl;
  \endcode

  \section performance Performance Characteristics

  - ⚙️ **Construction**: O(1) constant time
  - 📝 **Access**: O(1) constant time for all members
  - 📝 **Assignment**: O(1) constant time for member assignment
  - 🔗 **Comparison**: O(1) constant time for comparison
  - 💾 **Memory Usage**: X bytes (size description, e.g., "4 * sizeof(type)")
  - ⚡ **Cache Performance**: Description
  - 📋 **Copy Performance**: Description

  \section safety Safety Guarantees

  - 🛡️ **Bounds Safety**: Description of bounds safety
  - 📐 **Type Safety**: Description of type safety
  - ⚠️ **Exception Safety**: Description of exception guarantees
  - 🔒 **Memory Safety**: Description of memory safety (e.g., "No dynamic allocation")

  \section compatibility Compatibility

  - 🌐 **Cross-Platform**: Platform compatibility
  - 🔧 **Embedded Systems**: Suitability for embedded systems
  - 📱 **Mobile Platforms**: Suitability for mobile platforms

  \note Additional note about the struct, if necessary. For example, that the
        struct is designed for compile-time version management.

  \see RelatedStandard
*/
struct StructName {
  // ...
};
```

---

## Template 4: Simplified Version (without compatibility)

Used for simple classes that do not require a `compatibility` section (e.g., `Vector2D`, `Point`).

```cpp
/*!
  \class ClassName
  \brief Brief one-line description of the class.

  Detailed description of the class. 2-4 sentences about purpose and usage.

  \section features Key Features

  - 🔧 **ConstExpr Support**: Most operations can be evaluated at compile time
  - 🛡️ **Exception Safety**: All operations are noexcept
  - 🎯 **Optimized**: Designed specifically for [specific application]
  - 🧬 **Type Safety**: Strong typing with clear [semantics]

  \section usage Usage Example

  \code
  #include "path/to/class.hpp"

  // Usage example
  toy::namespace::ClassName obj(value1, value2);
  obj.method();
  \endcode

  \section performance Performance Characteristics

  - ⚙️ **Construction**: O(1) constant time
  - 📝 **Assignment**: O(1) constant time
  - 🔗 **Arithmetic Operations**: O(1) constant time
  - ⚖️ **Comparison Operations**: O(1) constant time
  - 💾 **Memory Usage**: X bytes
  - ⚡ **Cache Performance**: Excellent due to small size
  - 📋 **Copy Performance**: Fast due to simple copying

  \section safety Safety Guarantees

  - 🛡️ **Contracts & Debug Checks**: Description of debug checks
  - 🔒 **Bounds Safety**: All operations are bounds-safe
  - 📐 **Type Safety**: Strong typing prevents [what it prevents]
  - ⚠️ **Exception Safety**: All operations are noexcept

  \note Additional note, if necessary.

  \see RelatedClass
*/
class ClassName {
  // ...
};
```

---

## Emoji Usage in Sections

### For Key Features

- ⚙️ - general feature/functionality
- 🔧 - constexpr support, compile-time evaluation
- 🛡️ - exception safety, safety
- 🎯 - optimization for specific cases
- 🔗 - compatibility (STL, standards)
- 🌍 - international support (UTF-8, Unicode)
- 🧬 - type safety
- 📏 - size/limitations
- 🆕 - C++ standard requirements
- 🔒 - memory safety

### For Performance Characteristics

- ⚙️ - Construction
- 📝 - Assignment
- 🔗 - Operations
- ⚖️ - Comparison
- 💾 - Memory Usage
- ⚡ - Cache Performance
- 📋 - Copy Performance
- 🎯 - Class-specific operations

### For Safety Guarantees

- 🛡️ - Contracts & Debug Checks
- 🔒 - Bounds Safety / Null Pointer Safety
- 📐 - Type Safety
- ⚠️ - Exception Safety
- ⏰ - Lifetime Safety (for non-owning classes)

### For Compatibility

- 🆕 - C++ Standard
- 🔗 - STL Integration
- 🌐 - Cross-Platform
- 🔧 - Embedded Systems
- 📱 - Mobile Platforms

---

## Filling Guidelines

### \brief

- One line
- Brief, informative description
- Format: "What the class does" + "what it's used for"

### Detailed Description

- 2-4 sentences
- Explains purpose and scope of application
- Does not duplicate `\brief`

### \section features

- 4-8 items
- Each item: emoji + **bold text** + description
- Starts with the most important features

### \section usage

- Practical example
- Shows typical usage
- Includes `#include` directive
- Code should be compilable

### \section performance

- Specify complexity (Big-O notation)
- Specific memory values if important
- Describe cache-friendly properties if applicable

### \section safety

- Specific guarantees
- Mention debug mode checks
- Specify exception guarantees

### \section compatibility (optional)

- Only for classes with special requirements
- Template classes usually need this section
- Simple classes can do without it

### \note

- Additional information
- Does not duplicate main description
- Useful usage tips

### \warning (optional)

- Important warnings
- Rarely used, only for critical points
- For example, lifetime issues, ownership semantics

### \see

- Related classes, functions, concepts
- No more than 2-3 references
- Only relevant connections

---

## Pre-Commit Checklist

- [ ] `\class` / `\struct` is present
- [ ] `\brief` is filled (one line)
- [ ] Detailed description is present (2-4 sentences)
- [ ] `\section features` contains 4+ items with emoji
- [ ] `\section usage` contains a working code example
- [ ] `\section performance` is filled with complexity specification
- [ ] `\section safety` is filled with specific guarantees
- [ ] `\section compatibility` is added if needed
- [ ] `\note` is added if there is useful information
- [ ] `\warning` is added if there are critical warnings
- [ ] `\see` contains relevant references
- [ ] Emoji are used consistently
- [ ] All descriptions are written in English

---

## Examples from the Project

For reference, see:

- `FixedString` - full template for template class
- `CStringView` - full template with `\warning`
- `Vector2D` - simplified template without `compatibility`
- `Version` - template for struct
