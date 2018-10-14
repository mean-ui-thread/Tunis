#!/bin/bash

##
# MIT License
#
# Copyright (c) 2018 Matt Chiasson
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
##

ROOT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." >/dev/null && pwd )"

fail () {
    echo >&2 "$@"
    exit 1
}

[ "$#" -eq 1 ] || fail "One toolchain argument required, $# provided"
[ -f $ROOT_DIR/cmake/toolchains/$1 ] || fail "$ROOT_DIR/cmake/toolchains/$1 not found."

cmake -G Xcode -H$ROOT_DIR -B$ROOT_DIR/_build -DCMAKE_TOOLCHAIN_FILE=$ROOT_DIR/cmake/toolchains/$1
cmake --build $ROOT_DIR/_build

