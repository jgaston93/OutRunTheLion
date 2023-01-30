#include "AnimationSystem.hpp"


AnimationSystem::AnimationSystem(MessageBus& message_bus) : System(message_bus)
{

}

AnimationSystem::~AnimationSystem()
{

}

void AnimationSystem::handleMessage(Message message)
{

}

void AnimationSystem::Update(float delta_time)
{
    uint32_t num_entities = m_entity_manager->GetNumEntities();

    for(uint32_t i = 0; i < num_entities; i++)
    {
        if(m_entity_manager->GetEntityState(i) == EntityState::ACTIVE && 
            ((m_entity_manager->GetEntitySignature(i) & ANIMATION_SYSTEM_SIGNATURE) == ANIMATION_SYSTEM_SIGNATURE))
        {
            Texture& texture = m_component_manager->GetComponent<Texture>(i);
            Animation& animation = m_component_manager->GetComponent<Animation>(i);
            
            if(!animation.paused)
            {
                animation.counter += delta_time;
                if(animation.counter > animation.speed)
                {
                    animation.current_frame++;
                    texture.position[0] += texture.extent[0];
                    // texture.position[1] += texture.extent[1];
                    if(animation.current_frame >= animation.num_frames)
                    {
                        animation.current_frame = 0;
                        texture.position[0] = 0;
                        // texture.position[1] = 0;
                    }
                    animation.counter = 0;
                }
            }
        }
    }
}