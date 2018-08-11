cmake_minimum_required(VERSION 3.11)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(Hunter/HunterGate)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.13.tar.gz"
    SHA1 "ef7d6ac5a4ba88307b2bea3e6ed7206c69f542e8"
    LOCAL
)

project(Tunis)

include(BuildOptions)


add_library(Tunis
    src/TunisGL.c
)
target_include_directories(Tunis PUBLIC include)


if (TUNIS_BUILD_SAMPLES)
    add_subdirectory(samples)
endif()
