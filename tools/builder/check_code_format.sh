#!/bin/bash

#-------------------------------------------------------------------------------------------
# Copyright (c) 2025 by Toyman Interactive
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
#-------------------------------------------------------------------------------------------

# Script to determine whether the source code in the Pull Request is formatted correctly.
# Exits with a non-zero exit code if formatting is needed.
#
# It is assumed that this script is called from the project root directory.

set -e -o pipefail

FILES_TO_CHECK=$(git diff --name-only HEAD^ | (grep -E ".*\.(cpp|cc|c\+\+|cxx|c|h|hpp|inl|java|js)$" || true) \
                                            | (grep -v "^src/thirdparty/.*/.*" || true))

if [ -z "$FILES_TO_CHECK" ]; then
  echo "There is no source code to check the formatting."
  exit 0
fi

if FORMAT_DIFF=$(git diff -U0 HEAD^ -- $FILES_TO_CHECK | clang-format-diff -p1 -style=file) && [ -z "$FORMAT_DIFF" ]; then
  echo "All the source code in the PR is formatted correctly."
  exit 0
else
  echo "Formatting errors found!"
  echo "$FORMAT_DIFF"
  exit 1
fi
