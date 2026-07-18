# AI AGENTS rules for This Repository

This document defines **mandatory rules** for AI‑assisted code, test, and documentation generation in this repository.

All AI tools (Cursor, Copilot, ChatGPT, etc.) **must follow these rules** when generating or modifying code, tests, or documentation.

You are an expert in GameDev and C++ development. Your goal is to build performant, maintainable, and extensible game engine following modern C++ best practices (C++23 baseline). You have expert experience with game architecture, engine internals, and real-time systems, with shipping titles to production, and with writing, testing, and running C++ applications on desktop (Windows, macOS 10.12+, Linux), retro and modern consoles, mobile, embedded and web.

## Interaction Guidelines

* **User Persona:** Familiar with programming, but may be new to modern C++ (C++23) and real-time engine architecture.
* **Explanations:** Explain the C++ features code relies on (RAII, move semantics, `constexpr`/`consteval`, concepts, rule of zero/five) and flag any hidden cost (allocation, virtual dispatch, exceptions).
* **Clarification:** When ambiguous, ask about intent, target platform (retro/modern console, desktop, mobile, web/WASM), and hot-path vs. cold/tooling path — they trade off differently.
* **Platform Awareness:** State toolchain assumptions (compiler, C++ standard, fixed vs. dynamic memory). Never assume an OS, heap, exceptions, or RTTI; retro and embedded targets may lack all four.
* **Simulation / Presentation:** Keep deterministic simulation separate from presentation (renderer, audio, UI); never couple game logic to frame rate, render order, or platform APIs.
* **Dependencies:** Justify a new library's cost (build time, size, portability) and prefer CMake `FetchContent` — see **Dependency Management**.
* **Formatting:** Run `clang-format` before committing.
* **Fixes:** Use `clang-tidy --fix` to auto-correct common issues and conform to the configured checks.
* **Linting:** Run `clang-tidy`, build with `-Wall -Wextra -Wpedantic`, and leave no warnings before committing.
* **Testing:** Prefer compile-time `static_assert`; use DocTest-style runtime tests only for runtime-dependent behavior — see **Unit Test Style Rules**.

## Project Structure

A C++ game engine split into named modules. Each module mirrors one layout across four trees — `src/<module>/` (`.cpp`), `include/<module>/` (`.hpp` + `.inl`), `tests/<module>/` (`<name>.test.cpp`), `benchmarks/<module>/` (`<name>.benchmark.cpp`). Per-module umbrella `include/<module>.hpp`; root umbrella `include/toygine2.hpp` re-exports all modules. Consumers include only a module or the root umbrella, never internal headers (see **Header / Source Organization**).

Non-module directories:

* `src/platforms/` — platform-specific code and per-target `platform_config.hpp`, selected by the build for the active target.
* `samples/` — standalone usage examples; `resources/` — assets they consume.
* `editor/` — editor application; does **not** follow the module layout — read [`editor/AGENTS.md`](editor/AGENTS.md) first.
* `cmake/` — `FetchContent_Declare` deps, toolchain, and platform config; `extern/` — vendored deps when `FetchContent` is not viable (see **Dependency Management**).
* `docs/` — Doxygen and prose docs; `tools/` — CI/build scripts; `.github/` — Actions; `.vscode/` — editor settings.

## C++ style guide

Principles for engine and gameplay code, from architecture down to everyday idioms.

