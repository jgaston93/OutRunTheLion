#include <cstdio>

#include "PlayerInputSystem.hpp"


PlayerInputSystem::PlayerInputSystem(MessageBus& message_bus, InputMap& input_map) : System(message_bus), m_input_map(input_map)
{

}

PlayerInputSystem::~PlayerInputSystem()
{
}

void PlayerInputSystem::handleMessage(Message message)
{
    bool gameover = false;
    if(message.message_type == MessageType::COLLISION)
    {
        uint32_t entity_1_id = message.message_data >> 16;
        uint32_t entity_2_id = message.message_data & 0x0000FFFF;
        uint32_t player_entity_id = 0;
        bool player_found = false;
        bool goal_found = false;

        if(strcmp("goal", m_entity_manager->GetEntityTag(entity_1_id)) == 0 ||
            strcmp("goal", m_entity_manager->GetEntityTag(entity_2_id)) == 0)
        {
            goal_found = true;
        }

        if((m_entity_manager->GetEntitySignature(entity_1_id) & PLAYER_INPUT_SYSTEM_SIGNATURE) == PLAYER_INPUT_SYSTEM_SIGNATURE)
        {
            player_entity_id = entity_1_id;
            player_found = true;
        }
        else if((m_entity_manager->GetEntitySignature(entity_2_id) & PLAYER_INPUT_SYSTEM_SIGNATURE) == PLAYER_INPUT_SYSTEM_SIGNATURE)
        {
            player_entity_id = entity_2_id;
            player_found = true;
        }
        if(player_found)
        {
            PlayerInput& player_input = m_component_manager->GetComponent<PlayerInput>(player_entity_id);

            if(goal_found && player_input.state == PlayerState::ALIVE)
            {
                player_input.state = PlayerState::GONE;
                Message message;
                message.message_type = MessageType::WIN;
                message.message_data = 0;
                m_message_bus.postMessage(message);
            }
            else if(player_input.state == PlayerState::ALIVE)
            {
                Texture& texture = m_component_manager->GetComponent<Texture>(player_entity_id);
                RigidBody& rigid_body = m_component_manager->GetComponent<RigidBody>(player_entity_id);
                player_input.state = PlayerState::DYING;
                texture.position[1] = 50;
                rigid_body.acceleration[2] = 0;
                if(rigid_body.velocity[2] > -5)
                {
                    rigid_body.velocity[2] = -5;
                }
                Message message;
                message.message_type = MessageType::LOSE;
                message.message_data = 0;
                m_message_bus.postMessage(message);
            }
        }
    }
    else if(message.message_type == MessageType::TIMEOUT)
    {
        uint32_t player_id = m_entity_manager->GetEntityId("player");
        Texture& texture = m_component_manager->GetComponent<Texture>(player_id);
        RigidBody& rigid_body = m_component_manager->GetComponent<RigidBody>(player_id);
        PlayerInput& player_input = m_component_manager->GetComponent<PlayerInput>(player_id);
        player_input.state = PlayerState::DYING;
        texture.position[1] = 50;
        rigid_body.acceleration[2] = 0;
        if(rigid_body.velocity[2] > -5)
        {
            rigid_body.velocity[2] = -5;
        }
        Message message;
        message.message_type = MessageType::LOSE;
        message.message_data = 0;
        m_message_bus.postMessage(message);
    }

}

void PlayerInputSystem::Update(float delta_time)
{
    uint32_t num_entities = m_entity_manager->GetNumEntities();

    for(uint32_t i = 0; i < num_entities; i++)
    {
        if(m_entity_manager->GetEntityState(i) == EntityState::ACTIVE && 
            ((m_entity_manager->GetEntitySignature(i) & PLAYER_INPUT_SYSTEM_SIGNATURE) == PLAYER_INPUT_SYSTEM_SIGNATURE))
        {
            Transform& transform = m_component_manager->GetComponent<Transform>(i);
            PlayerInput& player_input = m_component_manager->GetComponent<PlayerInput>(i);
            RigidBody& rigid_body = m_component_manager->GetComponent<RigidBody>(i);
            Texture& texture = m_component_manager->GetComponent<Texture>(i);
            Animation& animation = m_component_manager->GetComponent<Animation>(i);
            QuadMesh& quad_mesh = m_component_manager->GetComponent<QuadMesh>(i);

            if(player_input.state == PlayerState::ALIVE)
            {
                transform.rotation[1] = 0;
                rigid_body.acceleration[2] = 0;

                if(m_input_map.IsPressed(player_input.steer_left))
                {
                    transform.rotation[1] -= player_input.rotation;
                }
                else if(m_input_map.IsPressed(player_input.steer_right))
                {
                    transform.rotation[1] += player_input.rotation;
                }

                if(m_input_map.IsPressed(player_input.accelerate))
                {
                    rigid_body.acceleration[2] += player_input.acceleration;
                }
                else if(m_input_map.IsPressed(player_input.brake) && rigid_body.velocity[2] < 0)
                {
                    rigid_body.acceleration[2] -= player_input.acceleration;
                }
                
                if(rigid_body.velocity[2] > 0)
                {
                    rigid_body.velocity[2] = 0;
                }
                else if(rigid_body.velocity[2] < -22)
                {
                    rigid_body.velocity[2] = -22;
                }

                if(-2 <= (transform.position[0] - 0.25) && (transform.position[0] + 0.25) <= 2)
                {
                    player_input.score += -rigid_body.velocity[2];
                }
                uint32_t score_entity_id = m_entity_manager->GetEntityId("score text");
                LabelTexture& label_texture = m_component_manager->GetComponent<LabelTexture>(score_entity_id);
                snprintf(label_texture.characters, 64, "SCORE %d", player_input.score);

            }
            else if(player_input.state == PlayerState::DYING)
            {
                rigid_body.velocity[2] *= 0.95;
                if(rigid_body.velocity[2] > -1)
                {
                    rigid_body.velocity[2] = 0;
                    player_input.state = PlayerState::DEAD;
                    transform.position[1] = 0.25;
                    texture.position[0] = 0;
                    texture.position[1] = 0;
                    texture.extent[0] = 100;
                    texture.extent[1] = 50;
                    quad_mesh.extent[0] = 1;
                    quad_mesh.extent[1] = 0.5;
                    Message message;
                    message.message_type = MessageType::PLAYER_STOPPED;
                    message.message_data = 0;
                    m_message_bus.postMessage(message);
                }
            }

            animation.speed = 0;
            animation.paused = true;
            if(rigid_body.velocity[2] < 0)
            {
                animation.speed = 1 / -rigid_body.velocity[2];
                if(animation.speed < 0.1)
                {
                    animation.speed = 0.1;
                }
                animation.paused = false;
            }
        }
    }
}