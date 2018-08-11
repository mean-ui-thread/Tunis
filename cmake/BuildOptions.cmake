
option(TUNIS_BUILD_SAMPLES "Build samples" ON )

set(TUNIS_SAMPLES_MAIN "SDL2" CACHE STRING "Main entry point implementation to use")
set_property(CACHE TUNIS_SAMPLES_MAIN PROPERTY STRINGS SDL2 GLFW3)
