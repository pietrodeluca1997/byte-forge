#define SDL_MAIN_HANDLED

#include <SDL.h>

#include <stdio.h>

#include "PlatformContext.h"
#include "PlatformWindow.h"
#include "PlatformRenderer.h"
#include "ECS.h"

typedef struct Transform2DComponent
{
    float x;
    float y;
    float size_x;
    float size_y;
} Transform2DComponent;

typedef struct Physics2DComponent
{
    float velocity_x;
    float velocity_y;
} Physics2DComponent;

typedef struct PlayerControllerComponent 
{
    void (*HandleInput)(Transform2DComponent* transform2d_component, Physics2DComponent* physics2d_component, uint8_t* keyboard_state);
} PlayerControllerComponent;

typedef struct BoxCollider2DComponent
{
    float x;
    float y;
    float size_x;
    float size_y;
    void (*OnCollisionDetected)(ECS_Entity collider_a, ECS_Entity collider_b);
} BoxCollider2DComponent;

typedef struct Render2DSystem
{
    PlatformRenderer platform_renderer;
    ECS_Vector** transform_component_vector;
} Render2DSystem;

typedef struct RenderCollider2DSystem
{
    PlatformRenderer platform_renderer;
    ECS_Vector** transform_component_vector;
    ECS_Vector** box_collider_component_vector;
} RenderCollider2DSystem;

typedef struct Physics2DSystem
{    
    ECS_Vector** transform_component_vector;
    ECS_Vector** physics_component_vector;
} Physics2DSystem;

typedef struct Collision2DSystem
{
    ECS_Vector** transform_component_vector;
    ECS_Vector** box_collider_component_vector;
} Collision2DSystem;

void Physics2DSystem_Update(ECS_Registry* registry, ECS_System* self)
{    
    Physics2DSystem* physics2d_data = (Physics2DSystem*) self->inner_system_data;
    
    for (uint64_t i = 0; i < self->entities->element_count; i++)
    {
        ECS_Entity* entity = ECS_Vector_Get(self->entities, i);

        Transform2DComponent* entity_transform = ECS_Vector_Get(*physics2d_data->transform_component_vector, entity->id);
        Physics2DComponent* physics = ECS_Vector_Get(*physics2d_data->physics_component_vector, entity->id);

        entity_transform->x += physics->velocity_x;
        entity_transform->y += physics->velocity_y;        
    }
}

void Render2DSystem_Update(ECS_Registry* registry, ECS_System* self)
{    
    Render2DSystem* render2d_data = (Render2DSystem*) self->inner_system_data;
    
    PlatformRenderer renderer = (PlatformRenderer) render2d_data->platform_renderer;    
    
    PlatformContext_ClearBackBuffer(&renderer);

    for (uint64_t i = 0; i < self->entities->element_count; i++)
    {
        ECS_Entity* entity = ECS_Vector_Get(self->entities, i);
        Transform2DComponent* entity_transform = ECS_Vector_Get(*render2d_data->transform_component_vector, entity->id);
        
        PlatformContext_SetDrawColor(&renderer, 255, 255, 255, 255);

        SDL_Rect rect = 
        { 
            (int)entity_transform->x, 
            (int)entity_transform->y, 
            (int)entity_transform->size_x,
            (int)entity_transform->size_y
        };

        SDL_RenderFillRect((SDL_Renderer*) renderer.renderer_data, &rect);
    }
}

void InputSystem_Update(ECS_Registry* registry, ECS_System* self)
{
    PlatformInputSystem* platform_input_system = (PlatformInputSystem*) self->inner_system_data;

    for (uint64_t i = 0; i < self->entities->element_count; i++)
    {
        ECS_Entity* entity = ECS_Vector_Get(self->entities, i);
        PlayerControllerComponent* controller = ECS_Vector_Get(*platform_input_system->player_controller_component_vector, entity->id);
        Physics2DComponent* physics2d = ECS_Vector_Get(*platform_input_system->physics_component_vector, entity->id);
        Transform2DComponent* transform2d = ECS_Vector_Get(*platform_input_system->transform_component_vector, entity->id);
        
        controller->HandleInput(transform2d, physics2d, platform_input_system->keyboard_state);
    }
}

void Collision2DSystem_Update(ECS_Registry* registry, ECS_System* self)
{
    Collision2DSystem* collision_system = (Collision2DSystem*) self->inner_system_data;

    for (uint64_t i = 0; i < self->entities->element_count; i++)
    {
        ECS_Entity* first_entity = ECS_Vector_Get(self->entities, i);
        Transform2DComponent* first_transform2d = ECS_Vector_Get(*collision_system->transform_component_vector, first_entity->id);
        BoxCollider2DComponent* first_box_collider_2d = ECS_Vector_Get(*collision_system->box_collider_component_vector, first_entity->id);

        for (uint64_t j = i; j != self->entities->element_count; j++)
        {            
            ECS_Entity* second_entity = ECS_Vector_Get(self->entities, j);
            
            if (first_entity->id == second_entity->id) continue;

            Transform2DComponent* second_transform2d = ECS_Vector_Get(*collision_system->transform_component_vector, second_entity->id);
            BoxCollider2DComponent* second_box_collider_2d = ECS_Vector_Get(*collision_system->box_collider_component_vector, second_entity->id);
        }
    }
}

