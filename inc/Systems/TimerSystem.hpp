#ifndef TIMER_SYSTEM_HPP
#define TIMER_SYSTEM_HPP

#include "System.hpp"
#include "Signatures.hpp"

class TimerSystem : public System
{
    public:
    TimerSystem(MessageBus& message_bus);
    ~TimerSystem();

    void Update(float delta_time);
    void handleMessage(Message message);

    private:
};

#endif // TIMER_SYSTEM_HPP