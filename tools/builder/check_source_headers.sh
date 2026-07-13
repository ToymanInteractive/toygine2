#!/bin/bash

#-----------------------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Toyman Interactive
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
# documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to the following conditions :
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of
# the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
# WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#-----------------------------------------------------------------------------------------------------------------------

# Script to determine whether source files carry the correct license header.
# Exits with a non-zero exit code if any checked file is missing or has an incorrect header.
#
# It is assumed that this script is called from the project root directory.

set -e -o pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
HEADER_C_LIKE="$SCRIPT_DIR/license/header_c_like.txt"
HEADER_SCRIPT="$SCRIPT_DIR/license/header_script.txt"

# Pick the license template that matches a file's comment style, or empty when the file is not checked.
# Slash style: C / C++ / Objective-C sources. Hash style: shell and CMake scripts.
template_for() {
  case "$(basename "$1")" in
    CMakeLists.txt) echo "$HEADER_SCRIPT"; return ;;
  esac
  case "${1##*.}" in
    cpp | cc | c++ | cxx | c | h | hpp | inl | mm | m) echo "$HEADER_C_LIKE" ;;
    sh | cmake) echo "$HEADER_SCRIPT" ;;
    *) echo "" ;;
  esac
}

# Compare a file's leading comment block against a template, allowing any 4-digit year where the template has YYYY.
# Prints nothing and returns 0 on match; returns 1 otherwise.
header_matches() {
  local file="$1" template="$2"

  local -a expected candidate
  mapfile -t expected < "$template"

  # Normalize the file head: drop an optional shebang, drop leading blank lines, strip trailing CR.
  mapfile -t candidate < <(awk '
    { sub(/\r$/, "") }
    NR == 1 && /^#!/ { next }
    started == 0 && /^[[:space:]]*$/ { next }
    { started = 1; print }
  ' "$file")

  if [ "${#candidate[@]}" -lt "${#expected[@]}" ]; then
    return 1
  fi

  local i exp cand pre suf mid
  for i in "${!expected[@]}"; do
    exp="${expected[$i]}"
    cand="${candidate[$i]}"

    if [[ "$exp" == *YYYY* ]]; then
      pre="${exp%%YYYY*}"
      suf="${exp##*YYYY}"
      mid="${cand#"$pre"}"
      if [[ "$mid" == "$cand" ]]; then
        return 1  # prefix did not match
      fi
      mid="${mid%"$suf"}"
      if [[ ! "$mid" =~ ^[0-9]{4}$ ]]; then
        return 1
      fi
    elif [[ "$cand" != "$exp" ]]; then
      return 1
    fi
  done

  return 0
}

# Get all modified files in the current branch compared to base branch
FILES_TO_CHECK=$(git diff --name-only "$(git merge-base origin/main HEAD)"..HEAD \
                                            | (grep -v "^extern/" || true))

if [[ -z "$FILES_TO_CHECK" ]]; then
  echo "There is no source code to check for license headers."
  exit 0
fi

echo "Checking license headers for files in branch..."

HEADER_ERRORS=0

while IFS= read -r file; do
  if [ ! -f "$file" ]; then
    continue  # File may have been deleted
  fi

  template="$(template_for "$file")"
  if [[ -z "$template" ]]; then
    continue  # Not a source file we check
  fi

  if ! header_matches "$file" "$template"; then
    echo "Missing or incorrect license header: $file"
    HEADER_ERRORS=$((HEADER_ERRORS + 1))
  fi
done <<< "$FILES_TO_CHECK"

if [ $HEADER_ERRORS -eq 0 ]; then
  echo "All checked source files have a correct license header."
  exit 0
else
  echo "Found $HEADER_ERRORS file(s) with a missing or incorrect license header!"
  exit 1
fi
