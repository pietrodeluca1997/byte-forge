#include "ECS_Registry.h"

#include <stdlib.h>

bool ECS_Registry_New(ECS_Registry** out_registry)
{
    *out_registry = (ECS_Registry*) calloc(1, sizeof(ECS_Registry));

    if(!*out_registry) return false;

    return ECS_Vector_New(sizeof(ECS_Entity), &(*out_registry)->entities) && 
           ECS_Vector_New(sizeof(ECS_ComponentType*), &(*out_registry)->component_types) &&
           ECS_Vector_New(sizeof(ECS_System), &(*out_registry)->systems) &&
           ECS_Vector_New(sizeof(uint64_t), &(*out_registry)->entities_components_bitsets);
}

bool ECS_Registry_CreateEntity(ECS_Registry* registry, ECS_Entity* out_entity)
{    
    if(!registry || !out_entity) return false;

    ECS_Entity entity = { registry->entities->element_count };
    
    if (ECS_Vector_Add(registry->entities, &entity))
    {        
        *out_entity = entity;

        uint64_t initial_entity_bitset = 0;
        ECS_Vector_Add(registry->entities_components_bitsets, &initial_entity_bitset);

        return true;
    }

    return false;
}

bool ECS_Registry_RegisterComponentType(ECS_Registry* registry, size_t component_type_size, ECS_ComponentType* out_component_type)
{
    if (!registry || component_type_size <= 0 || !out_component_type) return false;

    ECS_ComponentType component_type;
    ECS_ComponentType_New(registry->component_types->element_count, component_type_size, &component_type);     
    
    ECS_Vector* components_vector;
    
    if (ECS_Vector_New(component_type_size, &components_vector) && ECS_Vector_Add(registry->component_types, &components_vector))
    {
        *out_component_type = component_type;
        return true;
    }

    ECS_Vector_Free(&components_vector); 
    return false;
}

bool ECS_Registry_AddComponentToEntity(ECS_Registry* registry, ECS_ComponentType component_type, void* component, ECS_Entity entity)
{
    if (!registry || !component || entity.id >= registry->entities->element_count) return false;

    uint64_t* entity_component_bitset = ECS_Vector_Get(registry->entities_components_bitsets, entity.id);
    uint64_t component_bit = 1ULL << component_type.id;
    *entity_component_bitset |= component_bit;

    ECS_Vector** component_vector = ECS_Vector_Get(registry->component_types, component_type.id);

    if (!component_vector || !*component_vector) return false;

    return ECS_Vector_Add(*component_vector, component);
}

bool ECS_Registry_CreateSystem(ECS_Registry *registry, ECS_System* out_system)
{
    if(!registry || !out_system || !out_system->Update) return false;

    ECS_System system = { registry->systems->element_count };
    ECS_Vector_New(sizeof(ECS_Entity), &system.entities);
    system.required_entity_components_bitset = out_system->required_entity_components_bitset;
    system.Update = out_system->Update;
    system.inner_system_data = out_system->inner_system_data;
    
    if (ECS_Vector_Add(registry->systems, &system))
    {        
        *out_system = system;
        return true;
    }

    return false;
}

void ECS_Registry_PublishEntity(ECS_Registry *registry, ECS_Entity entity)
{
    uint64_t* entity_component_bitset = ECS_Vector_Get(registry->entities_components_bitsets, entity.id);
    for (uint64_t i = 0; i < registry->systems->element_count; i++)
    {
        ECS_System* system = ECS_Vector_Get(registry->systems, i);
        
        if((system->required_entity_components_bitset & *entity_component_bitset) == system->required_entity_components_bitset)
        {
            ECS_Vector_Add(system->entities, &entity);
        }
    }
}

void ECS_Registry_Update(ECS_Registry *registry)
{
    for (uint64_t i = 0; i < registry->systems->element_count; i++)
    {
        ECS_System* system = ECS_Vector_Get(registry->systems, i);
        system->Update(registry, system);
    }
}

void ECS_Registry_Free(ECS_Registry** registry)
{   
    ECS_Vector_Free(&(*registry)->entities);
    ECS_Vector_Free(&(*registry)->entities_components_bitsets);

    for (uint64_t i = 0; i < (*registry)->component_types->element_count; i++)
    {
        ECS_Vector** component_type_vector = ECS_Vector_Get((*registry)->component_types, i);
        ECS_Vector_Free(component_type_vector);
    }

    for (uint64_t i = 0; i < (*registry)->systems->element_count; i++)
    {
        ECS_System* system = ECS_Vector_Get((*registry)->systems, i);
        ECS_Vector_Free(&system->entities);
    }

    ECS_Vector_Free(&(*registry)->systems);
    ECS_Vector_Free(&(*registry)->component_types);
    free(*registry);
    *registry = NULL;
}
