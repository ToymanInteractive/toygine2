# AI AGENTS rules for Benchmarks

Rules for benchmarks in this repository. Bold section names refer to the root [`AGENTS.md`](../AGENTS.md) unless the section appears in this file.

## Benchmarks

Building, running, and gating. What a body measures follows **Benchmark Shape** below, case names **Naming Benchmarks**.

* **What a benchmark answers:** Measures a system against its declared per-frame budget (see Budgets and profiling); not a correctness test, never gates a merge. CI builds benchmarks and runs none — a result is a number a human reads.
* **Library and layout:** nanobench, one file per public type at `benchmarks/<module>/<name>.benchmark.cpp`, gated by `TOYGINE_BUILD_BENCHMARKS` so engine consumers never pull the dependency in (see Build-only dependencies).
* **Desktop alone:** nanobench needs a steady host clock and iostreams, which no console toolchain here offers, so `benchmarks/CMakeLists.txt` skips the tree off desktop. A claim about console performance comes from the target, never from an emulator on the build host.
* **One binary:** every `*.benchmark.cpp` links into `<library>-benchmarks` beside the runner in `benchmarks/runner/`. `BENCHMARK_CASE` is what joins a file to the build, so a new one costs no CMake; `benchmarks_run` builds the binary and runs it.
* **One entry point:** every benchmark translation unit includes `benchmarks/runner/toy_benchmark.hpp`, never nanobench directly — the registry lives there, and a file reaching past it registers nothing.
* **Deterministic order:** the registry sorts by name, so tables print in name order whatever order the linker gave the objects; no benchmark depends on running before or after another.

## Benchmark Shape

* **The runner owns the table:** title, ratio column and epoch length come from `benchmarks/runner/main.cpp`, which is what keeps two tables comparable; a body sets the unit and calls `run()` once per row.
* **One operation per case:** rows compare implementations of one operation. Two operations make the ratio column meaningless, so they are two cases.
* **A row needs something to measure against:** an engine type against its standard-library equivalent (`std::string_view` beside `toy::StringView`), or against the implementation it replaces — a lone number states neither a budget nor a regression.
* **Keep the compiler honest:** every measured result goes through `ankerl::nanobench::doNotOptimizeAway`, and a foldable input reaches the body through an opaque barrier. A benchmark over a folded constant still prints a number, which is worse than printing none.
* **Arrange outside the loop:** whatever is not the subject of the measurement is built before `run()`, so the timed callable holds the operation alone.
* **Data the cache can hold:** state the size a case works over and why. A working set that spills out of cache measures the memory system, not what the name promises.

## Naming Benchmarks

* `BENCHMARK_CASE` names follow `<namespace>/<class>/<operation>`, the shape `TEST_CASE` uses (see Naming Tests in [`tests/AGENTS.md`](../tests/AGENTS.md)): `toy::` stripped from the namespace, every slash-separated segment `lowercase_snake_case`.
* The name titles the table, so it reads alone in a report — `core/string_view/find`, never `find` or `benchmark_1`.
* Row names are the implementations compared, spelled the way the code spells them: `toy::StringView`, `std::string_view`.
