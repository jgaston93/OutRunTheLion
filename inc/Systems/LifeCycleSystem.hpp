#ifndef LIFE_CYCLE_SYSTEM_HPP
#define LIFE_CYCLE_SYSTEM_HPP

#include "System.hpp"
#include "Signatures.hpp"
#include "InputMap.hpp"

enum LifeCycleState
{
    START,
    RUN,
    GAMEOVER,
};

class LifeCycleSystem : public System
{
    public:
    LifeCycleSystem(MessageBus& message_bus, InputMap& input_map);
    ~LifeCycleSystem();

    void Update();
    void handleMessage(Message message);

    private:
    InputMap& m_input_map;
    LifeCycleState m_state;
};

#endif // LIFE_CYCLE_SYSTEM_HPP