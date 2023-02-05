#include <stdio.h>

#include "CollisionSystem.hpp"


CollisionSystem::CollisionSystem(MessageBus& message_bus) : System(message_bus)
{

}

CollisionSystem::~CollisionSystem()
{

}

void CollisionSystem::handleMessage(Message message)
{

}

void CollisionSystem::Update()
{
    uint32_t num_entities = m_entity_manager->GetNumEntities();

    for(uint32_t i = 0; i < num_entities; i++)
    {
        if(m_entity_manager->GetEntityState(i) == EntityState::ACTIVE && 
            ((m_entity_manager->GetEntitySignature(i) & COLLISION_SYSTEM_SIGNATURE) == COLLISION_SYSTEM_SIGNATURE))
        {
            Transform& transform = m_component_manager->GetComponent<Transform>(i);
            BoundingBox& bounding_box = m_component_manager->GetComponent<BoundingBox>(i);

            for(uint32_t j = 0; j < num_entities; j++)
            {
                if(i != j && m_entity_manager->GetEntityState(j) == EntityState::ACTIVE &&
                    ((m_entity_manager->GetEntitySignature(j) & COLLISION_SYSTEM_SIGNATURE) == COLLISION_SYSTEM_SIGNATURE))
                {
                    Transform& transform_2 = m_component_manager->GetComponent<Transform>(j);
                    BoundingBox& bounding_box_2 = m_component_manager->GetComponent<BoundingBox>(j);

                    bool collision = transform.position[0] - (bounding_box.extent[0] / 2) <= transform_2.position[0] + (bounding_box_2.extent[0] / 2) &&
                                    transform.position[0] + (bounding_box.extent[0] / 2) >= transform_2.position[0] - (bounding_box_2.extent[0] / 2) &&
                                    transform.position[1] - (bounding_box.extent[1] / 2) <= transform_2.position[1] + (bounding_box_2.extent[1] / 2) &&
                                    transform.position[1] + (bounding_box.extent[1] / 2) >= transform_2.position[1] - (bounding_box_2.extent[1] / 2) &&
                                    transform.position[2] - (bounding_box.extent[2] / 2) <= transform_2.position[2] + (bounding_box_2.extent[2] / 2) &&
                                    transform.position[2] + (bounding_box.extent[2] / 2) >= transform_2.position[2] - (bounding_box_2.extent[2] / 2);
                    if(collision && (strcmp("player", m_entity_manager->GetEntityTag(i)) == 0 || strcmp("player", m_entity_manager->GetEntityTag(j)) == 0))
                    {
                        Message message;
                        message.message_type = MessageType::COLLISION;
                        message.message_data = (i << 16) + j;
                        m_message_bus.postMessage(message);

                    }

                }
            }
        }
    }
}