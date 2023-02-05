#include <stdio.h>

#include "BoundsSystem.hpp"


BoundsSystem::BoundsSystem(MessageBus& message_bus) : System(message_bus)
{

}

BoundsSystem::~BoundsSystem()
{

}

void BoundsSystem::handleMessage(Message message)
{

}

void BoundsSystem::Update()
{
    uint32_t num_entities = m_entity_manager->GetNumEntities();

    for(uint32_t i = 0; i < num_entities; i++)
    {
        if(m_entity_manager->GetEntityState(i) == EntityState::ACTIVE && 
            ((m_entity_manager->GetEntitySignature(i) & BOUNDS_SYSTEM_SIGNATURE) == BOUNDS_SYSTEM_SIGNATURE))
        {
            Transform& transform = m_component_manager->GetComponent<Transform>(i);
            Bounds& bounds = m_component_manager->GetComponent<Bounds>(i);

            if(transform.position[0] > bounds.max[0])
            {
                transform.position[0] = bounds.min[0];
            }
            else if(transform.position[0] < bounds.min[0])
            {
                transform.position[0] = bounds.max[0];
            }
            
            if(transform.position[1] > bounds.max[1])
            {
                transform.position[1] = bounds.min[1];
            }
            else if(transform.position[1] < bounds.min[1])
            {
                transform.position[1] = bounds.max[1];
            }

            if(transform.position[2] > bounds.max[2])
            {
                transform.position[2] = bounds.min[2];
            }
            else if(transform.position[2] < bounds.min[2])
            {
                transform.position[2] = bounds.max[2];
            }
        }
    }
}