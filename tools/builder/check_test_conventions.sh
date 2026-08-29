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

# The runner passes its context as a parameter the macros name. A test naming it by hand is bypassing the
# guard that keeps REQUIRE out of helper functions.
CONTEXT_USES=$(grep -rln 'toyTestContext' tests/ --include='*.test.cpp' | grep -v '^tests/runner/' || true)

if [ -z "$CONTEXT_USES" ]; then
  echo "All test files leave the runner's context parameter to the macros."
  exit 0
fi

echo "These test files name the runner's context parameter, which only the macros may do:"
echo "$CONTEXT_USES"
exit 1
