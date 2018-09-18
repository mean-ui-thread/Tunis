##
# Backend engine to use
##
set(TUNIS_BACKEND "GL" CACHE STRING "backend implementation to use")
set_property(CACHE TUNIS_BACKEND PROPERTY STRINGS GL NanoVG-GL3)

##
# Enable/Disable Samples
##
option(TUNIS_BUILD_SAMPLES "Build samples" ON)

if (TUNIS_BUILD_SAMPLES)

    ##
    # Samples window system
    ##
    set(TUNIS_SAMPLES_MAIN "SDL2" CACHE STRING "Main entry point implementation to usef for the samples")
    set_property(CACHE TUNIS_SAMPLES_MAIN PROPERTY STRINGS SDL2 GLFW3)

    ##
    # Enable/Disable multi-sampling anti-aliasing
    ##
    option(TUNIS_SAMPLES_MSAA "Enable sample Multi-Sampling Anti-Aliasing" ON)

endif()

##
# Enable/Disabe Profiling with easy_profiler
##
if(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    set(defaultProfile ON)
else()
    set(defaultProfile OFF)
endif()
option(TUNIS_PROFILING "Enable profiling with easy_profiler" ${defaultProfile})
unset(defaultProfile)