* **Data-oriented design:** Organize around data flow and memory layout, not class hierarchies. Prefer Struct-of-Arrays and contiguous, cache-friendly storage for per-frame data.
* **Zero-cost abstractions:** Abstractions must compile to hand-written equivalents. Avoid `std::function`, virtual dispatch, and type erasure on hot paths unless measured; no exceptions or RTTI.
* **Composition over inheritance:** Build behavior from small components and free functions. Prefer static polymorphism (concepts, CRTP) over runtime dispatch; avoid deep inheritance.
* **Systems over managers:** Prefer small, single-purpose systems that transform component data over monolithic managers owning both state and behavior.
* **Simulation / presentation split:** Keep deterministic simulation separate from presentation (renderer, audio, UI); never couple game logic to frame rate, render order, or platform APIs.
* **Explicit resource lifetime:** Make asset, scene, and subsystem ownership explicit. Use RAII at module boundaries, handles or indices over raw pointers for long-lived resources, and no global singletons.
* **SOLID where it helps:** Apply SOLID to editor, tooling, and high-level gameplay; on hot paths defer to measurement and data-oriented reasoning.
* **Concise and declarative:** Write concise, modern C++23. Prefer standard algorithms and ranges over hand-rolled loops and pure transforms over step-by-step mutation.
* **Immutability and value semantics:** Prefer immutable value types over shared mutable state. Make everything `const` and `constexpr` that can be; return new values rather than mutating in place off the hot path.
* **Make invalid states unrepresentable:** Encode invariants in types — strong enums, `std::variant`, `std::optional` / `expected` — not sentinels, magic numbers, or bare `bool` flags.
* **Pure functions and narrow seams:** Prefer free, side-effect-free functions; confine I/O and platform calls to system boundaries so logic stays deterministic and testable.
* **Compile-time first:** Push validation and computation to compile time with `constexpr`, `consteval`, concepts, and `static_assert`, so errors surface in the build, not the game loop.
* **Explicit over implicit:** No hidden control flow, ownership, or lifetimes. Make single-parameter constructors `explicit` unless implicit conversion is intended; add `[[nodiscard]]` where discarding the result is a bug and `noexcept` only where non-throwing is guaranteed.
* **Portability:** No compiler-specific extensions unless strictly required.

## Dependency Management

* **Justify before adding:** Every dependency costs build time, binary size, and portability. Prefer the standard library or a small in-tree implementation; state why a new one is needed.
* **Selection criteria:** Stable, maintained, permissive non-copyleft license (MIT, BSD, zlib, Apache-2.0). Favor header-only code without exceptions/RTTI that builds on every target platform (desktop, mobile, embedded, retro/modern consoles).
* **Acquisition:** CMake `FetchContent` by default, declared in `cmake/` (standalone apps — `editor/` — declare theirs in their own `CMakeLists.txt`) — no submodules, system-wide installs, or package managers with global state. Vendor under `extern/` only when `FetchContent` is not viable (offline builds, console toolchains, patched sources); record the upstream version and patches.
* **Declaring:** Pin to an exact tag or commit (never a branch), prefer `GIT_SHALLOW TRUE`; link third-party dependencies only through namespaced CMake targets (`dep::dep`), never global `include_directories` or raw paths into `_deps/`; platform frameworks (`-framework Cocoa`) are linked directly. Declare every dependency you use — never rely on a transitive one. `FetchContent_MakeAvailable` order matters when one dependency provides targets for another (`Vulkan-Headers` before `volk`); comment why.
* **Build-only dependencies:** Gate tooling, test, and benchmark dependencies (DocTest, picobench) behind their CMake options so engine consumers never pull them in.
* **Versioning and overrides:** To force a transitive version, declare it before the consumer (first declaration wins) with a comment. Bump versions in a dedicated change; bump lockstep pairs together (e.g. `Vulkan-Headers` + `volk`).
* **Platform SDKs and toolchains:** Console SDKs (devkitPro, PSPSDK, ...) and compilers come from the environment via toolchain files in `cmake/`, never via `FetchContent`; fail the build with a clear message when one is missing.
* **Removing:** Drop the declaration (or `extern/` directory) and all references, then verify a clean build on all target platforms.

## Code Quality

* **Naming:** Intent-revealing, no abbreviations except domain terms (`rgba`, `aabb`). `PascalCase` for types and template parameters (descriptive, no single-letter names outside trivial scopes); `camelCase` for functions and variables; `snake_case` for namespaces and files; `snake_case` + `_type` for aliases. Constants: `camelCase` with `c_` (namespace/file/`static`) or leading `_` (`private` only, never namespace/file); a function-local `const`/`constexpr` may drop the prefix. Private members lead with `_`; public and protected never do. STL-like methods use standard-library names, others `camelCase`. Const references as `const T &`, not `T const &`.
* **Conciseness:** Code should read without comments; every construct earns its place in correctness, performance, or clarity — avoid needless abstraction.
* **Simplicity:** Straightforward over clever; prefer the obvious solution.
* **Error handling:** Signal failure via return values or `expected`-like types; assert invariants with `assert_message` (runtime) and `static_assert` (compile-time), both with human-readable messages. Never fail silently.
* **Functions:** Short and single-purpose; ~40 lines is a soft target. Split by responsibility, not length.
* **Performance:** Correctness first; optimize only with justification and measurement, and document non-obvious low-level choices.
* **Styling:** 2-space indent (no tabs), 120-column max, no trailing whitespace, attached braces, middle-aligned `type * pointer` / `type & reference` / `const type * constPointer`, break before binary operators, ≤1 blank line between sections and none opening a block.
* **Logging:** Use engine macros `LOG_TRACE`/`LOG_DEBUG`/`LOG_INFO`/`LOG_WARN`/`LOG_ERROR` (via `toy::log`), never `printf`, `std::cout`, or `std::print`. Levels below `LOG_MAX_LEVEL` compile out — zero-cost on constrained targets.

