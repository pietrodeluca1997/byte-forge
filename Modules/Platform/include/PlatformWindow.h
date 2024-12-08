#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct PlatformWindow 
{
    void* window_data;
    int window_height;
    int window_width;
    char* window_title;
    uint64_t window_bitflags;    
} PlatformWindow;

bool PlatformWindow_New(PlatformWindow* out_platform_window);
void PlatformWindow_Free(PlatformWindow* platform_window);