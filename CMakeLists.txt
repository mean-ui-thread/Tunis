cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(Hunter/HunterGate)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.13.tar.gz"
    SHA1 "ef7d6ac5a4ba88307b2bea3e6ed7206c69f542e8"
    LOCAL
)

project(Tunis)

include(BuildOptions)

add_subdirectory(3rdparty)

include(src/${TUNIS_BACKEND}/Sources.cmake)

add_library(Tunis
    cmake/BuildOptions.cmake
    cmake/Hunter/config.cmake

    ${TUNIS_BACKEND_SOURCES}

    src/TunisColor.cpp
    include/TunisColor.h

    src/TunisPaint.cpp
    include/TunisPaint.h

    include/TunisPath2D.h
    include/TunisPath2D.inl

    include/Tunis.h
    include/TunisMath.h
    include/TunisRect.h
    include/TunisRect.inl
    include/TunisRenderState.h
    include/TunisSOA.h
    include/TunisSOA.inl
    include/TunisSize.h
    include/TunisSize.inl
    include/TunisTexture.h
    include/TunisTypes.h
    include/TunisVertex.h

    # From https://github.com/Lunarsong/StructureOfArrays
    include/StructureOfArrays/soa.h
    include/StructureOfArrays/mapped_soa.h
    include/StructureOfArrays/LICENSE
)

target_include_directories(Tunis
    PUBLIC include
    PUBLIC include/StructureOfArrays
    PRIVATE src
    PRIVATE src/${TUNIS_BACKEND}
)

hunter_add_package(glm)
find_package(glm REQUIRED)

target_link_libraries(Tunis PUBLIC glm easy_profiler)

if (NOT TUNIS_PROFILING)
    target_compile_definitions(Tunis PUBLIC -DDISABLE_EASY_PROFILER=1)
endif()

if (TUNIS_BUILD_SAMPLES)
    add_subdirectory(samples)
endif()
