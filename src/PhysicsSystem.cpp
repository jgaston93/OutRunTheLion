#include "PhysicsSystem.hpp"
#include "Matrix.hpp"
#include "Rotation.hpp"


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

            rigid_body.velocity.x += rigid_body.acceleration.x * delta_time;
            rigid_body.velocity.y += rigid_body.acceleration.y * delta_time;
            rigid_body.velocity.z += rigid_body.acceleration.z * delta_time;

            Matrix3x3 rotation_matrix = CreateRotationMatrix(transform.rotation.x, transform.rotation.y, transform.rotation.z);
            Vector3 rotated_velocity = DotProduct(rotation_matrix, rigid_body.velocity);

            transform.position.x += rotated_velocity.x * delta_time;
            transform.position.y += rotated_velocity.y * delta_time;
            transform.position.z += rotated_velocity.z * delta_time;
        }
    }
}