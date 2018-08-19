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

    include/soa.h
    include/mapped_soa.h
)

target_include_directories(TunisBackend
    PUBLIC include
    PRIVATE ${TUNIS_BACKEND}
)

target_link_libraries(TunisBackend PUBLIC glm)
