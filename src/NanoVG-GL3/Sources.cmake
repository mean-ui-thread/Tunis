include_directories(3rdparty/nanovg/src)
add_definitions(-DNANOVG_GL3_IMPLEMENTATION)

set(TUNIS_BACKEND_SOURCES
    src/NanoVG-GL3/Sources.cmake
    src/NanoVG-GL3/Tunis.cpp
    src/NanoVG-GL3/TunisTexture.cpp
    src/GL/TunisGL.h
    3rdparty/nanovg/src/nanovg.c
)
