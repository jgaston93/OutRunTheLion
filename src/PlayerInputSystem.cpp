#include "PlayerInputSystem.hpp"


PlayerInputSystem::PlayerInputSystem(InputMap& input_map) : m_input_map(input_map)
{

}

PlayerInputSystem::~PlayerInputSystem()
{

}

void PlayerInputSystem::Update(EntityManger& entity_manager, ComponentManager& component_manager)
{
    uint32_t num_entities = entity_manager.GetNumEntities();

    for(uint32_t i = 0; i < num_entities; i++)
    {
        if(entity_manager.GetEntityState(i) == EntityState::ACTIVE && entity_manager.GetEntitySignature(i) & PLAYER_INPUT_SYSTEM_SIGNATURE > 0)
        {
            Transform& transform = component_manager.GetComponent<Transform>(i);
            PlayerInput& player_input = component_manager.GetComponent<PlayerInput>(i);
            RigidBody& rigid_body = component_manager.GetComponent<RigidBody>(i);

            transform.rotation.x = 0;
            rigid_body.acceleration.x = 0;

            if(m_input_map.IsPressed(player_input.steer_left))
            {
                transform.rotation.x -= player_input.rotation;
            }
            else if(m_input_map.IsPressed(player_input.steer_right))
            {
                transform.rotation.x += player_input.rotation;
            }

            if(m_input_map.IsPressed(player_input.accelerate))
            {
                rigid_body.acceleration.x += player_input.acceleration;
            }
            else if(m_input_map.IsPressed(player_input.brake))
            {
                rigid_body.acceleration.x -= player_input.acceleration;
            }

        }
    }
}