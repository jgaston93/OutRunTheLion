#ifndef ANIMATION_SYSTEM_HPP
#define ANIMATION_SYSTEM_HPP

#include "System.hpp"
#include "Signatures.hpp"

class AnimationSystem : public System
{
    public:
    AnimationSystem(MessageBus& message_bus);
    ~AnimationSystem();

    void Update(float delta_time);
    void handleMessage(Message message);

    private:
};

#endif // ANIMATION_SYSTEM_HPP