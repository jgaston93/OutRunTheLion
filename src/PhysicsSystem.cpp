#include <stdio.h>

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
        if(entity_manager.GetEntityState(i) == EntityState::ACTIVE && 
            ((entity_manager.GetEntitySignature(i) & PHYSICS_SYSTEM_SIGNATURE) == PHYSICS_SYSTEM_SIGNATURE))
        {
            Transform& transform = component_manager.GetComponent<Transform>(i);
            RigidBody& rigid_body = component_manager.GetComponent<RigidBody>(i);

            rigid_body.velocity[0] += rigid_body.acceleration[0] * delta_time * 0.9;
            rigid_body.velocity[1] += rigid_body.acceleration[1] * delta_time * 0.9;
            rigid_body.velocity[2] += rigid_body.acceleration[2] * delta_time * 0.9;
            
            if(rigid_body.velocity[2] > 0)
            {
                rigid_body.velocity[2] = 0;
            }
            else if(rigid_body.velocity[2] < -22)
            {
                rigid_body.velocity[2] = -22;
            }

            mat4x4 rotation_matrix;
            mat4x4_identity(rotation_matrix);
            mat4x4_rotate_Z(rotation_matrix, rotation_matrix, transform.rotation[0] * M_PI / 180.0);
            mat4x4_rotate_Y(rotation_matrix, rotation_matrix, transform.rotation[1] * M_PI / 180.0);
            mat4x4_rotate_X(rotation_matrix, rotation_matrix, transform.rotation[2] * M_PI / 180.0);

            vec4 world_velocity;
            vec4 body_velocity = { rigid_body.velocity[0], rigid_body.velocity[1], rigid_body.velocity[2], 1 };
            mat4x4_mul_vec4(world_velocity, rotation_matrix, body_velocity);

            transform.position[0] += world_velocity[0] * delta_time;
            transform.position[1] += world_velocity[1] * delta_time;
            transform.position[2] += world_velocity[2] * delta_time;
        }
    }
}