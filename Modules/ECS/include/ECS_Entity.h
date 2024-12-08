#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct ECS_Entity
{
    uint64_t id;
} ECS_Entity;

static inline bool ECS_Entity_New(uint64_t id, ECS_Entity* out_entity_type)
{
    if (!out_entity_type) 
    {
        return false;
    }

    ECS_Entity entity;
    entity.id = id;

    *out_entity_type = entity;

    return true;
}