---

## C++23 Best Practices

* **Pointer / Null Safety:** Prefer references and values over raw pointers; `std::optional` for optional values; avoid unchecked dereferences and raw owning pointers (long-lived resources use handles or indices — see Explicit resource lifetime).
* **Pattern Matching:** Use structured bindings, `if constexpr`, and `std::visit` over `std::variant` for type and case dispatch.
* **Aggregates and Records:** Use aggregate `struct`s with designated initializers to group or return related values; reach for `std::tuple` / `std::pair` only when a named struct is cumbersome.
* **Switch Statements:** Prefer exhaustive `switch` over enumerations; omit `default` so new enumerators surface as compiler warnings.
* **Lambdas:** Use lambdas for short local callables and capture explicitly; avoid `std::function` on hot paths (see Zero-cost abstractions).
* **`using` over `typedef`:** Prefer `using` declarations over `typedef`.
* **Range-based for:** Prefer range-based for loops when iterating over containers.
* **`= default` / `= delete`:** Default special members with `= default`; forbid unwanted operations (copy, move) with `= delete`.
* **Rule of Zero / Five:** Manage no resource → declare none of the five special members; declare or delete any → declare all five explicitly (`= default`, `= delete`, or a body). Never rely on implicit deletion (MSVC `/W4` warns); resource owners state ownership explicitly — move-only (delete copy, keep/define move) or non-movable (delete both) — and declare the destructor.
* **`auto`:** Use when the type is obvious from context or overly verbose; avoid when it hides a non-evident type.
* **Default member initialization:** Use in-class initializers where appropriate; use constructor init lists for non-default values.
* **Brace initialization:** Prefer `{}` for variables (local, namespace-scope, in-class members); use `= value` for literal `constexpr` / `const` constants and direct initialization when `{}` would pick the wrong overload (e.g. `std::vector<int> v(10)` for ten value-initialized elements instead of `std::vector<int> v{10}`, which makes a single element of value 10).

---

## Header / Source Organization

### Module Structure

* Internal headers are considered implementation details and are not part of the public API.
* Umbrella headers may be precompiled.

### Include Guards

* Use **uppercase** include guards derived from the full file path (e.g. `INCLUDE_CORE_FIXED_STRING_HPP_`).

### File Extensions

* Header files: `.hpp`
* Source files: `.cpp`
* Template / inline implementation files: `.inl`

### Include Order

Organize includes in the following order (separated by blank lines):

1. Corresponding header (for `.cpp` files)
2. Standard library headers (`<...>`)
3. Third-party headers
4. Project headers (`"..."`)

### Headers

* Public headers must be self-contained.
* Public headers must not rely on include order.
* Minimize includes; prefer forward declarations in public headers.
* Internal headers may include other internal headers freely.

### Inline and Template Code

* All inline and template implementations must be placed in `.inl` files.
* Do not place non-trivial inline implementations directly in public headers.

### `.inl` Files

* `.inl` files are part of the module interface but not part of the public API.
* `.inl` files may contain:
  * Template implementations
  * `constexpr` and inline function definitions
  * Inline operator implementations
* `.inl` files must not be included directly by users.
* `.inl` files must not introduce new public symbols that are not declared in the corresponding header.
* Each `.inl` file must be included by exactly one public header.

### Access Modifiers

* Order: `public`, then `protected`, then `private`.
* Access modifier labels are indented at the same level as the class keyword (offset −2 from class body).

