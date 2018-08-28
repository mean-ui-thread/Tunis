
set(TUNIS_BACKEND "GL" CACHE STRING "backend implementation to use")
set_property(CACHE TUNIS_BACKEND PROPERTY STRINGS GL NanoVG-GL3)

option(TUNIS_BUILD_SAMPLES "Build samples" ON)

set(TUNIS_SAMPLES_MAIN "SDL2" CACHE STRING "Main entry point implementation to usef for the samples")
set_property(CACHE TUNIS_SAMPLES_MAIN PROPERTY STRINGS SDL2 GLFW3)

option(TUNIS_PROFILING "Enable profiling using easy_profiling" ON)


