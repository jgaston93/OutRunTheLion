#include "EntityManger.hpp"


EntityManger::EntityManger(uint32_t num_entities) : m_num_entities(num_entities),
                                                    m_entity_signatures(new uint32_t[num_entities]),
                                                    m_entity_states(new EntityState[num_entities])
{

}

EntityManger::~EntityManger()
{
    delete[] m_entity_signatures;
    delete[] m_entity_states;
}

void EntityManger::SetEntitySignature(uint32_t entity_id, uint32_t signature)
{
    m_entity_signatures[entity_id] = signature;
}

void EntityManger::SetEntityState(uint32_t entity_id, EntityState state)
{
    m_entity_states[entity_id] = state;
}

uint32_t EntityManger::GetNumEntities()
{
    return m_num_entities;
}

uint32_t EntityManger::GetEntitySignature(uint32_t entity_id)
{
    return m_entity_signatures[entity_id];
}

EntityState EntityManger::GetEntityState(uint32_t entity_id)
{
    return m_entity_states[entity_id];
}