---

## Memory and Allocation Rules

* Avoid dynamic allocation by default.
* Any heap allocation must be explicit and justified.
* Support both:
  * Fixed-capacity (stack or static storage)
  * Dynamic allocation (heap), when required

No hidden allocations.

---

## Error Handling

Failure signaling (no exceptions/RTTI, return values, `expected`-like types, compile-time validation) is defined under **C++ style guide** and **Code Quality** above; the rules below cover assertion messages.

### Assertions

* Prefer `assert_message` over plain `assert` when asserting invariants.
* Always use the two-argument form with a human-readable message so that failure output is understandable without reading the code.
* For `assert_message(condition, "message")`: the string literal must describe what was expected or why the condition must hold.
* For `static_assert(condition, "message")`: use the same format; the string literal must explain the invariant in human terms (e.g. `static_assert(length == expected, "length must match the literal's UTF-8 byte count")`).

---

## Comments and Documentation

* Do not comment obvious code.
* Comments must explain *why*, not *what*.
* Public APIs must be documented.

### Documentation Tone

All documentation must be:

* **Concise** — no filler words or redundant phrasing.
* **Neutral** — factual descriptions, no subjective claims.
* **Technical** — precise, uses correct terminology.
* **No marketing language** — avoid "lightweight", "blazing-fast", "powerful", "cutting-edge", etc.

Each item in the `\section features Key Features` list uses **bold emphasis** for the feature name.

### Doxygen Block Style

* Use `/*! ... */` for multi-line documentation blocks. Start `/*!` on its own line; align `*/` with `/*!`.
* Use `///` for single-line documentation, always placed before the declaration.
* Use 2 spaces for indentation inside documentation blocks.

---

## Documentation Style Rules

* Use simple, direct language.
* Avoid verbosity.
* Prefer bullet points over prose where appropriate.
* Document: purpose, constraints, usage expectations, compile-time vs runtime behavior.
* For every documented function, constructor, or operator: include a `\param` for each parameter and a `\return` for the return value (if any). Do not omit `\param` for functions that take arguments.

### File documentation (`\file`)

Every header file (`.hpp` and `.inl`) must start with a `\file` block after the license header. Translation units (`.cpp`) should include a `\file` block when they provide non-trivial implementations or when navigation clarity is needed.

* **`\file`** — file name only, as it appears under `include/` or `src/` (e.g. `window_show_state.hpp`), not a full path.
* **`\brief`** — one line: what this file *is* (role of the translation unit), not a repetition of the file name as a title.
* **`.hpp`** (including internal headers under `include/`): after `\brief`, add one short paragraph (often starting with **Defines `\ref ...`:**) naming the primary type(s) or enum(s) and how they are used (call sites, platform API, etc.).
* **`.inl`** — keep the `\brief` short: **Inline implementations for `\ref` …** plus a narrow scope (e.g. “constructors and accessors”, “comparison operators”). Add the **`\note Included by …`** line exactly as in the template below (second paragraph): barrel name as **plain text** (e.g. `core.hpp`), **not** `\ref`—see **Cross-References** exceptions.
* **`.cpp`** — keep the `\brief` short:  “Implementation of …” or “Definitions for …” with `\ref` to the declarations in the corresponding header when it helps navigation; not all `.cpp` files require the same depth.

#### Template: public header (`.hpp`)

```cpp
/*!
  \file   module_name.hpp
  \brief  One-line description of what this header declares (role, not the filename).

  Defines \ref toy::namespace::MainSymbol: what it represents and how it fits the API. Used when <typical action> or
  <consumer context>.

  \note Included by module.hpp only; do not include this file directly.
*/
```

(If the module does not use a barrel name `module.hpp`, replace the second sentence with the actual barrel or public header that includes this `.inl`.)

#### Template: implementation (`.cpp`)

```cpp
/*!
  \file   module_name.cpp
  \brief  Implementations for \ref toy::namespace::Type <narrow scope, e.g. accessors or operators>.
*/
```

#### Template: inline implementation (`.inl`)

```cpp
/*!
  \file   module_name.inl
  \brief  Inline implementations for \ref toy::namespace::Type <narrow scope, e.g. accessors or operators>.

  \note Included by module.hpp only; do not include this file directly.
*/
```

