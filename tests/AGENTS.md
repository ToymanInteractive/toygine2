# AI AGENTS rules for Tests

Rules for tests and benchmarks in this repository. Bold section names refer to the root [`AGENTS.md`](../AGENTS.md) unless the section appears in this file.

## Testing

Building, running, and splitting tests across targets. Test shape follows **Testing Best Practices** below, case names **Naming Tests**, and float comparisons **Floating-Point Rules** — tests that stay short, deterministic, non-redundant, and buildable on constrained and embedded targets.

* **Running tests:** Configure with a preset enabling `TOYGINE_BUILD_TESTS` (feature preset `with-tests`, folded into the named `<platform>-<type>` presets), build, then run CTest with `--output-on-failure`. CI never invokes a test binary directly — CTest owns discovery, timeouts, and reporting.
* **Unit tests:** DocTest, one file per public type at `tests/<module>/<name>.test.cpp`, linked against the module under test, never an unrelated production module (see **Project Structure**). Every test translation unit includes `tests/runner/toy_test.hpp`, never doctest directly: the shim picks doctest on desktop and the built-in runner on consoles. `TOYGINE_BUILD_TESTS` gates the dependency so consumers never pull it in (see Build-only dependencies).
* **Compile-time tests:** `static_assert` blocks live in the same `.test.cpp` and need no runner — a violation is a build failure. They are the default; a runtime case adds what they cannot reach, or asserts parity where a behavior is reachable both ways (see Compile-time coverage and Constexpr / runtime parity under **Testing Best Practices**).
* **Test seams, not test hooks:** Test through the public API only. Needing `friend`, an `#ifdef TESTING` branch, or a widened access modifier is a design problem — narrow the dependency instead (see Pure functions and narrow seams, Explicit context, no globals).
* **Headless by construction:** Simulation tests run with renderer, audio, and editor support off (see Optional subsystems). A system that cannot run headless is coupled to presentation and must be split (see Simulation / presentation split).
* **Determinism fixtures:** Replay a recorded input snapshot against a fixed seed and compare state to a stored golden — both determinism guard and regression test for the systems it drives (see Determinism, Input).
* **Frame-loop independence:** Drive simulation tests by explicit tick counts and fixed deltas, never wall-clock time or a real frame loop; a test that sleeps or reads the clock is non-deterministic by construction (see Frame loop and time).
* **Assertions:** DocTest `REQUIRE` when failure makes the rest of the case meaningless, `CHECK` when independent expectations should all report; `static_assert` with a human-readable message for compile-time invariants (see **Assertions**).
* **Benchmarks:** picobench, at `benchmarks/<module>/<name>.benchmark.cpp`, gated by `TOYGINE_BUILD_BENCHMARKS`. Measures a system against its declared per-frame budget (see Budgets and profiling); not a correctness test, never gates a merge.
* **Cross-target verification:** Runtime tests execute on desktop, where sanitizers exist (see Sanitizers). The same translation units compile for every console through the built-in runner, so compile-time tests fire there, and on Nintendo GBA the ROM runs under mGBA. Module tests are built twice on desktop, under DocTest and the built-in runner, and a divergence between the two shows as a differing verdict.
* **Report format:** The built-in runner prints TAP version 14 — a version line, one test point per case in registry order, the plan and a summary comment counting assertions, not cases. A case with subcases prints as a subtest with one test point per branch; a failed assertion carries its file, line, expression and info entries in the YAML block under its point; a duplicate case name ends the run with `Bail out!`. The `gba-*` test presets run CTest verbose, so a passing run still shows the report; the DocTest binary beside it keeps its own format.
* **Coverage:** `TOYGINE_TESTS_ENABLE_COVERAGE` instruments a test build; reports go to Codecov per `codecov.yml`. Coverage is a signal, never a target — tests written to raise it are the redundancy the style rules forbid (see One contract, one case under **Testing Best Practices**).

## Testing Best Practices

What a case may depend on and which seam it drives; placement, gating, and CI mechanics are the bullets above.

