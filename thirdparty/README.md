# Third party libraries

Please keep categories (`##` level) listed alphabetically and matching their respective folder names.
Use two empty lines to separate categories for readability.

## ckdl

- Upstream: <https://github.com/tjol/ckdl>
- Version: main (c9c33fe64446287215e80705545139d92a48f829, 2026)
- License: MIT

Files extracted from upstream source:

- `include/` folder, minus the `dlg` subfolder
- `src/` folder
  - These files can be removed: `CMakeLists.txt`
- `COPYING`

## libpng

- Upstream: http://libpng.org/pub/png/libpng.html
- Version: 1.6.58 (3061454d980de7d53608f594194cfac722721d2a, 2026)
- License: libpng/zlib

Files extracted from upstream source:

- All `.c` and `.h` files of the main directory, apart from example.c and pngtest.c
- `arm/`, `intel/`, `loongarch/`, `mips/`, `powerpc/` and `riscv/` folders, except .editorconfig files
- `scripts/pnglibconf.h.prebuilt` as `pnglibconf.h`
- `CMakeLists.txt`
- `LICENSE`

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
