hunter_add_package(glm)
find_package(glm REQUIRED)

include(${TUNIS_BACKEND}/Sources.cmake)

add_library(TunisBackend
    ${TUNIS_BACKEND}/Sources.cmake
    ${TUNIS_BACKEND_SOURCES}

    ${TUNIS_BACKEND}/TunisBackend.cpp
    ${TUNIS_BACKEND}/TunisTexture.cpp

    include/TunisBackend.h
    include/TunisRenderState.h
    include/TunisTexture.h

    include/StructureOfArrays/soa.h
    include/StructureOfArrays/mapped_soa.h
    include/StructureOfArrays/LICENSE
)

target_include_directories(TunisBackend
    PUBLIC include
    PUBLIC include/StructureOfArrays
    PRIVATE ${TUNIS_BACKEND}
)

target_link_libraries(TunisBackend PUBLIC glm)
