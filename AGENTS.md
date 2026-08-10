# AI AGENTS rules for This Repository

This document defines **mandatory rules** for AI‑assisted code, test, and documentation generation in this repository.

All AI tools (Cursor, Copilot, ChatGPT, etc.) **must follow these rules** when generating or modifying code, tests, or documentation.

You are an expert in GameDev and C++ development. Your goal is to build performant, maintainable, and extensible game engine following modern C++ best practices (C++23 baseline). You have expert experience with game architecture, engine internals, and real-time systems, with shipping titles to production, and with writing, testing, and running C++ applications on desktop (Windows, macOS 10.12+, Linux), retro and modern consoles, mobile, embedded and web.

## Interaction Guidelines

* **User Persona:** Familiar with programming, but may be new to modern C++ (C++23) and real-time engine architecture.
* **Explanations:** Explain the C++ features code relies on (RAII, move semantics, `constexpr`/`consteval`, concepts, rule of zero/five) and flag any hidden cost (allocation, virtual dispatch, exceptions).
* **Clarification:** When ambiguous, ask about intent, target platform (retro/modern console, desktop, mobile, web/WASM), and hot-path vs. cold/tooling path — they trade off differently.
* **Platform Awareness:** State toolchain assumptions (compiler, C++ standard, fixed vs. dynamic memory). Never assume an OS, heap, exceptions, or RTTI; retro and embedded targets may lack all four.
* **Allocation:** Ask before adding heap allocation on a hot path or in a fixed-capacity type; name its pool and capacity — see Allocation policy.
* **Dependencies:** Justify a new library's cost (build time, size, portability) and prefer CMake `FetchContent` — see **Dependency Management**.
* **Tooling:** Format, lint, and build warning-free before committing — see **Lint Rules**.
* **Testing:** Prefer compile-time `static_assert`; use DocTest-style runtime tests only for runtime-dependent behavior — see [`tests/AGENTS.md`](tests/AGENTS.md).

## Project Structure

A C++ game engine split into named modules. Each module mirrors one layout across four trees — `src/<module>/` (`.cpp`), `include/<module>/` (`.hpp` + `.inl` inline and template bodies), `tests/<module>/` (`<name>.test.cpp`), `benchmarks/<module>/` (`<name>.benchmark.cpp`). Per-module umbrella `include/<module>.hpp`, precompilable; root umbrella `include/toygine.hpp` re-exports all modules. Consumers include only a module or the root umbrella, never internal headers — implementation details outside the public API (see **API Design Principles**).

Non-module directories:

* `src/platforms/` — platform-specific code and per-target `platform_config.hpp`, selected by the build for the active target.
* `tests/` and `benchmarks/` — test and benchmark rules live beside them — read [`tests/AGENTS.md`](tests/AGENTS.md) first.
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
* **Allocation policy:** No dynamic allocation by default; every heap allocation explicit and justified, never hidden. Types offer fixed-capacity (stack or static) storage alongside heap where required, and a counting allocator proves allocation-free paths in tests (see Allocation and failure are visible under **API Design Principles**, Allocation accounting in [`tests/AGENTS.md`](tests/AGENTS.md)).
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

* **Naming:** Intent-revealing, no abbreviations except domain terms (`rgba`, `aabb`). `PascalCase` for types and template parameters (descriptive, no single-letter names outside trivial scopes); `camelCase` for functions and variables; `snake_case` for namespaces and files; `snake_case` + `_type` for aliases. Constants: `camelCase` with `c_` (namespace/file/`static`) or leading `_` (`private` only, never namespace/file); a function-local `const`/`constexpr` may drop the prefix. Private members lead with `_`; public and protected never do. STL-like methods use standard-library names, others `camelCase`. Const references as `const T &`, not `T const &`. Include guards uppercase from the full file path (`INCLUDE_CORE_FIXED_STRING_HPP_`).
* **Conciseness:** Code should read without comments; every construct earns its place in correctness, performance, or clarity — avoid needless abstraction.
* **Simplicity:** Straightforward over clever; prefer the obvious solution.
* **Error handling:** Signal failure via return values or `expected`-like types; assert invariants with `assert_message` (runtime) and `static_assert` (compile-time), both with human-readable messages. Never fail silently.
* **Functions:** Short and single-purpose; ~40 lines is a soft target. Split by responsibility, not length.
* **Performance:** Correctness first; optimize only with justification and measurement, and document non-obvious low-level choices.
* **Styling:** 2-space indent (no tabs), 120-column max, no trailing whitespace, attached braces, middle-aligned `type * pointer` / `type & reference` / `const type * constPointer`, break before binary operators, ≤1 blank line between sections and none opening a block, access-modifier labels offset −2 from the class body — enforced by `.clang-format` (see **Lint Rules**); declare them `public`, `protected`, `private`.
* **Logging:** Use engine macros `LOG_TRACE`/`LOG_DEBUG`/`LOG_INFO`/`LOG_WARN`/`LOG_ERROR` (via `toy::log`), never `printf`, `std::cout`, or `std::print`. Levels below `LOG_MAX_LEVEL` compile out — zero-cost on constrained targets.

