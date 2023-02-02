#include "AISystem.hpp"


AISystem::AISystem(MessageBus& message_bus) : System(message_bus)
{

}

AISystem::~AISystem()
{

}

void AISystem::handleMessage(Message message)
{

}

void AISystem::Update(float delta_time)
{
    uint32_t num_entities = m_entity_manager->GetNumEntities();

    for(uint32_t i = 0; i < num_entities; i++)
    {
        if(m_entity_manager->GetEntityState(i) == EntityState::ACTIVE && 
            ((m_entity_manager->GetEntitySignature(i) & AI_SYSTEM_SIGNATURE) == AI_SYSTEM_SIGNATURE))
        {
            Transform& transform = m_component_manager->GetComponent<Transform>(i);

            if((rand() % 100) < 20)
            {
                transform.rotation[1] = (rand() % 60) - 30;
            }

            if(transform.position[0] < -2)
            {
                transform.rotation[1] = -(rand() % 10);
            }
            else if(transform.position[0] > 2)
            {
                transform.rotation[1] = (rand() % 15);
            }
        }
    }
}