void RenderCollider2DSystem_Update(ECS_Registry* registry, ECS_System* self)
{
    RenderCollider2DSystem* render_collider_2d_data = (RenderCollider2DSystem*) self->inner_system_data;
    
    PlatformRenderer renderer = (PlatformRenderer) render_collider_2d_data->platform_renderer;    
    
    for (uint64_t i = 0; i < self->entities->element_count; i++)
    {
        ECS_Entity* entity = ECS_Vector_Get(self->entities, i);
        Transform2DComponent* entity_transform = ECS_Vector_Get(*render_collider_2d_data->transform_component_vector, entity->id);
        BoxCollider2DComponent* box_collider_2d = ECS_Vector_Get(*render_collider_2d_data->box_collider_component_vector, entity->id);

        
        PlatformContext_SetDrawColor(&renderer, 255, 0, 0, 255);

        SDL_Rect rect = 
        { 
            (int)entity_transform->x, 
            (int)entity_transform->y, 
            (int)entity_transform->size_x,
            (int)entity_transform->size_y
        };

        SDL_RenderDrawRect((SDL_Renderer*) renderer.renderer_data, &rect);
    }
}

void HandlePlayerInput(Transform2DComponent* transform2d_component, Physics2DComponent* physics2d_component, uint8_t* keyboard_state)
{
    if (keyboard_state[SDL_SCANCODE_W])
    {
        physics2d_component->velocity_y = -2.0f;
    }

    if (keyboard_state[SDL_SCANCODE_S])
    {
        physics2d_component->velocity_y = 2.0f;
    }
}

const int APPLICATION_WINDOW_WIDTH = 800;
const int APPLICATION_WINDOW_HEIGHT = 600;
const char* APPLICATION_WINDOW_TITLE = "Pong";

