hunter_add_package(glm)
find_package(glm REQUIRED)

include(${TUNIS_BACKEND}/Sources.cmake)

add_library(TunisBackend
    ${TUNIS_BACKEND}/Sources.cmake
    ${TUNIS_BACKEND_SOURCES}
    include/TunisBackend.h
    include/TunisRenderState.h
)

target_include_directories(TunisBackend
    PUBLIC include
    PRIVATE ${TUNIS_BACKEND}
)

target_link_libraries(TunisBackend PUBLIC glm)