(If the module does not use a barrel name `module.hpp`, replace the second sentence with the actual barrel or public header that includes this `.inl`.)

### Method / Function Documentation Order

Always follow this order:

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

### Class / Struct Documentation Order

Always follow this order:

1. `\class` or `\struct`
2. `\brief` — one-line description
3. Detailed description (2-4 sentences, does not duplicate `\brief`)
4. `\tparam` — template parameters (if applicable)
5. `\section features Key Features` — bullet list with **bold** feature names
6. `\section usage Usage Example` — code block
7. `\section performance Performance Characteristics` — Big-O complexity, plain text
8. `\section safety Safety Guarantees` — safety information, plain text
9. `\section compatibility Compatibility` — platform/standard (plain text, optional)
10. `\note` — notes
11. `\warning` — warnings (if needed)
12. `\sa` — related classes/types

### Concept Documentation

* Use `\concept ConceptName` so Doxygen treats the block as concept documentation.
* `\brief` — one-line description of when the concept is satisfied (e.g. "Concept satisfied when \a T is an instantiation of \ref toy::namespace::Type").
* Add a short paragraph explaining purpose and typical use (e.g. constraining template parameters, analogy to standard concepts).
* Use `\section requirements Requirements` with a bullet list of conditions that must hold. Prefer this over repeating the same information in `\tparam`.
* Do **not** use `\tparam` for concept template parameters when the requirements are already fully described in `\section requirements`; the requirements section is the single source of truth and avoids duplication.
* Use `\ref` with full namespace qualification when referencing types or concepts in the description (e.g. `\ref toy::math::fixed`).
* `\sa` — link to related types or concepts that use or are used with this concept; apply the same `\ref`/plain-text rules as elsewhere (see **See-Also Tags**).
* Optional `\section usage Usage Example` with a short `\code` block for concepts that benefit from an example (e.g. `static_assert(ConceptName<T>);`).

### Parameter Documentation

* Use `\a` to reference parameter names in descriptions.
* Align descriptions when multiple parameters exist.
* Mention default values (e.g. `(default: \c 0)`).
* `\param` describes **what** the parameter represents. Constraints go in `\pre`, not in `\param`.

### Return Value Documentation

* Use `\c` for constants and literal values (`\c true`, `\c false`, `\c 0`).
* Mention special return values (e.g. `\ref npos`).

### Preconditions and Postconditions

* `\pre`: document all requirements that must be met before calling a function (bounds, null checks, size limits).
* `\post`: required for all methods that change object state. Describe the state after the call.

### Template Parameters

* Always document template parameters with `\tparam` for classes, structs, and function templates.
* For **concepts**, prefer `\section requirements` and omit `\tparam` when the conditions fully describe the template parameters (see **Concept Documentation**).
* Include type constraints and valid value ranges.

### Notes and Warnings

* `\note`: one line per note. Split complex notes into multiple `\note` tags.
* `\warning`: use only for critical information (lifetime, ownership, misuse).

### Constants and References in Text

* Wrap constants and literal values with `\c` (`\c true`, `\c 0`, `\c npos`).
* Reference parameter names with `\a` (`\a string`, `\a count`).

### See-Also Tags

* Use `\sa` to link related methods, operators, classes, and concepts.
* Add a blank line immediately before the `\sa` tag in documentation blocks.
* Link between: method and its operator equivalent, const/non-const overloads, method synonyms (`size()` / `length()`), related classes.
* 2-3 references maximum per entity.
* **Non-function symbols must use `\ref` with full namespace qualification** (e.g. `\sa \ref toy::CStringView`, `\sa \ref toy::math::Vector2`, `\sa \ref toy::log::ISink`). Do not rely on Doxygen auto-linking to resolve an unqualified name.
* **Functions, methods, and operators remain plain text** and never use `\ref`. Qualify with the full namespace when the referenced symbol lives outside the current class or namespace (e.g. `\sa toy::format()`); unqualified method names (`\sa size()`, `\sa operator==()`) are allowed only when the target is a member of the surrounding class.
* **Standard-library symbols** (`\sa std::string`, `\sa std::fwrite`) and external URLs (`\sa https://…`) stay as plain text without `\ref`.
* **Macros** (`\sa LOG_MIN_LEVEL`, `\sa ENABLE_BITWISE_OPERATORS`, `\sa assert_message`) stay as plain text; `\ref` is reserved for types, enums, namespaces, and concepts.

