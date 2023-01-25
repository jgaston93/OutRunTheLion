#include "SDL.h"
#include "SDL2/SDL_image.h"

#include "InputSystem.hpp"
#include "ComponentManager.hpp"
#include "EntityManger.hpp"

#include "PlayerInputSystem.hpp"
#include "PhysicsSystem.hpp"
#include "RenderSystem.hpp"

int main(int argv, char* args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    
    char window_name[] = "OutRunTheLion";
    SDL_Window* win = SDL_CreateWindow(window_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, 0);

    const uint32_t num_inputs = 5;
    SDL_Scancode input_list[] = {SDL_Scancode::SDL_SCANCODE_LEFT,
                                 SDL_Scancode::SDL_SCANCODE_RIGHT,
                                 SDL_Scancode::SDL_SCANCODE_UP,
                                 SDL_Scancode::SDL_SCANCODE_DOWN,
                                 SDL_Scancode::SDL_SCANCODE_RETURN};

    InputMap input_map(num_inputs);
    for(uint32_t i = 0; i < num_inputs; i++)
    {
        input_map.AddInput(input_list[i]);
    }

    InputSystem input_system(input_map);
    PlayerInputSystem player_input_system(input_map);
    PhysicsSystem physics_system;
    RenderSystem render_system;

    const uint32_t num_entities = 1;
    EntityManger entity_manager(num_entities);

    entity_manager.SetEntitySignature(0, PLAYER_INPUT_SYSTEM_SIGNATURE);
    entity_manager.SetEntityState(0, EntityState::ACTIVE);

    ComponentManager component_manager(num_entities);

    Transform transform;
    transform.position = { 0, 0, 0 };
    transform.rotation = { 0, 0, 0 };
    transform.scale = { 0, 0, 0 };

    RigidBody rigid_body;
    rigid_body.acceleration = { 0, 0, 0 };
    rigid_body.velocity = { 0, 0, 0 };

    PlayerInput player_input;
    player_input.steer_left = SDL_SCANCODE_LEFT;
    player_input.steer_right = SDL_SCANCODE_RIGHT;
    player_input.accelerate = SDL_SCANCODE_UP;
    player_input.brake = SDL_SCANCODE_DOWN;
    player_input.rotation = 10;
    player_input.acceleration = 10;

    component_manager.AddComponent<Transform>(0, transform);
    component_manager.AddComponent<RigidBody>(0, rigid_body);
    component_manager.AddComponent<PlayerInput>(0, player_input);
    
    uint32_t prev_time = 0;
    uint32_t num_frames = 0;
    uint32_t MS_PER_FRAME = 16;

    bool is_running = true;

    while(is_running)
    {
        uint32_t current_time = SDL_GetTicks();
        float delta_time = (current_time - prev_time) * 1e-3;
        prev_time = current_time;

        input_system.Update(is_running);
        player_input_system.Update(entity_manager, component_manager);
        physics_system.Update(delta_time, entity_manager, component_manager);
        render_system.Update(renderer, entity_manager, component_manager);
        
        // Calculate time to sleep and sleep if necessary
        int next_frame_time = num_frames++ * MS_PER_FRAME;
        current_time = SDL_GetTicks();
        if(current_time < next_frame_time)
        {
            int ms_to_sleep = next_frame_time - current_time;
            SDL_Delay(ms_to_sleep);
        }
    }
    
    return 0;
}