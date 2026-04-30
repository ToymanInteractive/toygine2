#!/usr/bin/env python3
"""Convert nanobench JSON output to github-action-benchmark customSmallerIsBetter format.

Usage:
    python3 convert_nanobench.py <input.json> <output.json>

nanobench stores median(elapsed) in seconds; this script converts to nanoseconds.
The output name is "<title> / <name>" so results from different modules stay distinct.
"""

import json
import sys


def convert(input_path: str, output_path: str) -> None:
    with open(input_path, encoding="utf-8") as f:
        data = json.load(f)

    results = []
    for r in data.get("results", []):
        title = r.get("title", "")
        name = r.get("name", "")
        full_name = f"{title} / {name}" if title else name

        elapsed_sec = r.get("median(elapsed)", 0.0)
        value_ns = elapsed_sec * 1e9

        error_pct = r.get("medianAbsolutePercentError(elapsed)", 0.0)
        range_str = f"±{error_pct:.2f}%"

        results.append({
            "name": full_name,
            "unit": "ns",
            "value": round(value_ns, 4),
            "range": range_str,
        })

    with open(output_path, "w", encoding="utf-8") as f:
        json.dump(results, f, indent=2)

    print(f"Converted {len(results)} benchmarks → {output_path}")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <input.json> <output.json>", file=sys.stderr)
        sys.exit(1)
    convert(sys.argv[1], sys.argv[2])
