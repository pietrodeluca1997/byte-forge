#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct ECS_ComponentType
{
    uint64_t id;
    size_t component_type_size;
} ECS_ComponentType;

static inline bool ECS_ComponentType_New(uint64_t id, size_t component_type_size, ECS_ComponentType* out_component_type)
{
    if (component_type_size <= 0 || !out_component_type) 
    {
        return false;
    }

    ECS_ComponentType component_type;
    component_type.id = id;
    component_type.component_type_size = component_type_size;

    *out_component_type = component_type;

    return true;
}