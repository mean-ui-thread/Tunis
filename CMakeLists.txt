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

add_subdirectory(backend)

add_library(Tunis
    cmake/BuildOptions.cmake
    cmake/Hunter/config.cmake

    include/soa.h
    include/mapped_soa.h

    src/TunisColor.cpp
    include/TunisColor.h

    src/TunisPaint.cpp
    include/TunisPaint.h

    src/Tunis.cpp
    include/Tunis.h

)
target_include_directories(Tunis PUBLIC include)
target_link_libraries(Tunis PUBLIC TunisBackend)

if (TUNIS_BUILD_SAMPLES)
    add_subdirectory(samples)
endif()
