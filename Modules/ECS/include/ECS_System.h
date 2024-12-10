#pragma once

#include <stdint.h>

#include "ECS_Vector.h"

typedef struct ECS_Registry ECS_Registry;
typedef struct ECS_System ECS_System;

typedef struct ECS_System
{
    uint64_t id;
    uint64_t required_entity_components_bitset;
    ECS_Vector* entities;
    void (*Update)(ECS_Registry* registry, ECS_System* self);
    void* inner_system_data;
} ECS_System;