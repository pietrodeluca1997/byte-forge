#define SDL_MAIN_HANDLED

#include <SDL.h>

#include <stdio.h>

#include "PlatformContext.h"
#include "PlatformWindow.h"
#include "PlatformRenderer.h"
#include "ECS.h"

const int APPLICATION_WINDOW_WIDTH = 800;
const int APPLICATION_WINDOW_HEIGHT = 600;

typedef struct Transform2DComponent
{
    float x;
    float y;
} Transform2DComponent;

typedef struct Physics2DComponent
{
    float velocity_x;
    float velocity_y;
} Physics2DComponent;

ECS_ComponentType transform2d_component_type;
ECS_ComponentType physics2d_component_type;

typedef struct Render2DSystem
{
    PlatformRenderer platform_renderer;
} Render2DSystem;

typedef struct Physics2DSystem
{

} Physics2DSystem;

void Physics2DSystem_Init()
{

}

void Physics2DSystem_Update(ECS_Registry* registry, ECS_System* self)
{
    for (uint64_t i = 0; i < self->entities->element_count; i++)
    {
        ECS_Entity* entity = ECS_Vector_Get(self->entities, i);
        ECS_Vector** transform2d_component_vector = ECS_Vector_Get(registry->component_types, transform2d_component_type.id);
        ECS_Vector** physics2d_component_vector = ECS_Vector_Get(registry->component_types, physics2d_component_type.id);

        Transform2DComponent* entity_transform = ECS_Vector_Get(*transform2d_component_vector, entity->id);
        Physics2DComponent* physics = ECS_Vector_Get(*physics2d_component_vector, entity->id);

        entity_transform->x += physics->velocity_x;
        entity_transform->y += physics->velocity_y;        
    }
}

void Render2DSystem_Init() 
{
    printf("Render 2D System initiating.\n");
}

void Render2DSystem_Update(ECS_Registry* registry, ECS_System* self)
{
    Render2DSystem* render2d_data = (Render2DSystem*) self->data;

    PlatformRenderer renderer = (PlatformRenderer) render2d_data->platform_renderer;    
    
    PlatformContext_ClearBackBuffer(&renderer);

    for (uint64_t i = 0; i < self->entities->element_count; i++)
    {
        ECS_Entity* entity = ECS_Vector_Get(self->entities, i);
        ECS_Vector** transform2d_component_vector = ECS_Vector_Get(registry->component_types, transform2d_component_type.id);
        Transform2DComponent* entity_transform = ECS_Vector_Get(*transform2d_component_vector, entity->id);
        
        PlatformContext_SetDrawColor(&renderer, 255, 255, 255, 255);

        SDL_Rect rect = 
        { 
            (int)entity_transform->x, 
            (int)entity_transform->y, 
            50, 50 
        };

        SDL_RenderFillRect((SDL_Renderer*) renderer.renderer_data, &rect);
    }
}

int main()
{
    if (!PlatformContext_Initialize(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Couldn't initialize platform context: %s", PlatformContext_GetLastError());
        return EXIT_FAILURE;
    }

    PlatformWindow platform_window = 
    {
        .window_width = APPLICATION_WINDOW_WIDTH,
        .window_height = APPLICATION_WINDOW_HEIGHT,
        .window_title = "Top Down Shooter",
        .window_bitflags = 0
    };

    if (!PlatformWindow_New(&platform_window))
    {
        fprintf(stderr, "Couldn't create platform window: %s", PlatformContext_GetLastError());
        return EXIT_FAILURE;
    }

    PlatformRenderer platform_renderer = 
    {
        .renderer_bitflags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    };

    if (!PlatformRenderer_New(&platform_renderer, &platform_window))
    {
        fprintf(stderr, "Couldn't create platform RENDERER: %s", PlatformContext_GetLastError());
        return EXIT_FAILURE;
    }
    
    ECS_Registry* registry = NULL;
    ECS_Registry_New(&registry);

    ECS_Registry_RegisterComponentType(registry, sizeof(Transform2DComponent), &transform2d_component_type);
    ECS_Registry_RegisterComponentType(registry, sizeof(Physics2DComponent), &physics2d_component_type);

    Render2DSystem render2d_system_data = 
    {
        .platform_renderer = platform_renderer
    };

    ECS_System render2d_system;
    render2d_system.required_entity_components_bitset = (1ULL << transform2d_component_type.id);
    render2d_system.Init = Render2DSystem_Init;
    render2d_system.Update = Render2DSystem_Update;
    render2d_system.data = (void*) &render2d_system_data;

    ECS_Registry_CreateSystem(registry, &render2d_system);

    Physics2DSystem physics2d_system_data = 
    {

    };

    ECS_System physics2d_system;
    physics2d_system.required_entity_components_bitset = (1ULL << transform2d_component_type.id) | (1ULL << physics2d_component_type.id);
    physics2d_system.Init = Physics2DSystem_Init;
    physics2d_system.Update = Physics2DSystem_Update;
    physics2d_system.data = (void*) &physics2d_system_data;

    ECS_Registry_CreateSystem(registry, &physics2d_system);

    ECS_Entity player, enemy;
    ECS_Registry_CreateEntity(registry, &player);
    ECS_Registry_CreateEntity(registry, &enemy);

    Transform2DComponent player_transform = { 100.0f, 100.0f };
    Physics2DComponent player_physics = { 2.0f, 0.0f };
    Transform2DComponent enemy_transform = { 250.0f, 120.0f };
    ECS_Registry_AddComponentToEntity(registry, transform2d_component_type, &player_transform, player);
    ECS_Registry_AddComponentToEntity(registry, physics2d_component_type, &player_physics, player);

    ECS_Registry_AddComponentToEntity(registry, transform2d_component_type, &enemy_transform, enemy);

    ECS_Registry_PublishEntity(registry, player);
    ECS_Registry_PublishEntity(registry, enemy);

    ECS_Registry_Init(registry);

    while (PlatformContext_PollThreadEvents())
    {    
        PlatformContext_ClearBackBuffer(&platform_renderer);
        PlatformContext_SetDrawColor(&platform_renderer, 0, 0, 255, 255);
        ECS_Registry_Update(registry);        
        PlatformContext_Render(&platform_renderer);
    }

    PlatformRenderer_Free(&platform_renderer);
    PlatformWindow_Free(&platform_window);
    PlatformContext_Free();

    return EXIT_SUCCESS;
}