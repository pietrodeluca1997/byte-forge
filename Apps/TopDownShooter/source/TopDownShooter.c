#define SDL_MAIN_HANDLED

#include <SDL.h>

#include <stdio.h>

#include "PlatformContext.h"
#include "PlatformWindow.h"
#include "PlatformRenderer.h"

const int APPLICATION_WINDOW_WIDTH = 800;
const int APPLICATION_WINDOW_HEIGHT = 600;

int main()
{
    if (!PlatformContext_Initialize(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Couldn't initialize platform context: %s", PlatformContext_GetLastError());
        return EXIT_FAILURE;
    }

    PlatformWindow platform_window = 
    {
        .window_width = APPLICATION_WINDOW_WIDTH,
        .window_height = APPLICATION_WINDOW_HEIGHT,
        .window_title = "Top Down Shooter",
        .window_bitflags = 0
    };

    if (!PlatformWindow_New(&platform_window))
    {
        fprintf(stderr, "Couldn't create platform window: %s", PlatformContext_GetLastError());
        return EXIT_FAILURE;
    }

    PlatformRenderer platform_renderer = 
    {
        .renderer_bitflags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    };

    if (!PlatformRenderer_New(&platform_renderer, &platform_window))
    {
        fprintf(stderr, "Couldn't create platform RENDERER: %s", PlatformContext_GetLastError());
        return EXIT_FAILURE;
    }

    while (PlatformContext_PollThreadEvents())
    {
        PlatformContext_SetDrawColor(&platform_renderer, 0, 0, 255, 255);
        PlatformContext_ClearBackBuffer(&platform_renderer);
        PlatformContext_Render(&platform_renderer);
    }

    PlatformRenderer_Free(&platform_renderer);
    PlatformWindow_Free(&platform_window);
    PlatformContext_Free();

    return EXIT_SUCCESS;
}