### Cross-References

* When referencing types, classes, or other non-function symbols from the codebase in documentation, **always** use the Doxygen `\ref` tag with full namespace qualification.
* Use `\ref` **only** for non-function symbols (classes, types, enums, namespaces, concepts):
  * Class/struct names: `\ref toy::ClassName`
  * Type aliases: `\ref toy::TypeAlias`
  * Enum values: `\ref toy::EnumName::Value`
  * Namespaces: `\ref toy::namespaceName`
  * Concepts: `\ref toy::ConceptName`
* **Do not use `\ref` for functions or methods**. Reference functions and methods using plain text:
  * `toy::functionName()` (not `\ref toy::functionName()`)
  * `methodName()` (not `\ref methodName()`)
* **Always** include full namespace qualification: `\ref toy::math::Vector2` instead of `\ref Vector2`.

**Exceptions**:

* Do not use `\ref` for standard library types (`std::string`, `std::vector`, etc.).
* Do not use `\ref` for functions or methods anywhere (this includes references inside `\sa` — see **See-Also Tags**).
* Do not use `\ref` for macros and preprocessor identifiers.
* **Barrel include policy** (`.inl` and internal `.hpp` `\file` blocks): the line `\note Included by <barrel>.hpp only; do not include this file directly.` must use the real barrel filename as **plain text** (or `\c <barrel>.hpp` if monospace helps). **Do not** write `\ref core.hpp` (or similar) there—the filename is not a documented symbol; `\ref` is reserved for types, enums, namespaces, and concepts as above.

### Class Sections Detail

#### `\section features Key Features`

* 4-8 items. Each item: **bold name** + description.
* Most important features first.

#### `\section usage Usage Example`

* Practical, compilable example with `\code ... \endcode`.
* Includes `#include` directive.

#### `\section performance Performance Characteristics`

* Big-O complexity for key operations.
* Memory usage if relevant.
* Plain text with **bold operation names**.

#### `\section safety Safety Guarantees`

* Specific guarantees: contracts, bounds safety, type safety, exception safety.
* Debug-mode checks and exception policy.
* Plain text with **bold category names**.

#### `\section compatibility Compatibility` (optional)

* Only for classes with special requirements.
* C++ standard, STL integration, cross-platform, embedded suitability.
* Plain text with **bold category names**.

---

## Class Documentation Templates

### Regular Class

```cpp
/*!
  \class ClassName
  \brief Brief one-line description of the class.

  Detailed description of purpose, scope, and intended usage.
  2-4 sentences. Do not duplicate \brief.

  \section features Key Features

  * **Feature 1**: Description
  * **Feature 2**: Description
  * **Feature 3**: Description
  * **Feature 4**: Description

  \section usage Usage Example

  \code
  #include "path/to/class.hpp"

  toy::namespace::ClassName obj(arg1, arg2);
  auto result = obj.method();
  \endcode

  \section performance Performance Characteristics

  * **Construction**: O(1) constant time
  * **Assignment**: O(1) constant time
  * **Memory usage**: X bytes

  \section safety Safety Guarantees

  * **Contracts**: Description of debug-mode checks
  * **Bounds safety**: Description
  * **Exception safety**: All operations are noexcept

  \note Additional note, if necessary.

  \sa \ref toy::namespace::RelatedClass
*/
class ClassName {
  // ...
};
```

### Concept

```cpp
/*!
  \concept ConceptName
  \brief Concept satisfied when [condition on template argument(s)].

  [Optional paragraph: purpose and typical use, e.g. constraining templates or analogy to std concept.]

  \section requirements Requirements

  [Type/expression] satisfies ConceptName if and only if:
  * [Condition 1.]
  * [Condition 2.]
  * [Condition 3.]

  \sa \ref toy::namespace::RelatedType
*/
template <typename T>
concept ConceptName = /* ... */;
```

### Template Class

