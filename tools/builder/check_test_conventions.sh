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

# Script to determine whether test files name the runner's context parameter, which only its macros may do.
# Exits with a non-zero exit code if any checked file names it.
#
# It is assumed that this script is called from the project root directory.

set -e -o pipefail

# BSD grep reports a missing path with the same 1 it uses for no match, so the scan cannot tell a wrong directory
# from a clean tree.
if [ ! -d tests ]; then
  echo "tests/ not found: run this script from the project root directory."
  exit 2
fi

# Only the macros name this parameter, so a test naming it by hand has taken the context into a helper, where a
# failed REQUIRE returns from the helper instead of ending the case. The shim's own tests do it by design.
SCAN_STATUS=0
CONTEXT_USES=$(grep -rlw 'toyTestContext' tests/ --include='*.test.cpp' --exclude-dir=runner) || SCAN_STATUS=$?

# 1 means nothing matched; anything higher means the scan failed and cannot count as a clean tree.
if [ "$SCAN_STATUS" -gt 1 ]; then
  echo "Cannot scan tests/ for the runner's context parameter: grep exited with $SCAN_STATUS."
  exit "$SCAN_STATUS"
fi

if [ -z "$CONTEXT_USES" ]; then
  echo "All test files leave the runner's context parameter to the macros."
  exit 0
fi

echo "These test files name the runner's context parameter, which only the macros may do:"
echo "$CONTEXT_USES"
exit 1
