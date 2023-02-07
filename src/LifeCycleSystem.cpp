#include <stdio.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "LifeCycleSystem.hpp"


LifeCycleSystem::LifeCycleSystem(MessageBus& message_bus, InputMap& input_map) : 
    System(message_bus), 
    m_input_map(input_map), 
    m_state(LifeCycleState::START)
{

}

LifeCycleSystem::~LifeCycleSystem()
{

}

void LifeCycleSystem::handleMessage(Message message)
{
    bool game_over = false;
    uint32_t text_id = 0;

    if(message.message_type == MessageType::WIN)
    {
        game_over = true;
        text_id = m_entity_manager->GetEntityId("win text");
    }
    else if(message.message_type == MessageType::LOSE)
    {
        game_over = true;
        text_id = m_entity_manager->GetEntityId("gameover text");
    }
    else if(message.message_type == MessageType::PLAYER_STOPPED)
    {
        uint32_t lion_id = m_entity_manager->GetEntityId("lion");
        m_entity_manager->SetEntityState(lion_id, EntityState::ACTIVE);
        Transform& lion_transform = m_component_manager->GetComponent<Transform>(lion_id);
        RigidBody& lion_rigid_body = m_component_manager->GetComponent<RigidBody>(lion_id);
        Texture& lion_texture = m_component_manager->GetComponent<Texture>(lion_id);
        Animation& animation = m_component_manager->GetComponent<Animation>(lion_id);

        uint32_t player_id = m_entity_manager->GetEntityId("player");
        Transform& player_transform = m_component_manager->GetComponent<Transform>(player_id);
        RigidBody& player_rigid_body = m_component_manager->GetComponent<RigidBody>(player_id);

        lion_transform.position[0] = player_transform.position[0] + 5;
        lion_transform.position[2] = player_transform.position[2];
        lion_rigid_body.velocity[0] = -1;
        lion_rigid_body.velocity[1] = 0;
        lion_rigid_body.velocity[2] = 0;
        lion_texture.position[1] = 75;
        animation.speed = 0.25;
    }
    else if(message.message_type == MessageType::COLLISION)
    {
        uint32_t entity_1_id = message.message_data >> 16;
        uint32_t entity_2_id = message.message_data & 0x0000FFFF;
        bool lion_found = false;
        uint32_t lion_id = 0;
        if(strcmp(m_entity_manager->GetEntityTag(entity_1_id), "lion") == 0)
        {
            lion_found = true;
            lion_id = entity_1_id;
        }
        else if(strcmp(m_entity_manager->GetEntityTag(entity_2_id), "lion") == 0)
        {
            lion_found = true;
            lion_id = entity_2_id;
        }
        if(lion_found)
        {
            RigidBody& rigid_body = m_component_manager->GetComponent<RigidBody>(lion_id);
            Texture& texture = m_component_manager->GetComponent<Texture>(lion_id);
            Animation& animation = m_component_manager->GetComponent<Animation>(lion_id);

            animation.speed = 1;
            rigid_body.velocity[0] = 0;
            texture.position[1] = 0;
        }
    }

    if(game_over)
    {
        m_entity_manager->SetEntityState(text_id, EntityState::ACTIVE);
        uint32_t timer_id = m_entity_manager->GetEntityId("timer");
        Timer& timer = m_component_manager->GetComponent<Timer>(timer_id);
        timer.active = false;
    }
}

void LifeCycleSystem::Update()
{
    if(m_state == LifeCycleState::START || m_state == LifeCycleState::GAMEOVER)
    {
        if(m_input_map.IsPressed(GLFW_KEY_SPACE))
        {
            uint32_t win_text_id = m_entity_manager->GetEntityId("win text");
            uint32_t gameover_text_id = m_entity_manager->GetEntityId("gameover text");
            uint32_t title_text_id = m_entity_manager->GetEntityId("title text");
            uint32_t other_title_text_id = m_entity_manager->GetEntityId("other title text");
            uint32_t score_text_id = m_entity_manager->GetEntityId("score text");
            uint32_t timer_id = m_entity_manager->GetEntityId("timer");
            uint32_t player_id = m_entity_manager->GetEntityId("player");
            uint32_t lion_id = m_entity_manager->GetEntityId("lion");

            m_entity_manager->SetEntityState(win_text_id, EntityState::INACTIVE);
            m_entity_manager->SetEntityState(gameover_text_id, EntityState::INACTIVE);
            m_entity_manager->SetEntityState(title_text_id, EntityState::INACTIVE);
            m_entity_manager->SetEntityState(other_title_text_id, EntityState::INACTIVE);
            m_entity_manager->SetEntityState(score_text_id, EntityState::ACTIVE);
            m_entity_manager->SetEntityState(lion_id, EntityState::INACTIVE);
            m_entity_manager->SetEntityState(timer_id, EntityState::ACTIVE);
            m_entity_manager->SetEntityState(player_id, EntityState::ACTIVE);

            PlayerInput& player_input = m_component_manager->GetComponent<PlayerInput>(player_id);
            RigidBody& rigid_body = m_component_manager->GetComponent<RigidBody>(player_id);
            Transform& transform = m_component_manager->GetComponent<Transform>(player_id);
            Texture& texture = m_component_manager->GetComponent<Texture>(player_id);
            QuadMesh& quad_mesh = m_component_manager->GetComponent<QuadMesh>(player_id);
            Timer& timer = m_component_manager->GetComponent<Timer>(timer_id);
            Animation& animation = m_component_manager->GetComponent<Animation>(lion_id);

            animation.speed = 1;

            timer.active = true;
            timer.time = 60;
            
            player_input.score = 0;
            player_input.state = PlayerState::ALIVE;
            rigid_body.velocity[0] = 0;
            rigid_body.velocity[1] = 0;
            rigid_body.velocity[2] = 0;
            rigid_body.acceleration[0] = 0;
            rigid_body.acceleration[1] = 0;
            rigid_body.acceleration[2] = 0;
            transform.position[0] = 0;
            transform.position[1] = 0.5;
            transform.position[2] = 0;
            transform.rotation[0] = 0;
            transform.rotation[1] = 0;
            transform.rotation[2] = 0;
            texture.position[0] = 0;
            texture.position[1] = 150;
            texture.extent[0] = 50;
            texture.extent[1] = 100;
            quad_mesh.extent[0] = 0.5;
            quad_mesh.extent[1] = 1;

        }
    }
}