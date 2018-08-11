
#include <SDL2/SDL.h>
#include "SampleApp.h"

#include <chrono>
#include <cstdarg>


int main( int argc, char* args[] )
{
    SampleApp app;

    if (SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return -1;
    }

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    SDL_Window* window = SDL_CreateWindow(app.getSampleName(),
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          app.getScreenWidth(),
                                          app.getScreenHeight(),
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

    if(!window)
    {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return -1;
    }


    SDL_GLContext context = SDL_GL_CreateContext(window);
    if(!context)
    {
        fprintf(stderr, "SDL_GL_CreateContext failed: %s\n", SDL_GetError());
        return -1;
    }

    SDL_GL_SetSwapInterval(1);

    SDL_Event e = {};
    Uint64 start = SDL_GetPerformanceCounter();

    bool quit = false;
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
        }

        app.render((double)(SDL_GetPerformanceCounter() - start) / SDL_GetPerformanceFrequency());

        SDL_GL_SwapWindow(window);
    }


    SDL_Quit();

    return 0;
}

