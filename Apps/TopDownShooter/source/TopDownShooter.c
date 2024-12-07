#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

#include <SDL.h>

typedef struct SDL_Properties
{
    SDL_Window* window;
    SDL_Renderer* renderer;
} SDL_Properties;

bool SDL_Initialize(SDL_Properties* out_sdl_properties);
void SDL_Free(SDL_Properties* sdl_properties);

bool SDL_Initialize(SDL_Properties* out_sdl_properties)
{
    SDL_Properties sdl_properties = {};
    memset(&sdl_properties, 0, sizeof(SDL_Properties));
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    int sdl_window_flags = 0;

    sdl_properties.window = SDL_CreateWindow("Top Down Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, sdl_window_flags);

    if (!sdl_properties.window)
    {
        fprintf(stderr, "Couldn't create SDL window: %s\n", SDL_GetError());
        SDL_Free(&sdl_properties);
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    int sdl_renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

    sdl_properties.renderer = SDL_CreateRenderer(sdl_properties.window, -1, sdl_renderer_flags);

    if (!sdl_properties.renderer)
    {
        fprintf(stderr, "Couldn't create SDL renderer: %s\n", SDL_GetError());
        SDL_Free(&sdl_properties);
        return false;
    }

    return true;
}

void SDL_Free(SDL_Properties* out_sdl_properties)
{
    if (!out_sdl_properties) return;
    
    if (out_sdl_properties->renderer)
    {
        SDL_DestroyRenderer(out_sdl_properties->renderer);
    }

    if (out_sdl_properties->window)
    {
        SDL_DestroyWindow(out_sdl_properties->window);
    }

    SDL_Quit();
}

int main(int argc, char** argv)
{
    SDL_Properties sdl_properties = {};

    if (!SDL_Initialize(&sdl_properties))
    {
        return EXIT_FAILURE;
    }


    SDL_Free(&sdl_properties);

    return EXIT_SUCCESS;
}