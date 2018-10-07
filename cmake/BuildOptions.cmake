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

##
# Backend engine to use
##
set(TUNIS_BACKEND "GL" CACHE STRING "backend implementation to use")
set_property(CACHE TUNIS_BACKEND PROPERTY STRINGS GL NanoVG-GL3)

##
# Enable/Disable Samples
##
option(TUNIS_BUILD_SAMPLES "Build samples" ON)

if (TUNIS_BUILD_SAMPLES)

    ##
    # Samples window system
    ##
    set(TUNIS_SAMPLES_MAIN "SDL2" CACHE STRING "Main entry point implementation to usef for the samples")
    set_property(CACHE TUNIS_SAMPLES_MAIN PROPERTY STRINGS SDL2 GLFW3)

    ##
    # Enable/Disable multi-sampling anti-aliasing
    ##
    option(TUNIS_SAMPLES_MSAA "Enable sample Multi-Sampling Anti-Aliasing" ON)

endif()

##
# Enable/Disabe Profiling with easy_profiler
##
if(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    set(defaultProfile ON)
else()
    set(defaultProfile OFF)
endif()
option(TUNIS_PROFILING "Enable profiling with easy_profiler" ${defaultProfile})
unset(defaultProfile)


