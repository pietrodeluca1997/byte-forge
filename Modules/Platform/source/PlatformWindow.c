#include "PlatformWindow.h"
#include <SDL.h>

bool PlatformWindow_New(PlatformWindow *out_platform_window)
{
    SDL_Window* sdl_window = SDL_CreateWindow(
        out_platform_window->window_title, 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        out_platform_window->window_width, 
        out_platform_window->window_height,
        out_platform_window->window_bitflags
    );

    if (!sdl_window) return false;

    out_platform_window->window_data = (void*) sdl_window;

    return true;
}

void PlatformWindow_Free(PlatformWindow* platform_window)
{
    SDL_DestroyWindow((SDL_Window*) platform_window->window_data);
    platform_window = NULL;
}