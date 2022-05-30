#define MKATAPPENGINECFILE
#include "MKTAppEngine.h"

#include <SDL/SDL.h>
#include <SDL/SDL_vulkan.h>

#include "types.h"
#include "init.h"

void _MKTAE_init()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN);

    _window = SDL_CreateWindow(
        "MKTMultiApp", // title
        SDL_WINDOWPOS_UNDEFINED, //
        SDL_WINDOWPOS_UNDEFINED, //
        _windowExtent.width,
        _windowExtent.height,
        window_flags
    );

    _isInitialized = true;
}

void _MKTAE_cleanup()
{
    if(_isInitialized)
    {
        SDL_DestroyWindow(_window);
    }
}

void _MKTAE_draw()
{
    // it don't be drawing
}

void _MKTAE_run()
{
    SDL_Event e;
    bool bQuit = false;

    while(!bQuit)
    {
        while(SDL_PollEvent(&e) != 0)
            if(e.type == SDL_QUIT) bQuit = true;

        _MKTAE_draw();
    }
}