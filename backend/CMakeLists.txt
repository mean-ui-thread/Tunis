hunter_add_package(glm)
find_package(glm REQUIRED)

include(${TUNIS_BACKEND}/Sources.cmake)

add_library(TunisBackend
    ${TUNIS_BACKEND}/Sources.cmake
    ${TUNIS_BACKEND_SOURCES}

    ${TUNIS_BACKEND}/TunisBackend.cpp
    ${TUNIS_BACKEND}/TunisTexture.cpp

    include/TunisBackend.h
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

target_include_directories(TunisBackend
    PUBLIC include
    PUBLIC include/StructureOfArrays
    PRIVATE ${TUNIS_BACKEND}
)

target_link_libraries(TunisBackend PUBLIC glm easy_profiler)

if (NOT TUNIS_PROFILING)
    target_compile_definitions(TunisBackend PUBLIC -DDISABLE_EASY_PROFILER=1)
endif()
