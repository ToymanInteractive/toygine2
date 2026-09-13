#-----------------------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Toyman Interactive
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this
# software and associated documentation files (the "Software"), to deal in the Software
# without restriction, including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and / or sell copies of the Software, and to permit
# persons to whom the Software is furnished to do so, subject to the following conditions :
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
# PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
# FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#-----------------------------------------------------------------------------------------------------------------------

"""Builds a Bencher Metric Format report from the CSV lines a benchmark run prints.

The runner prints integers only: one line per benchmark row, naming it once and following it with a pair per timed
epoch, the calls in that epoch and its length in timer ticks. Dividing, the median and the spread happen here, so a
target without a floating-point unit never has to do them.
"""

import argparse
import csv
import json
import statistics
import sys

CSV_PREFIX = "#csv "
HEADER = ["case", "row", "unit", "ps_per_tick", "epochs", "iters", "ticks"]
NAMED_COLUMNS = 5


def read_rows(stream):
    """Yields the CSV rows of a run log as dictionaries, skipping everything else."""
    # MSBuild indents the output of a custom target, so a CSV line does not always start at column one.
    stripped = (line.lstrip() for line in stream)
    lines = [line[len(CSV_PREFIX):] for line in stripped if line.startswith(CSV_PREFIX)]
    rows = list(csv.reader(lines))

    if not rows:
        raise SystemExit("no CSV lines in the input; was the runner called with --csv?")

    if rows[0] != HEADER:
        raise SystemExit(f"unexpected CSV header: {rows[0]}")

    if len(rows) == 1:
        raise SystemExit("no benchmark rows in the input; did every pattern match nothing?")

    for fields in rows[1:]:
        if len(fields) < NAMED_COLUMNS or (len(fields) - NAMED_COLUMNS) % 2 != 0:
            raise SystemExit(f"expected {NAMED_COLUMNS} columns and a pair per epoch, got {len(fields)}: {fields}")

        for value in fields[3:]:
            if not value.isdecimal():
                raise SystemExit(f"expected a whole number, got {value!r} in {fields}")

        pairs = [(int(fields[at]), int(fields[at + 1])) for at in range(NAMED_COLUMNS, len(fields), 2)]

        if len(pairs) != int(fields[4]):
            raise SystemExit(f"expected {fields[4]} epochs, got {len(pairs)}: {fields}")

        if any(iterations == 0 for iterations, _ in pairs):
            raise SystemExit(f"expected a positive iters, got 0: {fields}")

        yield {"case": fields[0], "row": fields[1], "ps_per_tick": int(fields[3]), "epochs": pairs}


def nanoseconds_per_call(iterations, ticks, picoseconds_per_tick):
    """Converts one epoch into nanoseconds per call."""
    return ticks * picoseconds_per_tick / iterations / 1000.0


def build_report(rows):
    """Groups epochs by benchmark and reduces each group to one latency measure."""
    epochs = {}

    for row in rows:
        values = epochs.setdefault(f"{row['case']}/{row['row']}", [])
        values.extend(nanoseconds_per_call(calls, ticks, row["ps_per_tick"]) for calls, ticks in row["epochs"])

    return {
        name: {
            "latency": {
                "value": statistics.median(values),
                "lower_value": min(values),
                "upper_value": max(values),
            }
        }
        for name, values in epochs.items()
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("log", nargs="?", help="run log to read; standard input when omitted")
    arguments = parser.parse_args()

    with open(arguments.log, encoding="utf-8") if arguments.log else sys.stdin as stream:
        report = build_report(read_rows(stream))

    json.dump(report, sys.stdout, indent=2, sort_keys=True)
    sys.stdout.write("\n")


if __name__ == "__main__":
    main()
