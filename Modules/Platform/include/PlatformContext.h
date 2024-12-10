#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "PlatformInputSystem.h"
#include "PlatformRenderer.h"

bool PlatformContext_Initialize(const uint64_t initialization_bitflags);
bool PlatformContext_PollThreadEvents(PlatformInputSystem* input_system);

const char* PlatformContext_GetLastError();

void PlatformContext_SetDrawColor(const PlatformRenderer* platform_renderer, const int r, const int g, const int b, const int a);
void PlatformContext_ClearBackBuffer(const PlatformRenderer* platform_renderer);
void PlatformContext_SwapBuffer(const PlatformRenderer* platform_renderer);

void PlatformContext_Free();