* **Arrange-Act-Assert:** Three visible steps, one behavioral aspect per case (Given-When-Then reads the same). Split by aspect, not by method — a case asserting unrelated contracts fails without naming which broke.
* **Readable without scrolling:** Minimal arrangement, no monolithic cases, no nested `SUBCASE` trees, no lone `SUBCASE` in a `TEST_CASE`. When setup outgrows the assertions, the API needs a narrower seam or the fixture a helper — never absorb it into the test (see Design from the call site).
* **Independent by construction:** No case depends on execution order, another case's residue, or global mutable state; each runs alone, the suite in any order (see Explicit context, no globals).
* **One contract, one case:** Two cases asserting the same contract collapse into one, a copy-paste variant becomes a parameterized case or a helper, and invariants of one contract share a single case.
* **Compile-time coverage:** Type traits, `constexpr` constructors and operators, and compile-time invariants go through two-argument `static_assert` (see **Assertions** under **Code Quality**); a runtime case adds what constant evaluation cannot reach.
* **Constexpr / runtime parity:** A behavior reachable both ways is asserted both ways and must produce the identical observable result; the runtime `CHECK` / `REQUIRE` comes first, the `static_assert` after it as the closing verification.
* **Unit level:** Value types, containers, math, allocators, handles, codecs — everything reachable without a running engine, where `static_assert` does most of the work.
* **System level:** Fabricate component storage, drive one system over an explicit tick count, assert the resulting data — never call sequences; a system's contract is the data it produces (see Component storage, Data flow over control flow).
* **Integration level:** Composition root, init, load a baked asset, tick, shut down in reverse — asserts wiring, not arithmetic. Keep few: the slowest, and the first to rot (see Composition root).
* **Fakes over mocks:** Exceptions and RTTI off, no hidden allocation — the mocking frameworks do not fit this build (see Language subset, Allocation policy). Hand-write fakes at the engine's virtual seams (null RHI backend, in-memory asset source, capturing log sink); a static seam takes a stub type argument (see Virtual seams where they earn it).
* **Golden data:** Compare baked output and replayed state byte-for-byte against a golden stored beside the test. Regenerating one is a reviewed commit with a stated cause, never a side effect of a failing run (see Determinism fixtures, Asset pipeline).
* **Allocation accounting:** Where a rule forbids allocation — frame loop, mixer callback, job body — assert it with a counting allocator; a stray `new` then fails a test, not a profiling session (see Frame allocators).
* **Budget guards stay out:** Wall-clock thresholds belong to benchmarks; a timing assertion fails on a loaded CI machine and says nothing about correctness (see Budgets and profiling).
* **Assert the observable contract:** Bind a case to what the public API returns, never to internal state or a layout the type may change; a refactor that preserves behavior leaves the test untouched (see Test seams, not test hooks).
* **Diagnose on failure:** Assert values, not a folded boolean, so the report prints what was produced; carry index or parameter context in a DocTest `INFO` so a parameterized failure names its case.
* **Expected values derive from the source:** Check a `size()` against `std::char_traits<char>::length("...")` on the same literal (needs `<string>`), never a hand-counted constant — the literal stays the single source of truth, multi-byte UTF-8 included.
* **A flaky test is a defect:** Fix or delete it — never retry, skip, or quarantine. Intermittence means the test reads unspecified state or the system is non-deterministic, both worse bugs than the test. Seed anything random explicitly; an unseeded failure is unreproducible (see Determinism).
* **Editor tests:** The editor does not follow the module layout — see [`editor/AGENTS.md`](../editor/AGENTS.md).

## Naming Tests

* Test names must describe behavior, not implementation.
* Avoid redundant prefixes.
* Avoid repeating the tested type name in the `<case>` segment — the `<class>` segment already carries it.
* `TEST_CASE` names must follow `<namespace>/<class>/<case>[_<variant>]`, where `<namespace>` is the class namespace stripped of the leading `toy::` prefix (e.g. `toy::log` → `log`, `toy::game::strategy` → `game::strategy`). If the class lives directly in `toy::`, the namespace segment is omitted entirely (e.g. `toy::Engine` → `engine/...`). Nested namespaces retain their structure after stripping `toy::` (e.g. `toy::chrono::Stopwatch` → `chrono/stop_watch/...`).
* Every slash-separated segment of a `TEST_CASE` name uses `lowercase_snake_case`.
* Human-readable descriptions go in a comment immediately before `TEST_CASE` or `SUBCASE`.

## Floating-Point Rules

* Avoid floating-point tests unless explicitly required.
* If unavoidable:
  * Account for platform limitations
  * Avoid fragile equality checks
