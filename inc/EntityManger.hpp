#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include <stdint.h>

enum EntityState
{
    INACTIVE,
    ACTIVE,
};

class EntityManger
{
    public:
    EntityManger(uint32_t num_entities);
    ~EntityManger();
    
    void SetEntitySignature(uint32_t entity_id, uint32_t signature);
    void SetEntityState(uint32_t entity_id, EntityState state);
    uint32_t GetNumEntities();
    uint32_t GetEntitySignature(uint32_t entity_id);
    EntityState GetEntityState(uint32_t entity_id);

    private:
    const uint32_t m_num_entities;
    uint32_t* m_entity_signatures;
    EntityState* m_entity_states;


};

#endif // ENTITY_MANAGER_HPP