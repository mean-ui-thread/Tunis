hunter_add_package(flatbuffers)
find_package(Flatbuffers CONFIG REQUIRED)

find_program(FLATC_EXECUTABLE flatc HINTS ${FLATBUFFERS_ROOT}/bin)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/TunisFonts_generated.h
        ${CMAKE_CURRENT_BINARY_DIR}/TunisFonts_generated.js
    COMMAND
        ${FLATC_EXECUTABLE} --cpp --js ${CMAKE_CURRENT_SOURCE_DIR}/TunisFonts.fbs
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/TunisFonts.fbs
)

add_custom_target(TunisFontsGenerator
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/TunisFonts.fbs
        ${CMAKE_CURRENT_BINARY_DIR}/TunisFonts_generated.h
        ${CMAKE_CURRENT_BINARY_DIR}/TunisFonts_generated.js
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/TunisFonts_generated.h
        ${CMAKE_CURRENT_BINARY_DIR}/TunisFonts_generated.js
)

add_library(TunisFonts INTERFACE)
target_include_directories(TunisFonts INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(TunisFonts INTERFACE flatbuffers::flatbuffers)
add_dependencies(TunisFonts TunisFontsGenerator)
