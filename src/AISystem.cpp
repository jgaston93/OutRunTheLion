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

        }
    }
}