#ifndef PLAYER_INPUT_SYSTEM_HPP
#define PLAYER_INPUT_SYSTEM_HPP

#include "InputMap.hpp"
#include "EntityManger.hpp"
#include "ComponentManager.hpp"

const uint32_t PLAYER_INPUT_SYSTEM_SIGNATURE = 0x0000000D;

class PlayerInputSystem
{
    public:
    PlayerInputSystem(InputMap& input_map);
    ~PlayerInputSystem();

    void Update(EntityManger& entity_manager, ComponentManager& component_manager);

    private:
    InputMap& m_input_map;
};

#endif // PLAYER_INPUT_SYSTEM_HPP