### Assertions

* **Form:** `assert_message` over plain `assert`, always two-argument — the message must make a failure readable without opening the source.
* **Runtime message:** what was expected, or why the condition must hold.
* **Compile-time message:** the same for `static_assert`, stating the invariant in human terms — `static_assert(length == expected, "length must match the literal's UTF-8 byte count")`.

## C++23 Best Practices

* **Pointer / Null Safety:** References and values over raw pointers, `std::optional` for optional values; no unchecked dereferences or raw owning pointers (see Explicit resource lifetime).
* **Pattern Matching:** Dispatch with structured bindings, `if constexpr`, and `std::visit` over `std::variant`.
* **Aggregates and Records:** Group or return related values as aggregate `struct`s with designated initializers; `std::tuple` / `std::pair` only when a named struct is cumbersome.
* **Switch Statements:** Exhaustive `switch` over enumerations, no `default` — new enumerators then surface as warnings.
* **Lambdas:** Short local callables, captured explicitly; no `std::function` on hot paths (see Zero-cost abstractions).
* **Modern spellings:** `using` over `typedef`; range-based for over index loops; `auto` when the type is obvious or verbose, never when it hides a non-evident type.
* **`= default` / `= delete`:** Default special members explicitly; delete unwanted copy or move.
* **Rule of Zero / Five:** Own no resource → declare none of the five; declare or delete one → declare all five (`= default`, `= delete`, or a body), never relying on implicit deletion. Owners state ownership — move-only or non-movable — and declare the destructor. Exceptions: aggregates and trivially copyable data declare none (declaring any costs designated initializers and trivial copyability); polymorphic bases suppress copy and move behind a public virtual or protected non-virtual destructor.
* **Initialization:** In-class initializers for defaults, constructor init lists for non-default values; `{}` for variables, `= value` for literal `constexpr` / `const` constants, `()` where `{}` picks the wrong overload (`std::vector<int> v(10)` — ten elements, not one of value 10).
* **Views over pointer pairs:** `std::span` / `std::string_view` (`std::mdspan` for multidimensional data) instead of pointer + length; they carry the extent and stay `constexpr`-friendly. Take views as parameters; store one only over storage that outlives it by construction (static-duration data or the object's own buffer), never over caller-supplied data — long-lived resources use handles, see Explicit resource lifetime.
* **Monadic error flow:** Chain fallible steps with `[[nodiscard]]` `std::expected` and `and_then` / `transform` / `or_else` instead of nested `if` checks; `value_or` only where discarding the error is deliberate — see Error handling under **Code Quality**.
* **Deducing `this`:** An explicit object parameter (`auto && at(this auto && self, size_t index)`) collapses `const` / non-`const` overload pairs and replaces CRTP in mixins.
* **Enum interop:** `std::to_underlying` instead of casts; bit flags as scoped enums with the engine's bitwise-operator macro, never raw integers.
* **Bit-level operations:** `std::bit_cast`, `std::byteswap`, `std::rotl` / `std::rotr`, `std::popcount`, `std::countl_zero` — `constexpr` and portable — instead of `memcpy` punning, unions, or intrinsics.
* **Compile-time branching:** `if consteval` over `std::is_constant_evaluated()`; both branches must produce identical results.
* **Operator conveniences:** `static operator()` / `static operator[]` for stateless callables; multidimensional `operator[](x, y)` over `operator()(x, y)` for grids, matrices, and tile maps.
* **Ranges and views:** `std::views` (`zip`, `enumerate`, `chunk`, `stride`, `filter`) over manual index bookkeeping — lazy and allocation-free, but materialize into contiguous storage before repeated hot-path iteration.
* **Flat containers:** `std::flat_map` / `std::flat_set` over node-based `std::map` / `std::set` — contiguous, no per-node allocation; check each target's standard library first.
* **Optimizer hints:** `[[likely]]` / `[[unlikely]]` only on measured branches; `std::unreachable()` and `[[assume]]` only where `assert_message` checks the same invariant in debug — violating them is undefined behavior.
* **Concurrency primitives:** Parallel work goes through the engine job system; `std::jthread` with `std::stop_token` for long-lived tooling threads only; coroutines only with measured or elided frame allocation; `std::atomic` always with an explicit memory order — `seq_cst` by default, weaker only with a comment.
* **Formatting:** `std::format` for tooling, load time, and error paths; runtime diagnostics use the engine log macros (see Logging) — never `std::print` or iostreams.
* **Concepts over SFINAE:** Constrain templates with concepts and `requires`, never `std::enable_if` or tag dispatch; prefer standard concepts (`std::integral`, `std::ranges::contiguous_range`) and constrained `auto` parameters — failures must read as unmet requirements (see **Concept Documentation**).
* **Diagnostics context:** A defaulted `std::source_location` parameter instead of `__FILE__` / `__LINE__` in assertion and logging helpers — survives inlining, `constexpr`-evaluable, keeps the helper a function (see **Assertions**).
* **Portability of new features:** `import std;` is unavailable on console and embedded toolchains — include headers; guard non-universal library features with their `__cpp_lib_*` macro and provide a fallback.

## Game Engine Development Best Practices

Runtime architecture of the engine: the frame, the data, and the platform. Language-level rules live under **C++ style guide** and **C++23 Best Practices**.

* **Frame loop and time:** Fixed-timestep simulation driven by an accumulator; cap the frame delta and substep count and drop the surplus, so a slow frame cannot spiral into catch-up. Presentation interpolates between the last two states; simulation time, wall-clock time, and time scale stay distinct — never derive gameplay from a variable delta or frame count.
* **Determinism:** Same inputs and seed must yield the same simulation on every run and target — the basis for replays, rollback netcode, and reproducible bugs. Use fixed-point where float divergence across targets matters; never depend on unordered-container order, pointer values, or uninitialized memory.
* **Explicit update phases:** Systems run in named phases (input → simulation → animation → physics resolve → extraction → render submit). Order is declared data, not an artifact of construction or registration order.
* **Component storage:** Contiguous homogeneous arrays keyed by entity handle, iterated whole per system. Add and remove through deferred command buffers applied at a phase boundary, so nothing mutates the storage it iterates.
* **Handles with generations:** Handles pack index plus generation, so a stale one is a detected error, not a dangling dereference into a recycled slot (see Explicit resource lifetime).
* **Frame allocators:** Per-frame data from a bump arena reset at the frame boundary, long-lived data from fixed pools. No heap allocation, blocking I/O, syscalls, or locks inside the frame loop (see Allocation policy).
* **Fixed capacity limits:** Entity, draw-call, particle, voice, and job counts are capped at compile time or by configuration; exceeding a cap is a diagnosed error, never silent growth or a reallocation spike.
* **Job system:** Pure tasks over disjoint ranges with dependencies as a graph — no per-job allocation, no blocking waits inside a job, no shared mutable state without an explicitly ordered atomic or a phase barrier (see Concurrency primitives).
* **Renderer boundary:** Graphics APIs sit behind a thin RHI; passes record command buffers from an immutable snapshot extracted from the simulation. No gameplay type reaches a backend header, and no backend type reaches gameplay.
* **Asset pipeline:** Bake offline into a target-ready binary form; the runtime loads and uses, never parses text or converts formats. Each format carries magic and version, and the loader checks version, sizes, offsets, and alignment before use — cheap validation only.
* **Resource loading:** Loads are asynchronous and return handles immediately. A missing or failed asset yields a visible placeholder plus `LOG_ERROR` — never a crash, never a silent skip.
* **Binary data portability:** Fix the serialized byte order and convert with `std::byteswap` on load; never `reinterpret_cast` a struct over a file buffer — retro and console targets differ in endianness and enforce alignment (see Bit-level operations).
* **Platform abstraction:** Platform code lives in `src/platforms/` behind an interface the build selects per target; no `#ifdef` platform branches in engine or gameplay code — the seam is a file, not a macro (see **Project Structure**).
* **Input:** The platform layer buffers raw events into one immutable snapshot per tick. Gameplay reads the snapshot and never polls a platform API, so replays and headless tests can feed recorded input.
* **Audio:** The mixer callback is real-time — no locks, allocation, I/O, or logging. Use one lock-free SPSC queue per producer, drained by the mixer, plus preallocated voice pools; streaming decode runs on a worker.
* **Math conventions:** Fix one convention — handedness, row- vs column-major, radians, up axis, unit scale — document it at the module boundary, and convert only at external API edges.
* **Budgets and profiling:** Each system states a per-frame budget in milliseconds and bytes and carries scoped profiler markers; optimize measured frame time against it, never a guess (see Performance under **Code Quality**).
* **Data-driven tuning:** Tunables live in data, not literals; hot reload is a dev-build feature.
* **Dev-only tooling:** Overlays, gizmos, validation layers, and console commands sit behind build flags and compile out of shipping builds.

## API Design Principles

The engine is consumed as a library — by gameplay code, samples, the editor, and its own future versions. A public header is a contract. Language rules live under **C++ style guide** and **C++23 Best Practices**; where the files themselves live, under **Project Structure**.

* **Design from the call site:** Write the sample or test first and shape the API around it. Correct use must be the shortest thing to write; misuse the types can express fails to compile, the rest is a documented `\pre` with `assert_message` (see Make invalid states unrepresentable).
* **Minimal surface:** Expose the smallest set of types and functions that solves the problem, preferring non-member non-friend functions; keep helpers internal. Adding later is cheap, removing breaks consumers.
* **The signature is the contract:** Ownership, mutability, optionality, and lifetime read from the declaration alone — value, `const T &`, `std::span`, `std::optional`, or a handle (see Explicit resource lifetime). No parameter's meaning depends on another's value.
* **Strong types at the boundary:** Scoped enums, named aggregates, and unit-bearing types over `bool`, `int`, or bare floats — `setFilter(TextureFilter::Linear)`, not `setFilter(true)`.
* **Explicit context, no globals:** Dependencies arrive as parameters or a context object; two instances must coexist in one process (editor, tests, headless tools).
* **Allocation and failure are visible:** Whoever allocates says so in the signature — caller storage, explicit capacity, or a named pool; fallible calls return `[[nodiscard]] std::expected` (see Allocation policy and Error handling under **Code Quality**).
* **Batch-oriented entry points:** Take ranges or spans and process a system per call; per-entity calls across a module boundary defeat contiguous storage and the job system.
* **Two layers, not one:** The hot-path API stays explicit and allocation-free; convenience wrappers for tooling sit on top of it, never inside it.
* **Consistency and orthogonality:** One argument order, naming, unit, and error convention across modules, fixed at the module boundary (see Math conventions).
* **Compile-time contracts:** State requirements as concepts and `static_assert` messages, so misuse reads as an unmet requirement, not a template instantiation dump.
* **Stability and deprecation:** Evolve additively; `[[deprecated("use X instead")]]` with a named replacement before removal. Serialized enumerator values are contract — never renumbered or reused; serialized data carries a versioned schema, never a raw struct layout (see Binary data portability).
* **Cheap to include:** Every consumer pays a public header's build cost — minimal includes and forward declarations. A `.inl` arrives through the barrel too — moving weight there organizes it, not removes it.
* **Self-contained headers:** A public header compiles on its own, independent of include order; internal headers include each other freely.
* **Bodies in `.inl`:** Template, `constexpr`, and inline definitions live in a `.inl`, never as a non-trivial body in the public header. A `.inl` is module interface, not public API: it adds no symbol the header lacks, exactly one header includes it, and consumers never do (see Barrel include policy under **Cross-References**).
* **Documented and demonstrated:** A Doxygen block per **Documentation** on every public symbol, plus a compilable example under `samples/` — the sample is the API's ergonomics test.

## Engine Architecture

Static structure: layers, modules, and where dependencies may point. Directory layout lives under **Project Structure**, frame-time behavior under **Game Engine Development Best Practices**.

* **One-way layers:** platform → core (types, memory, containers, log, math) → services (jobs, file, asset, input, audio, RHI) → systems (scene, renderer, physics, animation) → gameplay → tools. A layer sees only those below it; an upward include is a design error, not a forward-declaration problem.
* **Acyclic modules:** each module declares its dependencies in CMake and links only those. Break a cycle by moving the shared type down a layer or inverting it behind an interface — never a mutual include or a shared "utils" bucket.
* **Enforced by the build:** dependency direction is CMake target visibility — `PRIVATE` by default, `PUBLIC` only for what a public header exposes — so violations fail the build, not review.
* **Engine is a library:** no `main`, window, or argument parsing. The application (sample, editor, test runner, headless tool) owns entry, the window, and the frame loop, and drives engine systems.
* **Composition root:** subsystems are built once at startup in one place, wired by explicit parameters or a context aggregate, and shut down in reverse order; failed init returns `std::expected`, never a half-built subsystem (see Explicit context, no globals).
* **Data flow over control flow:** systems communicate through component storage, immutable snapshots, and queues drained at a phase boundary — never by calling into each other or via callbacks fired inside another system's update.
* **Events as data:** typed, fixed-capacity queues produced in one phase and consumed in the next; no global observer registry, no `std::function` subscriber lists (see Zero-cost abstractions).
* **Virtual seams where they earn it:** runtime polymorphism only at slow, replaceable boundaries — RHI backend, log sink, asset source, platform — crossed once per batch or frame; everything else resolves statically (see Platform abstraction).
* **Feature modules:** a feature owning its own types and systems becomes a module across all four trees, not a subfolder inside another module; single-user helpers stay internal.
* **Optional subsystems:** renderer, audio, editor support, and dev tooling are CMake options; a headless configuration must build and run the simulation with them off — what keeps tests and the asset pipeline viable (see Determinism).
* **Configuration flows down:** each subsystem takes capacities, budgets, paths, and backend choice as an aggregate at init, never reading globals, environment, or files from inside (see Data-driven tuning).
* **Tools share runtime code:** baker, importers, and editor link the same engine modules instead of reimplementing formats, math, or serialization; tool-only code sits behind build flags or in `editor/` (see Dev-only tooling).

## Lint Rules

Style and correctness are enforced by tools, not by review. Configs live at the repo root and CI runs the same checks on changed files; a change is ready when all of them pass clean.

* **Formatting:** `.clang-format` is the sole authority on layout — run it before committing, never hand-format against it. `// clang-format off` only where alignment carries meaning, with a comment saying why.
* **Include order:** `.clang-format` regroups into blank-line-separated blocks — corresponding header (in a `.cpp`), standard library `<...>`, third-party, project `"..."`.
* **Static analysis:** checks and options live in `.clang-tidy`; run it over the compile database before committing and review whatever `--fix` changed. Suppress with `// NOLINTNEXTLINE(check)` plus a reason — never a bare `// NOLINT`, never file-wide.
* **Warnings:** build at the toolchain's warning level and leave none; CI treats them as errors. Fix the code, not the diagnostic — `[[maybe_unused]]` for a deliberately unused parameter.
* **Language subset:** exceptions and RTTI are off in the build, so `throw`, `dynamic_cast`, and `typeid` fail to compile rather than fail review (see Zero-cost abstractions).
* **Sanitizers:** desktop debug and CI runs enable address, undefined-behavior, and thread sanitizers; a report fails the run like a failed assertion. Console targets have none — hence the same tests on desktop.
* **Every toolchain:** warnings stay clean on all target compilers in CI, not just the local one — console GCC diagnoses alignment and narrowing that Clang accepts, and vice versa (see Portability under **C++ style guide**).
* **License headers:** every source file starts with the block from `tools/builder/license`, verified in CI; what follows it is **Comment Placement**.
* **Docs build:** Doxygen must finish with no warnings — a broken `\ref`, a missing `\param`, or an undocumented public symbol is a lint failure.
* **Markdown:** `markdownlint-cli2` over `**/*.md`.
* **Out of scope:** `extern/` and `_deps/` are never formatted, linted, or auto-fixed; a vendored change is a recorded patch (see **Dependency Management**).
* **Sweeps land alone:** a reformat or `--fix` run is its own commit, never mixed into a feature diff.

## Testing

Tests and benchmarks have their own rules — read [`tests/AGENTS.md`](tests/AGENTS.md) first. It covers running and gating
tests, test shape, `TEST_CASE` naming, and floating-point comparisons.

## Documentation

Doxygen is the only documentation system here: this section covers what a block says, where it goes, how it's written, and which tag carries each fact, down to the ready-made blocks under **Class Documentation Templates** and the closing **Documentation Pre-Commit Checklist**.

### Documentation Philosophy

* **Document the contract, not the implementation:** State what a caller may rely on — ownership, lifetime, allocation, units, failure — so the block survives a rewrite of the body; one retelling the algorithm rots on the first refactor.
* **Comment intent, not mechanics:** Comments inside a function say why, the code says what (see Conciseness under **Code Quality**).
* **Write for the reader who is looking:** Put the answer where the question arises — the header of the type involved, not a commit message.
* **One term per concept:** Reuse the identifier's own vocabulary across headers, samples, and prose; a `Handle` is never a "reference" or an "id" (see Naming under **Code Quality**).
* **Documentation is code:** Behavior and its block change in the same commit; the docs build fails CI like a compiler warning (see Docs build under **Lint Rules**).
* **Say it once, then link:** A fact lives in the one block closest to what it constrains, every other mention being a `\ref` or `\sa` — duplicates drift apart and leave the reader guessing which the code obeys (see **See-Also Tags**, **Cross-References**).
* **Never document an unverified claim:** Read a `\pre`, a complexity, a thread-safety or determinism claim out of the code, never off the name — an invented guarantee is worse than a missing one, because callers build on it.
* **Stale is worse than absent:** A block contradicting the code misleads; a missing one costs only a reading of the signature. Delete a wrong claim that cannot be fixed in the same commit.
* **Match the block to the symbol's weight:** A trivial accessor earns a one-line `///`, a type owning a resource, a capacity, or a unit convention the full template — effort goes where a caller can get it wrong (see **Class Documentation Templates**).
* **Document the boundary, not the interior:** A block belongs in the public header, where a consumer meets the symbol; internal reasoning reaches neither the consumer nor the generated site (see **Comment Placement**).

### Comment Placement

* **Immediately before the declaration**, above attributes and the template head — `[[nodiscard]]`, `[[deprecated]]`, and `template <...>` sit between the block and the symbol.
* **Nothing in between:** no blank line, separator, or preprocessor branch — a detached block documents the wrong symbol or none, and an undocumented public symbol fails the docs build (see Docs build under **Lint Rules**).
* **`\file` opens the file:** after the license header, before the include guard — the one block documenting no symbol (see **File documentation (`\file`)**, License headers under **Lint Rules**).
* **One block per symbol:** on the declaration in the header; a second on the definition in a `.cpp` or `.inl` competes with it, so notes there stay ordinary `//` comments. A symbol first declared in a `.cpp` — internal linkage — carries its own.
* **Never on a forward declaration:** the block belongs where the type is defined (see Cheap to include under **API Design Principles**).
* **Inside the class body:** above the member, at member indentation; access-modifier labels carry none (see Styling under **Code Quality**).
* **Pairs get one block:** getter/setter or `const` / non-`const` overloads — document one, point to the other with `\sa` (see **See-Also Tags**).
* **Overloads that differ get their own:** one shared contract, one block; a different precondition, cost, or failure mode earns its own — that difference is what a reader came for.
* **Enumerators and aggregate members** take a trailing `///<`, which documents what precedes it on the line and never opens one; the enum or struct still carries a block above it.
* **Macros:** a block directly above the `#define`; `\def` only where it cannot sit there (a macro is never a `\ref` target — see **Cross-References**).
* **One block per namespace:** the `\namespace` description lives in the module barrel (`core.hpp` carries `\namespace toy`); Doxygen merges duplicates silently, so a second one elsewhere is invisible until it wins.

### Doxygen Block Style

* Use `/*! ... */` for multi-line documentation blocks. Start `/*!` on its own line; align `*/` with `/*!`.
* Use `///` for single-line documentation — for symbols whose whole contract fits one sentence (see Match the block to the symbol's weight under **Documentation Philosophy**).
* Use 2 spaces for indentation inside documentation blocks.
* Never use `//!` or `/**`; a bare `//` comment is an implementation note, invisible to Doxygen, and never a substitute for a block.
* Placement of every form follows **Comment Placement**.

### Documentation Tone

All documentation must be:

* **Concise** — no filler words or redundant phrasing.
* **Neutral** — factual descriptions, no subjective claims.
* **Technical** — precise, uses correct terminology.
* **No marketing language** — avoid "lightweight", "blazing-fast", "powerful", "cutting-edge", etc.

### Writing Style

* **Bullets over paragraphs** for independent facts; concision, neutrality, and terminology follow **Documentation Tone**.
* **One-sentence `\brief`, then a blank line:** it must read alone in an index. The detailed description never repeats it — 1-2 sentences on a function, 2-4 on a type, none when the `\brief` suffices.
* **No filler openings:** not "This function …", "A class that …", "Used to …" — start with the verb or the noun.
* **Doxygen commands over raw markup:** Markdown sparingly, never HTML — the XML output feeds the documentation site. Which command marks what is fixed by **Parameter Documentation** and **Cross-References**.
* **Mark up by kind, not by emphasis:** `\a` parameters, `\c` literals, `\ref` non-function symbols, plain `toy::function()` for functions (see **Parameter Documentation**, **Cross-References**). **Bold** only for the lead term of a `\section` bullet; section bodies are plain text.
* **Present tense, active voice:** "Returns the element count", not "Will return" or "The count is returned"; a `\param` is a noun phrase, not a sentence about the caller.
* **Do not restate the signature:** a block says what the declaration cannot. Never open a `\brief` with the symbol's name — in a `\file` block, with the file name the `\file` tag already carries — nor spell out a visible type.
* **One fact per tag:** one line per `\note`, `\warning` only for what breaks a caller, 2-3 `\sa` links at most (see **Notes and Warnings**, **See-Also Tags**).
* **No abbreviations** beyond the domain terms the code uses (see Naming under **Code Quality**).
* **Examples compile:** a `\section usage` block builds as written, lifted from a sample or a test, never from memory (see Documented and demonstrated under **API Design Principles**).

### What to Document

Every public symbol carries a block (see Documented and demonstrated), every header a `\file` block (see **File documentation (`\file`)**). Beyond purpose and usage, state what the signature cannot:

* **Ownership and lifetime:** who owns the resource, how long a handle stays valid, what a stale one does (see Explicit resource lifetime, Handles with generations).
* **Allocation:** whether the call allocates, from which pool or caller storage, against which fixed capacity (see Allocation policy).
* **Failure:** the error values a returned `std::expected` carries, the `\pre` an `assert_message` enforces in debug, what a violation does in a shipping build. Exceptions are off — never document throwing (see Error handling under **Code Quality**).
* **State change:** what the object holds after a mutating call; every such method carries a `\post`, and its absence claims nothing observable changed (see **Preconditions and Postconditions**).
* **Special and default values:** what a sentinel means — `\ref npos`, an empty `std::optional`, a bare `\c false` — and which behavior a parameter's default selects (see **Return Value Documentation**, **Parameter Documentation**).
* **Type requirements:** the concept a constrained template parameter satisfies and what it guarantees the caller — a `requires` clause is contract, not implementation (see **Template Parameters**, **Concept Documentation**).
* **Real-time and thread safety:** whether the call is legal from the mixer callback, a job body, or the frame loop, and which phase may issue it (see Job system, Audio).
* **Determinism:** whether the result is identical across runs and targets — replay, rollback, and golden data need this stated, not inferred (see Determinism under **Game Engine Development Best Practices**).
* **Units and conventions:** radians or degrees, handedness, byte order, coordinate space, time base — at the module boundary, again on any function departing from it (see Math conventions).
* **Platform availability:** targets where the symbol is absent, degraded, or compiled out by a build option (see Optional subsystems).
* **How it is reached:** the barrel a consumer includes, spelled in the `\section usage` example; an internal header or `.inl` says it arrives through that barrel and is never included directly (see Barrel include policy under **Cross-References**).
* **Invalidation:** which operations invalidate an outstanding handle, index, iterator, or `std::span` into the object's storage and which keep them valid — a caller holding a view across a mutating call has no other way to know (see Views over pointer pairs).
* **Compile-time use:** whether the symbol works in a constant expression, and which paths force runtime evaluation — `constexpr` states intent, not reachability (see Compile-time first).
* **Stability:** what a `[[deprecated]]` symbol is replaced by, and which values are frozen contract — a serialized enumerator or schema version does not read off the declaration (see Stability and deprecation under **API Design Principles**).
* **Non-obvious cost:** a complexity, a fixed byte footprint, or a low-level trick earns a sentence saying why; a trivial accessor earns none (see Performance under **Code Quality**).

### File documentation (`\file`)

Every header file (`.hpp` and `.inl`) carries a `\file` block; a translation unit (`.cpp`) carries one when it provides a non-trivial implementation or when navigation clarity is needed. Where the block sits is **Comment Placement**.

* **`\file`** — file name only, as it appears under `include/` or `src/` (e.g. `window_show_state.hpp`), not a full path.
* **`\brief`** — one line: what this file *is*, the role of the translation unit (see Do not restate the signature under **Writing Style**).
* **`.hpp`** (including internal headers under `include/`): after `\brief`, add one short paragraph (often starting with **Defines `\ref ...`:**) naming the primary type(s) or enum(s) and how they are used (call sites, platform API, etc.).
* **`.inl`** — keep the `\brief` short: **Inline implementations for `\ref` …** plus a narrow scope (e.g. “constructors and accessors”, “comparison operators”). Add the **`\note Included by …`** line exactly as in the template below, spelling the barrel per Barrel include policy under **Cross-References**.
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

### Method / Function Documentation Order

Every function, constructor, and operator block carries a `\param` per parameter and a `\return` where one is returned.

Always follow this order:

1. `\brief` — one-line summary
2. Detailed description — optional; length and wording per **Writing Style**
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
2. `\brief` — one-line summary
3. Detailed description — optional; length and wording per **Writing Style**
4. `\tparam` — template parameters (if applicable)
5. `\section features Key Features`
6. `\section usage Usage Example` — code block
7. `\section performance Performance Characteristics` — Big-O complexity
8. `\section safety Safety Guarantees`
9. `\section compatibility Compatibility` — platform/standard (optional)
10. `\note` — notes
11. `\warning` — warnings (if needed)
12. `\sa` — related classes/types

What each `\section` contains is **Class Sections Detail**; how its bullets are marked up is **Writing Style**.

### Class Documentation Templates

The class block below is the base form; a template class and a struct are stated as deltas from it, not as copies. A concept carries its own block, where `\section requirements` replaces `\tparam`. Tag order is fixed by **Class / Struct Documentation Order**, section content by **Class Sections Detail**.

#### Class

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
  #include "module.hpp"

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
  * **Exception safety**: No operation throws; exceptions are off in the build

  \note Additional note, if necessary.

  \sa \ref toy::namespace::RelatedClass
*/
class ClassName {
  // ...
};
```

Add `\section compatibility` only where the type has a special requirement (see **`\section compatibility Compatibility`**). For value types — math, handles, fixed-capacity containers — the features usually read **Constexpr support**, **Type safety**, **Exception safety**, and one naming what the type is optimized for.

#### Template Class

The class block, with these deltas:

* `\tparam` per template parameter, between the detailed description and `\section features`, naming the concept a constrained parameter satisfies (see **Template Parameters**).
* `\section usage` instantiates explicitly and shows the constant-expression form:

  ```cpp
  toy::ClassName<Type, Size> obj(arg1);
  constexpr auto obj2 = toy::ClassName<Type, 32>("data");
  ```

* `\section performance` states the type's actual memory model — fixed at compile time and heap-free only where that is the contract; `\section safety` carries **Type safety**: uses C++23 concepts.
* `\section compatibility` where the parameters constrain use — C++ standard, cross-platform support, embedded suitability where the type allocates nothing dynamically.
* `\warning` after `\note` where a parameter choice can be got wrong (see **Notes and Warnings**).

#### Struct

The class block, with these deltas:

* `\struct` instead of `\class`; the detailed description covers purpose and data format.
* `\section usage` constructs by aggregate initialization, at runtime and in a constant expression:

  ```cpp
  toy::namespace::StructName obj{value1, value2};
  constexpr auto obj2 = toy::namespace::StructName{1, 2};
  ```

* `\section performance` states **Access** alongside **Construction**; `\section safety` reads **Type safety**, **Exception safety**, and a **Memory safety** entry stating the type's actual allocation model (see Allocation under **What to Document**).

#### Concept

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

`\section requirements` replaces `\tparam` where it already describes the parameters in full; the rest of the rules are **Concept Documentation**.

#### Class Sections Detail

* **`\section features Key Features`** — 4-8 items, most important first; each names one capability a caller can act on, not a member list restated.
* **`\section usage Usage Example`** — the `#include` a consumer actually writes (module barrel or root umbrella, never an internal header — see **Project Structure**), then the call sequence end to end: construction, the operation, what the caller does with the result. Provenance and compilability follow Examples compile under **Writing Style**.
* **`\section performance Performance Characteristics`** — Big-O complexity for key operations, memory usage where it matters.
* **`\section safety Safety Guarantees`** — contracts, bounds safety, type safety, allocation behavior; which invariants `assert_message` checks in debug and what a violation does in a shipping build. **Exception safety** is a fixed entry: no operation throws, exceptions being off in the build; name an operation `noexcept` only where the declaration carries the specifier (see Failure under **What to Document**, Language subset under **Lint Rules**).
* **`\section compatibility Compatibility`** (optional) — only where the type has a special requirement: C++ standard, STL integration, cross-platform reach, embedded suitability.

### Concept Documentation

* Use `\concept ConceptName` so Doxygen treats the block as concept documentation.
* `\brief` — one-line description of when the concept is satisfied (e.g. "Concept satisfied when \a T is an instantiation of \ref toy::namespace::Type").
* Add a short paragraph on purpose and typical use (e.g. constraining template parameters, analogy to standard concepts) — none when the `\brief` suffices.
* Use `\section requirements Requirements` with a bullet list of conditions that must hold; it is the single source of truth for the template parameters, so omit `\tparam` when it already describes them fully.
* Never use Doxygen's `\requirement` command here — it registers the label `requirements` and collides with `\section requirements`, which fails the docs build (see Docs build).
* `\sa` — link to related types or concepts that use or are used with this concept (see **See-Also Tags**).
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
* Include type constraints and valid value ranges.
* Name the concept a constrained parameter must satisfy; a `requires` clause is part of the contract, not an implementation detail.
* Concepts themselves are the one exception to the first rule — see **Concept Documentation**.

### Notes and Warnings

* `\note`: one line per note. Split complex notes into multiple `\note` tags.
* `\warning`: use only for critical information (lifetime, ownership, misuse).

### See-Also Tags

* Use `\sa` to link related methods, operators, classes, and concepts.
* Add a blank line immediately before the `\sa` tag in documentation blocks.
* Link between: method and its operator equivalent, const/non-const overloads, method synonyms (`size()` / `length()`), related classes.
* 2-3 references maximum per entity.
* `\ref` applies as everywhere else — see **Cross-References**. Specific to `\sa`: an unqualified method name (`\sa size()`, `\sa operator==()`) is allowed only when the target is a member of the surrounding class; otherwise qualify it (`\sa toy::format()`).

### Cross-References

* When referencing types, classes, or other non-function symbols from the codebase in documentation, **always** use the Doxygen `\ref` tag with full namespace qualification — `\ref toy::math::Vector2`, never `\ref Vector2`. Do not rely on Doxygen auto-linking to resolve an unqualified name.
* Use `\ref` **only** for non-function symbols (classes, types, enums, namespaces, concepts):
  * Class/struct names: `\ref toy::ClassName`
  * Type aliases: `\ref toy::TypeAlias`
  * Enum values: `\ref toy::EnumName::Value`
  * Namespaces: `\ref toy::namespaceName`
  * Concepts: `\ref toy::ConceptName`
* **Do not use `\ref` for functions or methods** anywhere, `\sa` included (see **See-Also Tags**). Reference functions and methods using plain text:
  * `toy::functionName()` (not `\ref toy::functionName()`)
  * `methodName()` (not `\ref methodName()`)

**Exceptions**:

* Do not use `\ref` for standard library symbols (`std::string`, `std::vector`, `std::fwrite`) or for external URLs.
* Do not use `\ref` for macros and preprocessor identifiers (`LOG_MIN_LEVEL`, `ENABLE_BITWISE_OPERATORS`, `assert_message`).
* **Barrel include policy** (`.inl` and internal `.hpp` `\file` blocks): the line `\note Included by <barrel>.hpp only; do not include this file directly.` must use the real barrel filename as **plain text** (or `\c <barrel>.hpp` if monospace helps). **Do not** write `\ref core.hpp` (or similar) there—the filename is not a documented symbol; `\ref` is reserved for types, enums, namespaces, and concepts as above.

### Documentation Pre-Commit Checklist

Each line is a pointer to the rule that defines it — check the block against the section, not against this list.

* Type block: `\class` / `\struct`, `\brief`, a detailed description where the `\brief` does not suffice, and the sections in order — **Class / Struct Documentation Order**, **Class Sections Detail**.
* Concept block: `\concept`, `\brief`, and `\section requirements` where it replaces `\tparam` — **Concept Documentation**.
* Member blocks: tags present and in order, none omitted — **Method / Function Documentation Order**.
* Contracts placed correctly: constraints in `\pre`, state changes in `\post` — **Preconditions and Postconditions**.
* Beyond-signature facts stated where they apply: ownership, allocation, failure, determinism, units, invalidation — **What to Document**.
* Markup: `\c` for literals, `\a` for parameters, `\ref` only for non-function symbols — Mark up by kind under **Writing Style**, **Parameter Documentation**, **Cross-References**.
* Wording: concise, neutral, technical, no marketing language — **Documentation Tone**, **Writing Style**.
* `\file` block present in every `.hpp` and `.inl` — **File documentation (`\file`)**.
* Docs build passes with no warnings — the checklist's only mechanical gate (see Docs build under **Lint Rules**).
