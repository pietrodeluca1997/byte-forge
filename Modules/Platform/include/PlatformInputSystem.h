#pragma once

#include "ECS_Registry.h"

#include <stdint.h>

typedef struct PlatformInputSystem
{
    uint8_t* keyboard_state;
    ECS_Vector** player_controller_component_vector;
    ECS_Vector** physics_component_vector;
    ECS_Vector** transform_component_vector;
} PlatformInputSystem;