cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(Hunter/HunterGate)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.13.tar.gz"
    SHA1 "ef7d6ac5a4ba88307b2bea3e6ed7206c69f542e8"
    LOCAL
)

project(Tunis)

include(BuildOptions)

add_subdirectory(3rdparty)
add_subdirectory(backend)

add_library(Tunis
    cmake/BuildOptions.cmake
    cmake/Hunter/config.cmake

    src/TunisColor.cpp
    include/TunisColor.h

    src/TunisPaint.cpp
    include/TunisPaint.h

    include/TunisPath2D.h
    include/TunisPath2D.inl

    include/TunisPath2DCommand.h
    include/TunisPath2DCommand.inl

    src/Tunis.cpp
    include/Tunis.h

    include/TunisMath.h

)
target_include_directories(Tunis PUBLIC include)
target_link_libraries(Tunis PUBLIC TunisBackend)

if (TUNIS_BUILD_SAMPLES)
    add_subdirectory(samples)
endif()
