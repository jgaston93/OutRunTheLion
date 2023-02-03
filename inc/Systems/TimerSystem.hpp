#ifndef TIMER_SYSTEM_HPP
#define TIMER_SYSTEM_HPP

#include "System.hpp"
#include "Signatures.hpp"

class Timer : public System
{
    public:
    Timer(MessageBus& message_bus);
    ~Timer();

    void Update(float delta_time);
    void handleMessage(Message message);

    private:
};

#endif // TIMER_SYSTEM_HPP