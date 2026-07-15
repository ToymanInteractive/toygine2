#!/bin/bash

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
# THE SOFTWARE  IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
# PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
# FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#-----------------------------------------------------------------------------------------------------------------------

# Script to determine whether source files carry the correct license header.
# Exits with a non-zero exit code if any checked file is missing or has an incorrect header.
#
# It is assumed that this script is called from the project root directory.

set -e -o pipefail

# Collect candidate files. A newline-separated list may be passed as $1 (e.g. from
# `gh pr view ... --json files`); otherwise diff against the base ref.
if [[ -n "${1:-}" ]]; then
  [[ -f "$1" ]] || { echo "File list '$1' not found." >&2; exit 2; }
  RAW_FILES=$(<"$1")
else
  # BASE_REF must resolve, or we would silently check nothing and pass — fail loudly instead.
  BASE_REF="${BASE_REF:-origin/main}"
  git rev-parse --verify --quiet "${BASE_REF}^{commit}" >/dev/null \
    || { echo "Base ref '${BASE_REF}' not found. In CI, check out with fetch-depth: 0, pass a file list, or set BASE_REF." >&2; exit 2; }
  RAW_FILES=$(git diff --name-only "$(git merge-base "$BASE_REF" HEAD)"..HEAD)
fi

# Exclude vendored code under extern/; the per-file switch below decides which of the rest are checked.
FILES_TO_CHECK=$(grep -v "^extern/" <<< "$RAW_FILES" || true)

if [[ -z "$FILES_TO_CHECK" ]]; then
  echo "There is no source code to check for license headers."
  exit 0
fi

echo "Checking license headers for files in branch..."
echo "$FILES_TO_CHECK"

# License header templates, one per comment style, resolved relative to this script.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
HEADER_C_LIKE="$SCRIPT_DIR/license/header_c_like.txt"
HEADER_SCRIPT="$SCRIPT_DIR/license/header_script.txt"

HEADER_ERRORS=0

while IFS= read -r file; do
  if [ ! -f "$file" ]; then
    continue  # File may have been deleted
  fi

  # Pick the license template that matches the file's comment style, skipping files we do not check.
  # Slash style: C / C++ / Objective-C sources. Hash style: shell and CMake scripts.
  case "$file" in
    *.cpp | *.cc | *.c++ | *.cxx | *.c | *.h | *.hpp | *.inl | *.mm | *.m) template="$HEADER_C_LIKE" ;;
    *.sh | *.cmake | */CMakeLists.txt | CMakeLists.txt) template="$HEADER_SCRIPT" ;;
    *) continue ;;  # Not a source file we check
  esac

  # Read the expected header (template) and the file's leading comment block into arrays.
  expected=()
  candidate=()
  while IFS= read -r line || [ -n "$line" ]; do
    expected+=("$line")
  done < "$template"

  # Normalize the file head: drop an optional shebang, drop leading blank lines, strip trailing CR.
  while IFS= read -r line || [ -n "$line" ]; do
    candidate+=("$line")
  done < <(awk '
    { sub(/\r$/, "") }
    NR == 1 && /^#!/ { next }
    started == 0 && /^[[:space:]]*$/ { next }
    { started = 1; print }
  ' "$file")

  # Compare line by line, allowing any 4-digit year where the template has YYYY.
  header_ok=1
  if [ "${#candidate[@]}" -lt "${#expected[@]}" ]; then
    header_ok=0
  else
    for i in "${!expected[@]}"; do
      expectedLine="${expected[$i]}"
      actualLine="${candidate[$i]}"

      if [[ "$expectedLine" == *YYYY* ]]; then
        prefix="${expectedLine%%YYYY*}"
        suffix="${expectedLine##*YYYY}"
        middle="${actualLine#"$prefix"}"
        if [[ "$middle" == "$actualLine" ]]; then
          header_ok=0  # prefix did not match
          break
        fi
        middle="${middle%"$suffix"}"
        if [[ ! "$middle" =~ ^[0-9]{4}$ ]]; then
          header_ok=0
          break
        fi
      elif [[ "$actualLine" != "$expectedLine" ]]; then
        header_ok=0
        break
      fi
    done
  fi

  if [ "$header_ok" -eq 0 ]; then
    echo "Missing or incorrect license header: $file"
    HEADER_ERRORS=$((HEADER_ERRORS + 1))
  fi
done <<< "$FILES_TO_CHECK"

if [ "$HEADER_ERRORS" -eq 0 ]; then
  echo "All checked source files have a correct license header."
  exit 0
else
  echo "Found $HEADER_ERRORS file(s) with a missing or incorrect license header!"
  exit 1
fi
