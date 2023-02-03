#ifndef TIMER_HPP
#define TIMER_HPP

#include <stdint.h>

struct Timer
{
    uint32_t time;
    bool active;
};

#endif // TIMER_HPP