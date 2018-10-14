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
find_program(XCRUN_EXECUTABLE xcrun)
find_program(XCODE_SELECT_EXECUTABLE xcode-select)

execute_process(
    COMMAND ${XCRUN_EXECUTABLE} --find "clang"
    OUTPUT_VARIABLE XCODE_COMPILER_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND ${XCRUN_EXECUTABLE} --show-sdk-version
    OUTPUT_VARIABLE OSX_SDK_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND ${XCODE_SELECT_EXECUTABLE} -print-path
    OUTPUT_VARIABLE XCODE_DEVELOPER_ROOT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(CMAKE_XCODE_ATTRIBUTE_CC "${XCODE_COMPILER_PATH}" CACHE STRING "Xcode Compiler" FORCE)
set(CMAKE_OSX_SYSROOT "${XCODE_DEVELOPER_ROOT}/Platforms/MacOSX.platform/Developer/SDKs/MacOSX${OSX_SDK_VERSION}.sdk" CACHE STRING "System root for OSX" FORCE)

