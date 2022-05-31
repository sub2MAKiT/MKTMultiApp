#define MKATAPPENGINECFILE
#include "MKTAppEngine.h"

#include <SDL/SDL.h>
#include <SDL/SDL_vulkan.h>

#define VK_CHECK(x) do{VkResult err = x;if (err){std::cout <<"Detected Vulkan error: " << err << std::endl;abort();}} while (0)

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

    init_vulkan();

    _isInitialized = true;
}

void init_vulkan()
{
    vkb::InstanceBuilder builder;
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