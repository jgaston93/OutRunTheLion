#include "PhysicsSystem.hpp"


PhysicsSystem::PhysicsSystem()
{

}

PhysicsSystem::~PhysicsSystem()
{

}

void PhysicsSystem::Update(float delta_time, EntityManger& entity_manager, ComponentManager& component_manager)
{
    
    uint32_t num_entities = entity_manager.GetNumEntities();

    for(uint32_t i = 0; i < num_entities; i++)
    {
        if(entity_manager.GetEntityState(i) == EntityState::ACTIVE && entity_manager.GetEntitySignature(i) & PHYSICS_SYSTEM_SIGNATURE > 0)
        {
            Transform& transform = component_manager.GetComponent<Transform>(i);
            RigidBody& rigid_body = component_manager.GetComponent<RigidBody>(i);

            
        }
    }
}