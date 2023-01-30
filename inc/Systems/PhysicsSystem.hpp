#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "System.hpp"
#include "Signatures.hpp"

class PhysicsSystem : public System
{
    public:
    PhysicsSystem(MessageBus& message_bus);
    ~PhysicsSystem();

    void Update(float delta_time);
    void handleMessage(Message message);

    private:
};

#endif // PHYSICS_SYSTEM_HPP