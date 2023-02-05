#ifndef BOUNDS_SYSTEM_HPP
#define BOUNDS_SYSTEM_HPP

#include "System.hpp"
#include "Signatures.hpp"

class BoundsSystem : public System
{
    public:
    BoundsSystem(MessageBus& message_bus);
    ~BoundsSystem();

    void Update();
    void handleMessage(Message message);

    private:
};

#endif // BOUNDS_SYSTEM_HPP