#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include "System.hpp"
#include "Signatures.hpp"

class CollisionSystem : public System
{
    public:
    CollisionSystem(MessageBus& message_bus);
    ~CollisionSystem();

    void Update();
    void handleMessage(Message message);

    private:
};

#endif // COLLISION_SYSTEM_HPP