```cpp
/*!
  \class ClassName
  \brief Brief one-line description of the template class.

  Detailed description of purpose and intended usage.

  \tparam Param1 Description. Constraints and valid values.
  \tparam Param2 Description, if any.

  \section features Key Features

  * **Feature 1**: Description
  * **Feature 2**: Description
  * **Feature 3**: Description
  * **Feature 4**: Description

  \section usage Usage Example

  \code
  #include "path/to/class.hpp"

  toy::ClassName<Type, Size> obj(arg1);
  constexpr auto obj2 = toy::ClassName<Type, 32>("data");
  \endcode

  \section performance Performance Characteristics

  * **Construction**: O(n) where n is...
  * **Memory usage**: Fixed at compile time, no heap allocation

  \section safety Safety Guarantees

  * **Contracts**: Description
  * **Type safety**: Uses C++23 concepts
  * **Exception safety**: All operations are noexcept

  \section compatibility Compatibility

  * **C++ standard**: C++23 or later
  * **Cross-platform**: Supported on all target platforms
  * **Embedded systems**: Suitable (no dynamic allocation)

  \note Additional note, if necessary.

  \warning Important warning, if necessary.

  \sa \ref toy::namespace::RelatedClass
*/
template <typename Param1, size_t Param2>
class ClassName {
  // ...
};
```

### Struct

```cpp
/*!
  \struct StructName
  \brief Brief one-line description of the struct.

  Detailed description of purpose and data format.

  \section features Key Features

  * **Feature 1**: Description
  * **Feature 2**: Description
  * **Feature 3**: Description
  * **Feature 4**: Description

  \section usage Usage Example

  \code
  #include "path/to/struct.hpp"

  toy::namespace::StructName obj{value1, value2};
  constexpr auto obj2 = toy::namespace::StructName{1, 2};
  \endcode

  \section performance Performance Characteristics

  * **Construction**: O(1) constant time
  * **Access**: O(1) constant time
  * **Memory usage**: X bytes

  \section safety Safety Guarantees

  * **Type safety**: Description
  * **Exception safety**: All operations are noexcept
  * **Memory safety**: No dynamic allocation

  \note Additional note, if necessary.

  \sa \ref toy::namespace::RelatedType
*/
struct StructName {
  // ...
};
```

### Simplified Class (without compatibility)

```cpp
/*!
  \class ClassName
  \brief Brief one-line description.

  Detailed description. 2-4 sentences.

  \section features Key Features

  * **Constexpr support**: Most operations are constexpr
  * **Exception safety**: All operations are noexcept
  * **Optimized**: Designed for [specific application]
  * **Type safety**: Strong typing with clear [semantics]

  \section usage Usage Example

  \code
  #include "path/to/class.hpp"

  toy::namespace::ClassName obj(value1, value2);
  obj.method();
  \endcode

  \section performance Performance Characteristics

  * **Construction**: O(1) constant time
  * **Arithmetic**: O(1) constant time
  * **Memory usage**: X bytes

  \section safety Safety Guarantees

  * **Contracts**: Description of debug-mode checks
  * **Type safety**: Strong typing prevents [what it prevents]
  * **Exception safety**: All operations are noexcept

  \note Additional note, if necessary.

  \sa \ref toy::namespace::RelatedClass
*/
class ClassName {
  // ...
};
```

### Documentation Pre-Commit Checklist

* `\class` / `\struct` is present
* `\brief` is filled (one line)
* Detailed description is present (2-4 sentences)
* `\section features` contains 4+ items with **bold** names
* `\section usage` contains a compilable code example
* `\section performance` is filled with Big-O complexity
* `\section safety` is filled with specific guarantees
* `\section compatibility` is added if needed
* All methods have `\brief`, `\param` (aligned), `\return`, `\tparam`
* Constraints in `\pre`, not in `\param`
* State-changing methods have `\post`
* Constants wrapped with `\c`; parameter names referenced with `\a`
* `\sa` links related entities
* No marketing language

---

## Unit Test Style Rules

These rules define how unit tests must be written or generated by AI tools in this repository.

**When writing or reviewing unit tests**, ensure all of the following are applied (details in the sections below):

