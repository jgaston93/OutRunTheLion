#include "ComponentManager.hpp"


ComponentManager::ComponentManager(uint32_t num_entities) : m_transform_pool(num_entities),
                                                            m_rigid_body_pool(num_entities),
                                                            m_texture_pool(num_entities),
                                                            m_player_input_pool(num_entities)
{

}

ComponentManager::~ComponentManager()
{

}

template <>
void ComponentManager::AddComponent<Transform>(uint32_t entity_id, Transform component)
{
    m_transform_pool.AddComponent(entity_id, component);
}

template <>
Transform& ComponentManager::GetComponent<Transform>(uint32_t entity_id)
{
    return m_transform_pool.GetComponent(entity_id);
}

template <>
void ComponentManager::AddComponent<RigidBody>(uint32_t entity_id, RigidBody component)
{
    m_rigid_body_pool.AddComponent(entity_id, component);
}

template <>
RigidBody& ComponentManager::GetComponent<RigidBody>(uint32_t entity_id)
{
    return m_rigid_body_pool.GetComponent(entity_id);
}

template <>
void ComponentManager::AddComponent<Texture>(uint32_t entity_id, Texture component)
{
    m_texture_pool.AddComponent(entity_id, component);
}

template <>
Texture& ComponentManager::GetComponent<Texture>(uint32_t entity_id)
{
    return m_texture_pool.GetComponent(entity_id);
}

template <>
void ComponentManager::AddComponent<PlayerInput>(uint32_t entity_id, PlayerInput component)
{
    m_player_input_pool.AddComponent(entity_id, component);
}

template <>
PlayerInput& ComponentManager::GetComponent<PlayerInput>(uint32_t entity_id)
{
    return m_player_input_pool.GetComponent(entity_id);
}