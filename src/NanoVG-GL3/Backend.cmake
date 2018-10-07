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

function(tunis_backend_dependencies)

    add_library(nanovg
        3rdparty/nanovg/src/fontstash.h
        3rdparty/nanovg/src/nanovg.c
        3rdparty/nanovg/src/nanovg.h
        3rdparty/nanovg/src/nanovg_gl.h
        3rdparty/nanovg/src/nanovg_gl_utils.h
        3rdparty/nanovg/src/stb_image.h
        3rdparty/nanovg/src/stb_truetype.h
    )

    target_include_directories(nanovg PUBLIC 3rdparty/nanovg/src)

    target_link_libraries(Tunis PUBLIC nanovg)

endfunction()
