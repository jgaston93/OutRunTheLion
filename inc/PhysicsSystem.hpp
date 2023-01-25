#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "InputMap.hpp"
#include "EntityManger.hpp"
#include "ComponentManager.hpp"

const uint32_t PHYSICS_SYSTEM_SIGNATURE = 0x00000005;

class PhysicsSystem
{
    public:
    PhysicsSystem();
    ~PhysicsSystem();

    void Update(float delta_time, EntityManger& entity_manager, ComponentManager& component_manager);

    private:
};

#endif // PHYSICS_SYSTEM_HPP