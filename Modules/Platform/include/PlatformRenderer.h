#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "PlatformWindow.h"

typedef struct PlatformRenderer
{
    void* renderer_data;
    uint64_t renderer_bitflags;    
} PlatformRenderer;

bool PlatformRenderer_New(PlatformRenderer* out_platform_renderer, PlatformWindow* platform_window);
void PlatformRenderer_Free(PlatformRenderer* platform_renderer);