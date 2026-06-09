# ToyGine2

ToyGine2 is a modern C++ high‑level engine.

![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/ToymanInteractive/toygine2/push.yaml?branch=main&logo=github&logoColor=fff&label=Build)
[![GitHub License](https://img.shields.io/github/license/ToymanInteractive/toygine2?logo=github&logoColor=fff&label=License)](https://github.com/ToymanInteractive/toygine2/blob/main/LICENSE)
[![GitHub Release](https://img.shields.io/github/v/release/ToymanInteractive/toygine2?logo=github&logoColor=fff&label=Release&sort=semver)](https://github.com/ToymanInteractive/toygine2/releases/latest)
[![Sonar Tech Gate](https://img.shields.io/sonar/tech_debt/ToymanInteractive_toygine2?logo=sonarqubecloud&logoColor=fff&label=Tech%20Debt&server=https%3A%2F%2Fsonarcloud.io)](https://sonarcloud.io/dashboard?id=ToymanInteractive_toygine2)
[![Sonar Bugs](https://img.shields.io/sonar/bugs/ToymanInteractive_toygine2?logo=sonarqubecloud&logoColor=fff&label=Bugs&server=https%3A%2F%2Fsonarcloud.io)](https://sonarcloud.io/dashboard?id=ToymanInteractive_toygine2)
[![Sonar Code Smells](https://img.shields.io/sonar/code_smells/ToymanInteractive_toygine2?logo=sonarqubecloud&logoColor=fff&label=Code%20Smells&server=https%3A%2F%2Fsonarcloud.io)](https://sonarcloud.io/dashboard?id=ToymanInteractive_toygine2)
[![Sonar Duplicated Lines (%)](https://img.shields.io/sonar/duplicated_lines_density/ToymanInteractive_toygine2?logo=sonarqubecloud&logoColor=fff&label=Duplicated%20Lines&server=https%3A%2F%2Fsonarcloud.io)](https://sonarcloud.io/dashboard?id=ToymanInteractive_toygine2)
[![Code Coverage](https://img.shields.io/codecov/c/github/ToymanInteractive/toygine2?logo=codecov&logoColor=fff&flag=units&label=Code%20Coverage)](https://app.codecov.io/gh/ToymanInteractive/toygine2?flags%5B0%5D=units)
[![Documentation Coverage](https://img.shields.io/codecov/c/github/ToymanInteractive/toygine2?logo=codecov&logoColor=fff&flag=documentation&label=Doxygen%20Coverage)](https://app.codecov.io/gh/ToymanInteractive/toygine2?flags%5B0%5D=documentation)
![CodeRabbit Pull Request Reviews](https://img.shields.io/coderabbit/prs/github/ToymanInteractive/toygine2?logo=coderabbit&logoColor=fff&label=CodeRabbit%20Reviews)

## Supported Systems

- Windows
- macOS
- Linux

## Requirements

`ToyGine2` requires a C++23-compliant compiler. The following tools and compilers are supported:

### Supported C++ compilers

| OS      | Toolchain               | Version                          |
| ------- | ----------------------- | -------------------------------- |
| Windows | Microsoft Visual Studio | MSVC 2022 17.4 and above         |
| macOS   | Xcode                   | 26.5 and above (Apple Clang 21+) |
| Linux   | GCC                     | 13.3 and above                   |
| GBA     | devkitARM               | release 66 (GCC 15.2.0)          |

### Minimum OS versions

| OS      | Version                 | Reason                                                  |
| ------- | ----------------------- | ------------------------------------------------------- |
| Windows | 2000 Professional       | `QueryPerformanceFrequency` / `QueryPerformanceCounter` |
| macOS   | 10.12 (Sierra) or later | `clock_gettime`                                         |
| Linux   | 2.6.28 or later         | `clock_gettime`                                         |

### Tools

- CMake 3.31 or newer
- Doxygen 1.16+ and Graphviz (for documentation)
- ClangFormat 22

## Building

The project is built with [CMake presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html) (schema v10, CMake 3.31+). List everything available:

```sh
cmake --list-presets
```

Configure, build, and run tests:

```sh
cmake --preset linux-release
cmake --build --preset linux-release
ctest --preset linux-release
```

### Preset naming

Presets follow `<platform>[-<generator>]-<tier>`.

- **Generator:** [Ninja](https://ninja-build.org/) is the default and is omitted from the name. A generator suffix appears only when it differs from the default: `-xcode` (Xcode), `windows-msvc` (Visual Studio).
- **Tier:**
  - `debug` — `Debug` build type, developer tools (tests, samples, benchmarks) enabled.
  - `release` — `RelWithDebInfo` (optimized, with debug info), developer tools enabled.
  - `shipping` — `Release` (optimized, no debug info), developer tools disabled. Lean distributable build.
  - `tests-coverage` — Debug build with coverage instrumentation (Linux). Special-purpose preset used by CI/CD for coverage reporting; not intended for local development.

### Platforms

| Platform          | Presets                                                      | Generator             |
| ----------------- | ------------------------------------------------------------ | --------------------- |
| Linux             | `linux-debug`, `linux-release`, `linux-shipping`             | Ninja                 |
| macOS             | `macos-debug`, `macos-release`, `macos-shipping`             | Ninja                 |
| macOS (Xcode)     | `macos-xcode`                                                | Xcode                 |
| Windows           | `windows-msvc`, `windows-msvc-x86`                           | Visual Studio 18 2026 |
| Nintendo consoles | `gba-*`, `nds-*`, `3ds-*`, `switch-*`, `gamecube-*`, `wii-*` | Ninja                 |

Console presets require [devkitPro](https://devkitpro.org/) with the `DEVKITPRO` environment variable set.

Multi-config generators (Xcode, Visual Studio) select the build type at build time, so they have no dedicated `shipping` preset — build the `Release` configuration explicitly, e.g. `cmake --build --preset macos-xcode --config Release`.