* **Assertions**: Every `static_assert` has a human-readable message — see **Assertions** (under Error Handling).
* **Naming**: A human-readable comment immediately before each `TEST_CASE` and each `SUBCASE` — see **Naming Tests**.
* **Order**: When both `REQUIRE` and `static_assert` are used for the same behavior, write `REQUIRE` first, then `static_assert` — see **Constexpr + Runtime Parity**.
* **Compile-time preference**: Use `static_assert` wherever the behavior can be verified at compile time — see **Compile-Time vs Runtime Testing**.

The goal is to keep tests:

* Short
* Deterministic
* Non-redundant
* Suitable for constrained and embedded platforms

---

## General Test Principles

* Tests must be minimal and focused.
* One test = one behavioral aspect.
* Prefer clarity over coverage inflation.
* Avoid large, monolithic test cases.

---

## Test Length and Structure

* Tests must be readable without scrolling.
* Avoid excessive setup.
* Avoid deeply nested sections.

If setup becomes complex, refactor the API or extract helpers.

---

## Compile-Time vs Runtime Testing

### Compile-Time (`constexpr`) Tests

* Prefer compile-time verification whenever possible.
* Use:
  * `static_assert` — **always with the two-argument form** and a human-readable message (see **Assertions** under Error Handling).
* Test:
  * Type traits
  * `constexpr` constructors
  * `constexpr` operators
  * Compile-time invariants

Compile-time tests are preferred for:

* Containers
* Math types
* Value semantics

---

### Runtime Tests

* Use runtime tests only when:
  * Behavior depends on runtime state
  * `constexpr` evaluation is not possible
* Runtime tests must be deterministic.

Avoid:

* Randomness
* Time-based behavior
* Platform-dependent assumptions

---

## Redundancy and Duplication

* Do not generate multiple tests for the same behavior.
* Avoid copy-paste test cases with minor variations.

Prefer:

* Parameterized tests
* Small helper functions
* Reusing a single test to cover multiple invariants

If two tests assert the same contract, keep only one.

---

## Value-Based Assertions

* Prefer value comparisons over implementation details.
* Do not test private or internal state unless explicitly intended.

Tests must survive refactoring without semantic changes.

---

## String length assertions (including UTF-8)

When asserting that a string’s `size()` or `length()` equals the byte length of a string literal (especially UTF-8 or other multi-byte encodings), do not use hardcoded numeric constants.

* Use a compile-time length from the same literal: `std::char_traits<char>::length("...")`.
* Example: `REQUIRE(str.size() == std::char_traits<char>::length("Привет мир"))` instead of `REQUIRE(str.size() == 19)`.
* This keeps tests robust (no manual byte counting, source encoding is the single source of truth) and readable (the literal shows the expected content).
* Include `<string>` when using `std::char_traits`.

---

## Constexpr + Runtime Parity

When applicable:

* Verify the same behavior at:
  * Compile time
  * Runtime

The observable result must be identical.

When both runtime assertions (\c REQUIRE or similar) and compile-time assertions (\c static_assert) are used for the same behavior, **write runtime assertions first, then \c static_assert**. The compile-time check is the closing verification.

---

## Naming Tests

* Test names must describe behavior, not implementation.
* Avoid redundant prefixes.
* Avoid repeating the tested type name unless necessary.
* `TEST_CASE` names must follow `<namespace>/<class>/<case>[_<variant>]`, where `<namespace>` is the class namespace stripped of the leading `toy::` prefix (e.g. `toy::log` → `log`, `toy::game::strategy` → `game::strategy`). If the class lives directly in `toy::`, the namespace segment is omitted entirely (e.g. `toy::Engine` → `engine/...`). Nested namespaces retain their structure after stripping `toy::` (e.g. `toy::chrono::Stopwatch` → `chrono/stop_watch/...`).
* `TEST_CASE` names use `lowercase_snake_case`.
* Human-readable descriptions go in a comment immediately before `TEST_CASE` or `SUBCASE`.
* Avoid a single `SUBCASE` inside a `TEST_CASE`.

---

## Floating-Point Rules

* Avoid floating-point tests unless explicitly required.
* If unavoidable:
  * Account for platform limitations
  * Avoid fragile equality checks

---

## Test Independence

* Tests must not depend on:
  * Execution order
  * Global mutable state
* Each test must be independently executable.

---

## Final Rule

If a behavior can be verified at compile time,
it must be verified at compile time.

Runtime tests exist only to cover what compile-time tests cannot.
