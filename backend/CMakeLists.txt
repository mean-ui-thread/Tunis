
include(${TUNIS_BACKEND}/Sources.cmake)

add_library(TunisBackend
    ${TUNIS_BACKEND}/Sources.cmake
    ${TUNIS_BACKEND_SOURCES}
)

target_include_directories(TunisBackend
    PUBLIC include
    PRIVATE ${TUNIS_BACKEND}
)
