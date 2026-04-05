//
// Copyright (c) 2025-2026 Toyman Interactive
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and / or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
/*!
  \file   main.cpp
  \brief  Entry point for nanobench benchmarks (core, geometry, math).

  Accepts an optional \c --json \c \<path\> argument to write results as JSON.
  When omitted results are only printed to stdout (default nanobench output).
*/

#include <nanobench.h>

#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

ankerl::nanobench::Bench runCoreBenchmarks() noexcept;
ankerl::nanobench::Bench runGeometryBenchmarks() noexcept;
ankerl::nanobench::Bench runMathBenchmarks() noexcept;

/*!
  \brief Extracts the JSON array body from a nanobench JSON render.

  nanobench renders \c {"results":[...]}. This helper strips the outer wrapper
  so results from multiple \c Bench instances can be merged into one array.

  \param bench The bench to render.
  \return The content between the outermost \c [ and \c ] (may be empty).
*/
static std::string extractJsonResults(const ankerl::nanobench::Bench & bench) {
  std::ostringstream ss;
  ankerl::nanobench::render(ankerl::nanobench::templates::json(), bench, ss);
  const std::string json = ss.str();
  const auto start = json.find('[');
  const auto end = json.rfind(']');
  if (start == std::string::npos || end == std::string::npos || end <= start) {
    return {};
  }
  return json.substr(start + 1, end - start - 1);
}

int main(int argc, char * argv[]) {
  const char * jsonPath = nullptr;
  for (int i = 1; i + 1 < argc; ++i) {
    if (std::strcmp(argv[i], "--json") == 0) {
      jsonPath = argv[i + 1];
      break;
    }
  }

  auto core = runCoreBenchmarks();
  auto geometry = runGeometryBenchmarks();
  auto math = runMathBenchmarks();

  if (jsonPath != nullptr) {
    std::ofstream out(jsonPath);
    out << "{\"results\":[";
    out << extractJsonResults(core);
    out << ',';
    out << extractJsonResults(geometry);
    out << ',';
    out << extractJsonResults(math);
    out << "]}";
  }

  return 0;
}
