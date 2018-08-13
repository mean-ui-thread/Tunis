hunter_add_package(SDL2)
find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_library(TunisSampleMain
    mainSDL2.cpp
)

target_link_libraries(TunisSampleMain
    PUBLIC
        SDL2::SDL2
        SDL2::SDL2main
        OpenGL::GL
)
