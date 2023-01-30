#ifndef AI_SYSTEM_HPP
#define AI_SYSTEM_HPP

#include "System.hpp"
#include "Signatures.hpp"

class AISystem : public System
{
    public:
    AISystem(MessageBus& message_bus);
    ~AISystem();

    void Update(float delta_time);
    void handleMessage(Message message);

    private:
};

#endif // AI_SYSTEM_HPP