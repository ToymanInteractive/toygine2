#-----------------------------------------------------------------------------------------------------------------------
# Copyright (c) 2026-7 Toyman Interactive
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

The runner prints integers only: whole epochs in timer ticks, the length of a tick in picoseconds and the number of
calls in the epoch. Dividing, the median and the spread happen here, so a target without a floating-point unit never
has to do them.
"""

import argparse
import csv
import json
import statistics
import sys

CSV_PREFIX = "#csv "
HEADER = ["case", "row", "unit", "epoch", "iters", "ticks", "ps_per_tick"]


def read_rows(stream):
    """Yields the CSV rows of a run log as dictionaries, skipping everything else."""
    lines = [line[len(CSV_PREFIX):] for line in stream if line.startswith(CSV_PREFIX)]
    rows = list(csv.reader(lines))

    if not rows:
        raise SystemExit("no CSV lines in the input; was the runner called with --csv?")

    if rows[0] != HEADER:
        raise SystemExit(f"unexpected CSV header: {rows[0]}")

    for fields in rows[1:]:
        if len(fields) != len(HEADER):
            raise SystemExit(f"expected {len(HEADER)} fields, got {len(fields)}: {fields}")

        row = dict(zip(HEADER, fields))

        for name in ("epoch", "iters", "ticks", "ps_per_tick"):
            if not row[name].isdigit():
                raise SystemExit(f"expected a whole number in {name}, got {row[name]!r}")

        if int(row["iters"]) == 0:
            raise SystemExit(f"expected a positive iters, got 0: {fields}")

        yield row


def nanoseconds_per_call(row):
    """Converts one epoch into nanoseconds per call."""
    return int(row["ticks"]) * int(row["ps_per_tick"]) / int(row["iters"]) / 1000.0


def build_report(rows):
    """Groups epochs by benchmark and reduces each group to one latency measure."""
    epochs = {}

    for row in rows:
        epochs.setdefault(f"{row['case']}/{row['row']}", []).append(nanoseconds_per_call(row))

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
