#include "PlatformRenderer.h"
#include <SDL.h>

bool PlatformRenderer_New(PlatformRenderer *out_platform_renderer, PlatformWindow* platform_window)
{
    SDL_Renderer* sdl_renderer = SDL_CreateRenderer((SDL_Window*) platform_window->window_data, -1, out_platform_renderer->renderer_bitflags);

    bool creation_result = sdl_renderer;
    out_platform_renderer->renderer_data = (void*) sdl_renderer;

    return creation_result;
}

void PlatformRenderer_Free(PlatformRenderer *platform_renderer)
{
    SDL_DestroyRenderer((SDL_Renderer*) platform_renderer->renderer_data);
    platform_renderer = NULL;
}
