#ifndef COMPONENT_MANAGER_HPP
#define COMPONENT_MANAGER_HPP

#include "ComponentPool.hpp"
#include "Transform.hpp"
#include "Texture.hpp"
#include "RigidBody.hpp"
#include "PlayerInput.hpp"

class ComponentManager
{
    public:
    ComponentManager(uint32_t num_entities);
    ~ComponentManager();

    template <typename T>
    void AddComponent(uint32_t entity_id, T component);

    template <typename T>
    T& GetComponent(uint32_t entity_id);

    private:
    ComponentPool<Transform> m_transform_pool;
    ComponentPool<Texture> m_texture_pool;
    ComponentPool<RigidBody> m_rigid_body_pool;
    ComponentPool<PlayerInput> m_player_input_pool;
};

#endif // COMPONENT_MANAGER_HPP