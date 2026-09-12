# AI AGENTS rules for Benchmarks

Rules for benchmarks in this repository. Bold section names refer to the root [`AGENTS.md`](../AGENTS.md) unless the section appears in this file.

## Benchmarks

Building, running, and gating. What a body measures follows **Benchmark Shape** below, case names **Naming Benchmarks**.

* **What a benchmark answers:** Measures a system against its declared per-frame budget (see Budgets and profiling); not a correctness test, never gates a merge. CI builds benchmarks and runs none — a result is a number a human reads.
* **Measured under optimization:** Numbers come from a `<platform>-release` preset, which compiles at `-O2`. The debug presets pass no `-O`, so an inline header-only type is measured uninlined and unvectorized, and the ratio can come out wrong in either direction. Debug builds check that a benchmark runs, never what it costs.
* **Library and layout:** nanobench, one file per public type at `benchmarks/<module>/<name>.benchmark.cpp`, gated by `TOYGINE_BUILD_BENCHMARKS` so engine consumers never pull the dependency in (see Build-only dependencies).
* **Desktop alone:** nanobench needs a steady host clock and iostreams, which no console toolchain here offers, so `benchmarks/CMakeLists.txt` skips the tree off desktop. A claim about console performance comes from the target, never from an emulator on the build host.
* **One binary:** every `*.benchmark.cpp` links into `<library>-benchmarks` beside the runner in `benchmarks/runner/`. A `file(GLOB_RECURSE ... CONFIGURE_DEPENDS)` in `benchmarks/CMakeLists.txt` picks up a new file, so adding one costs no CMake; `BENCHMARK_CASE` registers the cases inside it. `benchmarks_run` builds and runs the binary.
* **Selecting what runs:** a pattern runs every benchmark whose name carries it as a substring, and no pattern runs all. `--list` first prints each name with the file and line declaring it instead of running; patterns after it narrow that listing.
* **Machine-readable report:** `--csv` prints one `#csv`-prefixed line per timed epoch beside the tables — `case,row,unit,epoch,iters,ticks,ps_per_tick`, all integers, with the whole epoch in `ticks` and one picosecond per tick on a desktop. `--no-table` takes the stream away from nanobench, so a run gives the data alone. Medians, spreads and the Bencher report come from `tools/bmf_from_csv.py` on the host.
* **One entry point:** every benchmark translation unit includes `benchmarks/runner/toy_benchmark.hpp`, never nanobench directly — the registry lives there, and a file reaching past it registers nothing.
* **Deterministic order:** the registry sorts by name, so tables print in name order whatever order the linker gave the objects; no benchmark depends on running before or after another. A repeated name ends the run before the first table: the name both titles a table and selects it.

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
