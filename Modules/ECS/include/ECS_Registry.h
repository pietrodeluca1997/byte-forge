#pragma once

#include "ECS_Entity.h"
#include "ECS_ComponentType.h"
#include "ECS_Vector.h"
#include "ECS_System.h"

typedef struct ECS_Registry
{
    ECS_Vector* entities;
    ECS_Vector* component_types;
    ECS_Vector* systems;
    ECS_Vector* entities_components_bitsets;
} ECS_Registry;

bool ECS_Registry_New(ECS_Registry** out_registry);
bool ECS_Registry_CreateEntity(ECS_Registry* registry, ECS_Entity* out_entity);
bool ECS_Registry_RegisterComponentType(ECS_Registry* registry, size_t component_type_size, ECS_ComponentType* out_component_type);
bool ECS_Registry_AddComponentToEntity(ECS_Registry* registry, ECS_ComponentType component_type, void* component, ECS_Entity entity);
bool ECS_Registry_CreateSystem(ECS_Registry* registry, ECS_System* out_system);
void ECS_Registry_PublishEntity(ECS_Registry* registry, ECS_Entity entity);
void ECS_Registry_Init(ECS_Registry* registry);
void ECS_Registry_Update(ECS_Registry* registry);
void ECS_Registry_Free(ECS_Registry** registry);