#include <cstdio>

#include "TimerSystem.hpp"


TimerSystem::TimerSystem(MessageBus& message_bus) : System(message_bus)
{

}

TimerSystem::~TimerSystem()
{

}

void TimerSystem::handleMessage(Message message)
{

}

void TimerSystem::Update(float delta_time)
{
    uint32_t num_entities = m_entity_manager->GetNumEntities();

    for(uint32_t i = 0; i < num_entities; i++)
    {
        if(m_entity_manager->GetEntityState(i) == EntityState::ACTIVE && 
            ((m_entity_manager->GetEntitySignature(i) & TIMER_SYSTEM_SIGNATURE) == TIMER_SYSTEM_SIGNATURE))
        {
            Timer& timer = m_component_manager->GetComponent<Timer>(i);
            LabelTexture& label_texture = m_component_manager->GetComponent<LabelTexture>(i);
            if(timer.active)
            {
                timer.time -= delta_time;
                if(timer.time <= 0)
                {
                    timer.time = 0;
                    timer.active = false;
                    Message message;
                    message.message_type = MessageType::TIMEOUT;
                    message.message_data = 0;
                    m_message_bus.postMessage(message);
                }
            }
            snprintf(label_texture.characters, 64, "%d", (int)timer.time);
        }
    }
}