int main()
{

#pragma region Platform
    if (!PlatformContext_Initialize(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Couldn't initialize platform context: %s", PlatformContext_GetLastError());
        return EXIT_FAILURE;
    }

    PlatformWindow platform_window = 
    {
        .window_width = APPLICATION_WINDOW_WIDTH,
        .window_height = APPLICATION_WINDOW_HEIGHT,
        .window_title = APPLICATION_WINDOW_TITLE,
        .window_bitflags = 0
    };

    if (!PlatformWindow_New(&platform_window))
    {
        fprintf(stderr, "Couldn't create platform window: %s", PlatformContext_GetLastError());
        PlatformContext_Free();
        return EXIT_FAILURE;
    }

    PlatformRenderer platform_renderer = 
    {
        .renderer_bitflags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    };

    if (!PlatformRenderer_New(&platform_renderer, &platform_window))
    {
        fprintf(stderr, "Couldn't create platform renderer: %s", PlatformContext_GetLastError());
        PlatformWindow_Free(&platform_window);
        PlatformContext_Free();
        return EXIT_FAILURE;
    }
#pragma endregion Platform

#pragma region ECS
    ECS_Registry* registry = NULL;
    if (!ECS_Registry_New(&registry))
    {
        fprintf(stderr, "Couldn't create ECS registry.");        
        ECS_Registry_Free(&registry);
        PlatformRenderer_Free(&platform_renderer);
        PlatformWindow_Free(&platform_window);
        PlatformContext_Free();
        return EXIT_FAILURE;
    }

    ECS_ComponentType transform2d_component_type;
    ECS_ComponentType physics2d_component_type;
    ECS_ComponentType player_controller_type;
    ECS_ComponentType box_collider2d_component_type;

    ECS_Registry_RegisterComponentType(registry, sizeof(Transform2DComponent), &transform2d_component_type);
    ECS_Registry_RegisterComponentType(registry, sizeof(Physics2DComponent), &physics2d_component_type);
    ECS_Registry_RegisterComponentType(registry, sizeof(PlayerControllerComponent), &player_controller_type);
    ECS_Registry_RegisterComponentType(registry, sizeof(BoxCollider2DComponent), &box_collider2d_component_type);

    Render2DSystem render2d_system_data = 
    {
        .platform_renderer = platform_renderer,
        .transform_component_vector = ECS_Vector_Get(registry->component_types, transform2d_component_type.id)
    };

    ECS_System render2d_system;
    render2d_system.required_entity_components_bitset = (1ULL << transform2d_component_type.id);
    render2d_system.Update = Render2DSystem_Update;
    render2d_system.inner_system_data = (void*) &render2d_system_data;

    ECS_Registry_CreateSystem(registry, &render2d_system);
        
    Collision2DSystem collision2d_system_data = 
    {
        .transform_component_vector = ECS_Vector_Get(registry->component_types, transform2d_component_type.id),
        .box_collider_component_vector = ECS_Vector_Get(registry->component_types, box_collider2d_component_type.id)
    };

    ECS_System collision2d_system;
    collision2d_system.required_entity_components_bitset = (1ULL << transform2d_component_type.id) |  (1ULL << box_collider2d_component_type.id);
    collision2d_system.Update = Collision2DSystem_Update;
    collision2d_system.inner_system_data = (void*) &collision2d_system_data;

    ECS_Registry_CreateSystem(registry, &collision2d_system);

    RenderCollider2DSystem render_collider2d_system_data = 
    {
        .transform_component_vector = ECS_Vector_Get(registry->component_types, transform2d_component_type.id),
        .box_collider_component_vector = ECS_Vector_Get(registry->component_types, box_collider2d_component_type.id),
        .platform_renderer = platform_renderer
    };

    ECS_System render_collider2d_system;
    render_collider2d_system.required_entity_components_bitset = (1ULL << transform2d_component_type.id) |  (1ULL << box_collider2d_component_type.id);
    render_collider2d_system.Update = RenderCollider2DSystem_Update;
    render_collider2d_system.inner_system_data = (void*) &render_collider2d_system_data;

    ECS_Registry_CreateSystem(registry, &render_collider2d_system);

    Physics2DSystem physics2d_system_data = 
    {
        .transform_component_vector = ECS_Vector_Get(registry->component_types, transform2d_component_type.id),
        .physics_component_vector = ECS_Vector_Get(registry->component_types, physics2d_component_type.id)
    };

    ECS_System physics2d_system;
    physics2d_system.required_entity_components_bitset = (1ULL << transform2d_component_type.id) | (1ULL << physics2d_component_type.id);
    physics2d_system.Update = Physics2DSystem_Update;
    physics2d_system.inner_system_data = (void*) &physics2d_system_data;

    ECS_Registry_CreateSystem(registry, &physics2d_system);    
    
    PlatformInputSystem input_system_data;
    input_system_data.player_controller_component_vector = ECS_Vector_Get(registry->component_types, player_controller_type.id);
    input_system_data.transform_component_vector = ECS_Vector_Get(registry->component_types, transform2d_component_type.id),
    input_system_data.physics_component_vector = ECS_Vector_Get(registry->component_types, physics2d_component_type.id);

    ECS_System input_system;
    input_system.required_entity_components_bitset = (1ULL << player_controller_type.id) | (1ULL << transform2d_component_type.id) | (1ULL << physics2d_component_type.id);
    input_system.Update = InputSystem_Update;
    input_system.inner_system_data = (void*) &input_system_data;

    ECS_Registry_CreateSystem(registry, &input_system);

    ECS_Entity player, ball;
    ECS_Registry_CreateEntity(registry, &player);
    ECS_Registry_CreateEntity(registry, &ball);

    Transform2DComponent player_transform = { .x = 10.0f, .y = 275.0f, .size_x = 20.0f, .size_y = 100.0f };
    Physics2DComponent player_physics = { 0.0f, 0.0f };
    PlayerControllerComponent player_controller = { .HandleInput = HandlePlayerInput };
    BoxCollider2DComponent player_collider = { .x = 10.0f, .y = 275.0f, .size_x = 20.0f, .size_y = 100.f };
    ECS_Registry_AddComponentToEntity(registry, transform2d_component_type, &player_transform, player);
    ECS_Registry_AddComponentToEntity(registry, physics2d_component_type, &player_physics, player);
    ECS_Registry_AddComponentToEntity(registry, player_controller_type, &player_controller, player);
    ECS_Registry_AddComponentToEntity(registry, box_collider2d_component_type, &player_collider, player);

    ECS_Registry_PublishEntity(registry, player);
    
    Transform2DComponent ball_transform = { 275.0f, 375.0f, 20.0f, 20.0f };
    Physics2DComponent ball_physics = { 5.0f, 5.0f };
    ECS_Registry_AddComponentToEntity(registry, transform2d_component_type, &ball_transform, ball);
    ECS_Registry_AddComponentToEntity(registry, physics2d_component_type, &ball_physics, ball);

    ECS_Registry_PublishEntity(registry, ball);

#pragma endregion ECS

#pragma region ApplicationLoop
    while (PlatformContext_PollThreadEvents(&input_system_data))
    {    
        PlatformContext_ClearBackBuffer(&platform_renderer);
        PlatformContext_SetDrawColor(&platform_renderer, 0, 0, 255, 255);

        ECS_Registry_Update(registry);        

        PlatformContext_SwapBuffer(&platform_renderer);
    }
#pragma endregion ApplicationLoop

#pragma region Dispose
    PlatformRenderer_Free(&platform_renderer);
    PlatformWindow_Free(&platform_window);
    PlatformContext_Free();
#pragma endregion Dispose

    return EXIT_SUCCESS;
}