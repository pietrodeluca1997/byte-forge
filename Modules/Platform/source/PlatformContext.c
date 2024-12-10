#include "PlatformContext.h"

#include <SDL.h>

bool PlatformContext_Initialize(uint64_t initialization_bitflags)
{
    return SDL_Init(initialization_bitflags) == 0;
}

bool PlatformContext_PollThreadEvents(PlatformInputSystem* input_system)
{
    SDL_Event sdl_event;

    while (SDL_PollEvent(&sdl_event))
    {
        switch (sdl_event.type)
        {
            case SDL_QUIT:
                return false;
        }
    }

    input_system->keyboard_state = (uint8_t*) SDL_GetKeyboardState(NULL);

    return true;
}

const char* PlatformContext_GetLastError()
{
    return SDL_GetError();
}

void PlatformContext_SetDrawColor(const PlatformRenderer *platform_renderer, const int r, const int g, const int b, const int a)
{
    SDL_SetRenderDrawColor((SDL_Renderer*) platform_renderer->renderer_data, r, g, b, a);
}

void PlatformContext_ClearBackBuffer(const PlatformRenderer *platform_renderer)
{
    SDL_RenderClear((SDL_Renderer*) platform_renderer->renderer_data);
}

void PlatformContext_SwapBuffer(const PlatformRenderer *platform_renderer)
{
    SDL_RenderPresent((SDL_Renderer*) platform_renderer->renderer_data);
}

void PlatformContext_Free()
{
    SDL_Quit();
}
