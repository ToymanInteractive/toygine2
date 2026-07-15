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
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
# PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
# FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#-----------------------------------------------------------------------------------------------------------------------

# Script to determine whether the source code in the Pull Request is formatted correctly.
# Exits with a non-zero exit code if formatting is needed.
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

# Keep only C / C++ (and Java / JS) sources, excluding vendored code under extern/.
FILES_TO_CHECK=$( (grep -E "\.(cpp|cc|c\+\+|cxx|c|h|hpp|inl|mm|m|java|js)$" <<< "$RAW_FILES" || true) \
                | (grep -v "^extern/" || true) )

if [[ -z "$FILES_TO_CHECK" ]]; then
  echo "There is no source code to check the formatting."
  exit 0
fi

echo "Checking formatting for files in branch..."
echo "$FILES_TO_CHECK"

FORMAT_ERRORS=0

while IFS= read -r file; do
  if [ ! -f "$file" ]; then
    continue  # File may have been deleted
  fi

  # Check if the file requires formatting
  if ! clang-format --dry-run -Werror -style=file "$file" 2>/dev/null; then
    echo "Formatting needed: $file"
    FORMAT_ERRORS=$((FORMAT_ERRORS + 1))
  fi
done <<< "$FILES_TO_CHECK"

if [ $FORMAT_ERRORS -eq 0 ]; then
  echo "All source code is formatted correctly."
  exit 0
else
  echo "Found $FORMAT_ERRORS file(s) with formatting errors!"
  exit 1
fi
