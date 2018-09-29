cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(Hunter/HunterGate)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.27.tar.gz"
    SHA1 "70d75a25f895df3da00d8382417e8c2a08c93173"
    LOCAL
)

project(Tunis)

include(BuildOptions)

add_library(Tunis
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
    include/TunisTexture.h
    include/TunisTypes.h
    include/TunisVertex.h
    src/${TUNIS_BACKEND}/Backend.cmake
    src/${TUNIS_BACKEND}/Tunis.cpp
    src/${TUNIS_BACKEND}/TunisTexture.cpp
    src/${TUNIS_BACKEND}/TunisGraphicStates.h
    src/TunisColor.cpp

    # fast-poly2tri
    3rdparty/fast-poly2tri/MPE_fastpoly2tri.h

    # StructureOfArrays from https://github.com/Lunarsong/StructureOfArrays
    3rdparty/StructureOfArrays/include/mapped_soa.h
    3rdparty/StructureOfArrays/include/soa.h
)

target_include_directories(Tunis
    PUBLIC include
    PUBLIC include/${TUNIS_BACKEND}
    PUBLIC 3rdparty/StructureOfArrays/include
    PUBLIC 3rdparty/fast-poly2tri
    PRIVATE src
    PRIVATE src/${TUNIS_BACKEND}
)

hunter_add_package(glm)
hunter_add_package(easy_profiler)

find_package(glm CONFIG REQUIRED)
find_package(easy_profiler CONFIG REQUIRED)
find_package(OpenMP REQUIRED)

target_link_libraries(Tunis PUBLIC glm easy_profiler OpenMP::OpenMP_CXX)

include(src/${TUNIS_BACKEND}/Backend.cmake)
tunis_backend_dependencies()

if (NOT TUNIS_PROFILING)
    target_compile_definitions(Tunis PUBLIC DISABLE_EASY_PROFILER=1)
endif()

if (TUNIS_BUILD_SAMPLES)
    add_subdirectory(samples)
endif()
