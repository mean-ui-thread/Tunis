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
cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(Hunter/HunterGate)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.30.tar.gz"
    SHA1 "c062b0a2316cf8dcb57b6a9ad49fea1025a928f6"
    LOCAL
)

project(Tunis)

include(BuildOptions)

file(GLOB_RECURSE BACKEND_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/${TUNIS_BACKEND}/*)

add_library(Tunis
    LICENSE
    README.md

    cmake/BuildOptions.cmake
    cmake/Hunter/config.cmake

    include/Tunis.h
    include/Tunis.inl
    include/TunisBitMask.h
    include/TunisColor.h
    include/TunisContextState.h
    include/TunisGL.h
    include/TunisGradient.h
    include/TunisGradient.inl
    include/TunisImage.h
    include/TunisImage.inl
    include/TunisMath.h
    include/TunisPaint.h
    include/TunisPaint.inl
    include/TunisPath2D.h
    include/TunisPath2D.inl
    include/TunisRect.h
    include/TunisRect.inl
    include/TunisRenderState.h
    include/TunisSOA.h
    include/TunisSOA.inl
    include/TunisSize.h
    include/TunisSize.inl
    include/TunisSpinLock.h
    include/TunisTypes.h
    include/TunisVertex.h
    src/TunisColor.cpp
    ${BACKEND_SRC}

    # fast-poly2tri
    3rdparty/fast-poly2tri/MPE_fastpoly2tri.h

    # StructureOfArrays from https://github.com/Lunarsong/StructureOfArrays
    3rdparty/StructureOfArrays/include/mapped_soa.h
    3rdparty/StructureOfArrays/include/soa.h

    # concurrent queue
    3rdparty/concurrentqueue/concurrentqueue.h
)

target_include_directories(Tunis
    PUBLIC include
    PUBLIC include/${TUNIS_BACKEND}
    PUBLIC 3rdparty/StructureOfArrays/include
    PUBLIC 3rdparty/fast-poly2tri
    PUBLIC 3rdparty/concurrentqueue
    PRIVATE src
    PRIVATE src/${TUNIS_BACKEND}
)

hunter_add_package(glm)
hunter_add_package(easy_profiler)
hunter_add_package(msdfgen)
hunter_add_package(stb)

find_package(glm CONFIG REQUIRED)
find_package(easy_profiler CONFIG REQUIRED)
find_package(stb CONFIG REQUIRED)
find_package(msdfgen CONFIG REQUIRED)
find_package(OpenMP REQUIRED)

target_link_libraries(Tunis PUBLIC
    glm
    easy_profiler
    stb::stb
    msdfgen::lib_msdfgen
    OpenMP::OpenMP_CXX
)

include(src/${TUNIS_BACKEND}/Backend.cmake)
tunis_backend_dependencies()

if (NOT TUNIS_PROFILING)
    target_compile_definitions(Tunis PUBLIC DISABLE_EASY_PROFILER=1)
endif()

if (TUNIS_BUILD_SAMPLES)
    add_subdirectory(samples)
endif()
