# Third party libraries

Please keep categories (`##` level) listed alphabetically and matching their respective folder names.

## agbabi

- Upstream: <https://github.com/felixjones/agbabi>
- Version: main (2adf1e6c01f9d4acd6f8e728892c5e18aae5332e, 2023)
- License: zlib, in `LICENSE.md`

Files extracted from upstream source:

- `include/agbabi.h`, unmodified
- `LICENSE.md`
- `source/rtc.c` as `src/rtc.c`

Local patch (marked in the file banner):

- `src/rtc.c`: strips the newlib POSIX glue (`_gettimeofday`, `settimeofday`, `bcd_decode`/`bcd_encode`, the `_gettimeofday_r` reentrancy wrapper) and switches the includes (`<reent.h>`/`<sys/time.h>`) to the relative header. The engine is freestanding: it must not override libc stubs or pull `mktime`/`gmtime` into the ROM

The engine links this target privately on GBA, which does not merge static archives: a consumer of an installed `libtoygine.a` links `libagbabi.a` alongside it, and both archives ship in the `library` component.

## ckdl

- Upstream: <https://github.com/tjol/ckdl>
- Version: main (c9c33fe64446287215e80705545139d92a48f829, 2026)
- License: MIT

Files extracted from upstream source:

- `include/` folder, except `dlg/`
- `src/` folder, except `utils/` (the CLI tools)
- `COPYING`

## clownmdsdk

Overlay over the installed ClownMDSDK toolchain: a file here wins over the SDK's own copy (see `src/platforms/md/CMakeLists.txt`).

Local files, MIT like the engine:

- `include/cstring`, `include/string.h`, `src/string.c`
- `src/string.c` also replaces the SDK's `memcpy`, `memmove` and `memset`: the SDK's `memmove` always copies front to back and corrupts an overlapping move to a higher address, and all three share one object in its `libc.a`, so defining `memmove` alone collides with the SDK's copy at link time
- `memcpy`, `memset` and `memcmp` in `src/string.c` are 68000 assembly that saves no registers; sizes of 64 KiB and more fall back to the C functions `_MemcpyLarge`, `_MemsetLarge` and `_MemcmpLarge`

Files extracted from GCC libstdc++:

- Upstream: <https://gcc.gnu.org/git/gcc.git>
- Version: releases/gcc-16.2.0 (78d4ac73dd391005b895a6148cd9831e28e1208b, 2026)
- License: GPL-3.0 with the GCC Runtime Library Exception (`LICENSES/COPYING3`, `LICENSES/COPYING.RUNTIME`)
- `libstdc++-v3/include/bits/binders.h` as `include/bits/binders.h`, unmodified; the freestanding libstdc++ in ClownMDSDK omits it, and `<ranges>` fails to compile without it
- `libstdc++-v3/include/std/chrono` as `include/chrono` and `libstdc++-v3/include/bits/chrono.h` as `include/bits/chrono.h`; the SDK omits both, and `toy::chrono` is `std::chrono` aliases, so `<chrono>` has to exist
- Local patch in `include/chrono`: `year_month_day::_S_from_days` casts `__m1` and `__d1` to `unsigned int` — under `-mshort` they deduce to `long unsigned int`, and the narrowing in the upstream brace-init fails `-Werror`
- Must match the GCC version of the SDK toolchain; update it together with ClownMDSDK

## cmake-modules

- Upstream: <https://github.com/bilke/cmake-modules>
- Version: master (5b988b5beb64270cf68b7d6c20298ebc8236b580, 2026)
- License: BSD-3-Clause, in the file header

Files extracted from upstream source:

- `CodeCoverage.cmake`

Local patches:

- 0001-lcov-version-suffix: the lcov version regex accepts a multi-digit package suffix and output without a trailing newline
- 0002-split-report-comments: the SonarQube report message gets its own `add_custom_command`; upstream passes two `COMMENT` arguments to one call

## doctest

- Upstream: <https://github.com/doctest/doctest>
- Version: v2.5.3 (2d0a9359a60c51affe2a9bebb1be1dca47868151, 2026)
- License: MIT

Files extracted from upstream source:

- `doctest/doctest.h`
- `scripts/cmake/doctest.cmake`
- `scripts/cmake/doctestAddTests.cmake`
- `LICENSE.txt`

## doxygen-awesome-css

- Upstream: <https://github.com/jothepro/doxygen-awesome-css>
- Version: v2.5.0 (46483f1e5a70ffb9ecd3b82d0a1cd1b24edf13da, 2026)
- License: MIT

Files extracted from upstream source:

- `doxygen-awesome*.css` and `doxygen-awesome*.js`
- `LICENSE`

## font8x8

- Upstream: <https://github.com/dhepper/font8x8>
- Version: master (8e279d2d864e79128e96188a6b9526cfa3fbfef9, 2026)
- License: Public Domain

Files extracted from upstream source:

- `font8x8_basic.h`
- `README`

`font8x8_basic.h` defines the glyph array in the header; include it from a single translation unit. Bits read least-significant first: pixel n of a row is `(byte >> n) & 1`, n = 0 is the leftmost pixel.

Local patch (marked in the file):

- `font8x8_basic.h`: the array is trimmed to U+0020..U+007E (index 0 = space), the range the console backend prints, and its type is `unsigned char`, not `char` — under the MD toolchain's `-fsigned-char` the `0xFF` glyph bytes narrow in the braced initializer and fail `-Werror`

## libpng

- Upstream: <https://github.com/pnggroup/libpng>
- Version: v1.6.59 (cd952f49f95bb27154ae77dbb103032d95f6e580, 2026)
- License: libpng-2.0

Files extracted from upstream source:

- `png*.c` and `png*.h`, except `pngtest.c`
- `scripts/pnglibconf.h.prebuilt` as `pnglibconf.h`, unmodified; nothing regenerates it here
- `arm/` folder, except `filter_neon.S`: the NEON path uses the intrinsics
- `intel/` folder
- `AUTHORS` and `LICENSE`

The `loongarch/`, `mips/`, `powerpc/` and `riscv/` filters are left out: the editor targets x86 and ARM only.

## nanobench

- Upstream: <https://github.com/martinus/nanobench>
- Version: main (7252da9d96bd0d71f508e9b2813ab1f57fdfaec1, 2026)
- License: MIT

Files extracted from upstream source:

- `src/include/nanobench.h`
- `LICENSE`

Local patches:

- 0001-msvc-support

## volk

- Upstream: <https://github.com/zeux/volk>
- Version: master, 1.4.365 (0dc3ce00bf98b9f0b6fe708ca0f7eb74e2830173, 2026)
- License: MIT

Files extracted from upstream source:

- `volk.c` and `volk.h`
- `LICENSE.md`

`VOLK_HEADER_VERSION` in `volk.h` must match `VK_HEADER_VERSION` in the `vulkan` headers; update both together.

## vulkan

- Upstream: <https://github.com/KhronosGroup/Vulkan-Headers>
- Version: v1.4.365 (c46850864f4661461b0f6cb9922c058ffea4915e, 2026)
- License: Apache 2.0 / MIT

Files extracted from upstream source:

- `include/`
- `LICENSES/`

## zlib

- Upstream: <https://github.com/madler/zlib>
- Version: develop (767c4c947852e143f582c85f14cf573411df1b35, 2026)
- License: zlib

Files extracted from upstream source:

- All `.c` and `.h` files, except `gz*.c`, `infback.c`, `compress.c` and `uncompr.c`
- `LICENSE`
