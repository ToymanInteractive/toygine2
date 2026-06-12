# AI AGENTS rules for the ToyGine 2 editor

Extends and overrides the root `AGENTS.md` for everything under `editor/`. The editor is a Qt6 desktop app, not a library module. Where this file is silent, root rules apply.

## Layout: group by feature

No public API, so the library's `include/` vs `src/` split adds nothing. Each feature is one directory with its `.hpp` and `.cpp` side by side.

```txt
editor/src/
  main.cpp                  # entry point at the source-tree root
  main_window/
    main_window.hpp         # header + source co-located per feature
    main_window.cpp
```

* `src/` is the include base → includes are feature-qualified: `#include "main_window/main_window.hpp"`.
* Include guards are path-derived from `src/`: `EDITOR_SRC_MAIN_WINDOW_MAIN_WINDOW_HPP_`.
* Editor code lives in `toy::editor`.

## Qt

* UI classes derive from a Qt type with `Q_OBJECT`; list the header in `HDR_LIST` so AUTOMOC runs (a missing moc surfaces as an "undefined vtable" link error).
* Child widgets are owned via Qt parent/child — no smart pointers.
* The root "no exceptions / no RTTI" rule targets the engine; Qt uses both and the editor links it as-is. Keep your own logic free of exception-based control flow.
* moc output fails the strict `-Weverything -Werror` profile, so warnings are off for the generated `mocs_compilation.cpp` only (see `CMakeLists.txt`). Never relax warnings on hand-written code.

## Documentation

The library's full Doxygen template (`\section` blocks, usage examples, pre-commit checklist) is overkill here. Keep `\file` blocks and root tone rules; drop the rest.

* Classes / structs: `\brief` + at most 2–3 sentences. No `\section` blocks.
* Functions / methods / constructors / fields: one sentence, prefer a single `///` line.
* Add `\param` / `\return` only when the signature isn't self-explanatory. Document the non-obvious (ownership, lifetime, units, side effects), not what the types already say.

## Build

* Reconfigure after adding/moving/renaming files or changing include dirs: `cmake --preset <your-platform-preset>` (e.g. `macos-debug`, `linux-debug`, `windows-msvc` — see `CMakePresets.json`). A bare `cmake --build` won't pick up the change and fails with "file not found".
