# Third party libraries

Please keep categories (`##` level) listed alphabetically and matching their respective folder names.

## ckdl

- Upstream: <https://github.com/tjol/ckdl>
- Version: main (c9c33fe64446287215e80705545139d92a48f829, 2026)
- License: MIT

Files extracted from upstream source:

- `include/` folder, minus the `dlg` subfolder
- `src/` folder
  - These files can be removed: `CMakeLists.txt`
- `COPYING`

## clownmdsdk

Overlay over the installed ClownMDSDK toolchain: a file here wins over the SDK's own copy (see
`src/platforms/md/CMakeLists.txt`).

Local files, MIT like the engine:

- `include/cstring`, `include/string.h`, `src/string.c`

Files extracted from GCC libstdc++:

- Upstream: <https://gcc.gnu.org/git/gcc.git>
- Version: releases/gcc-16.2.0 (78d4ac73dd391005b895a6148cd9831e28e1208b, 2026)
- License: GPL-3.0 with the GCC Runtime Library Exception (`LICENSES/COPYING3`, `LICENSES/COPYING.RUNTIME`)
- `libstdc++-v3/include/bits/binders.h` as `include/bits/binders.h`, unmodified; the freestanding libstdc++ in
  ClownMDSDK omits it, and `<ranges>` fails to compile without it
- Must match the GCC version of the SDK toolchain; update it together with ClownMDSDK

## nanobench

- Upstream: <https://github.com/martinus/nanobench>
- Version: main (7252da9d96bd0d71f508e9b2813ab1f57fdfaec1, 2026)
- License: MIT

Files extracted from upstream source:

- `src/include/nanobench.h`
- `LICENSE`

Local patches:

- 0001-msvc-support

## vulkan

- Upstream: <https://github.com/KhronosGroup/Vulkan-Headers>
- Version: v1.4.362 (ee2ec5fd83dafce291024683b50dc89219333076, 2026)
- License: Apache 2.0 / MIT

Files extracted from upstream source:

- `include/`
- `LICENSES/`

## zlib

- Upstream: <https://github.com/madler/zlib>
- Version: develop (98095f829266a1cab0360bc9b4164c81a71bda66, 2026)
- License: zlib

Files extracted from upstream source:

- All `.c` and `.h` files, except `gz*.c`, `infback.c`, `compress.c` and `uncompr.c`
- `LICENSE`
